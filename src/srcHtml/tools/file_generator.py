import json
import sys
import os
import argparse
import math
import re

gInputDir = ''

def makeFilePath(fname):
    if gInputDir == '':
        return fname
    return os.path.join(gInputDir, fname)

def replaceVar(fstr, varEntry, skipLangTranslation):
    tp = varEntry["value"]["type"]
    value = ""
    if "string" == tp:
        value = varEntry["value"]["value"]
    elif "template" == tp:
        value = translate(varEntry["value"], skipLangTranslation)
     
    fstr = fstr.replace(varEntry["var"], value)

    return fstr


def translate(data, skipLangTranslation):
    format = ''
    content = ''
    fname = makeFilePath(data["template"])
    if "format" in data:
        format = data["format"]

    if format == "css" or format == "html" or format == "js":
        if os.path.exists(fname):
            # print(f"Processing file '{fname}'.")
            file = open(fname, "r", encoding='utf-8')
            # Read each line one by one
            for line in file:  
                line = line.strip()
                if format == "js":
                    if not skipLangTranslation:
                        line = replaceJavascriptLanguageTags(line)
                    line = line.replace("\\", "\\\\")
                elif format == "html":
                    if not skipLangTranslation:
                        line = replaceHTMLLanguageTags(line)
                        line = replaceHTMLTokens(line)
                elif format == "css":
                    line = line.replace("\"", "\\\"")
                if not skipLangTranslation:
                    line += "\\n"
                content += line
            # Close the file
            file.close()

        else:
            print(f"The file '{fname}' does not exist.")
            content = ''

    else:
        if os.path.exists(fname):
            file = open(fname, "r")
            content = file.read()
        else:
            print(f"The file '{fname}' does not exist.")
            content = ''
    
    if "replaceMap" in data:
        for item in data["replaceMap"]:
            content = replaceVar(content, item, skipLangTranslation)
    # print('content: ')
    # print(content)
    return content

def translateFile(jsonFname, outputFname, skipLangTranslation):
    data = ''
    with open(jsonFname, 'r') as file:
        data = json.load(file)
        file.close()

    content = translate(data, skipLangTranslation)
    of = open(makeFilePath(outputFname), "w", encoding='utf-8')
    of.write(content)
    of.close()

def replaceHTMLLanguageTags(html):
    # Case 3: '<!--L_ID-->' -> '" L_ID "' (token as sole attribute value, no default text)
    case3 = re.compile(r"'<!--(L_[A-Za-z0-9_]+)-->'")
    html = case3.sub(lambda m: f"'\" {m.group(1)} \"'", html)

    # Case 1: <!--L_ID-->'string' -> '"L_HTML_LANGUAGE"'
    case1 = re.compile(r"<!--(L_[A-Za-z0-9_]+)-->'([^']*)'")
    html = case1.sub(lambda m: f"'\" {m.group(1)} \"'", html)

    # Case 2: <!--L_ID-->text...< (text up to next '<')
    case2 = re.compile(r'<!--(L_[A-Za-z0-9_]+)-->([^<]*)<')
    html = case2.sub(lambda m: f'" {m.group(1)} "<', html)

    return html

def replaceHTMLTokens(html):
    # Case 3: '<!--D_ID-->' -> '" ID "' (token as sole attribute value, no default text)
    case3 = re.compile(r"'<!--D_([A-Za-z0-9_]+)-->'")
    html = case3.sub(lambda m: f"'\" {m.group(1)} \"'", html)

    # Case 1: <!--D_ID-->'string' -> '"ID"'
    case1 = re.compile(r"<!--D_([A-Za-z0-9_]+)-->'([^']*)'")
    html = case1.sub(lambda m: f"'\" {m.group(1)} \"'", html)

    # Case 2: <!--D_ID-->text...< (text up to next '<')
    case2 = re.compile(r'<!--D_([A-Za-z0-9_]+)-->([^<]*)<')
    html = case2.sub(lambda m: f'" {m.group(1)} "<', html)

    return html


def replaceJavascriptLanguageTags2(js_code):
    # Pattern: /*L_ABC123*/'String to be replaced'
    pattern = r"/\*(L_[A-Za-z0-9_]+)\*/'[^']*'"
    # Replace with: "  L_ABC123 "
    return re.sub(pattern, r'" \1" ', js_code)

def replaceJavascriptLanguageTags(js_code):
    # Pattern: /*L_ABC123*/'String to be replaced'
    case1 = re.compile(r"/\*(L_[A-Za-z0-9_]+)\*/'[^']*'")
    # Replace with: "  L_ABC123 "
    return case1.sub(lambda m: f"'\" {m.group(1)} \"'", js_code)


# example:
if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog='File Generator',
        description='Generate target file from a template and variables')
    parser.add_argument("json", help="Filename of JSON file with inputs", type=str)
    parser.add_argument("output", help="Output filename", type=str)
    # Optional argument to skip translation of language tags
    parser.add_argument('-s', '--skipLangTranslation', action='store_true', help='Skip translation of language tags.')

    args = parser.parse_args()

    if args.json is None:
        print("json argument is needed.")
        sys.exit()

    gInputDir = os.path.dirname(args.json)
    translateFile(args.json, args.output, args.skipLangTranslation)
