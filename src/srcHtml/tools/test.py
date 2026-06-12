import re

def replace_comment_tags(html_string):
  """
  Finds and replaces HTML comment tags of the form <!--L_*--> with L_*.

  Args:
    html_string: The HTML string to process.

  Returns:
    The modified HTML string with comment tags replaced.
  """
  pattern = re.compile(r"<!--(L_[A-Za-z0-9_]+)-->(.+)<")
  
  def replace(match):
      print(f"match: {match}\n")
      return '" ' + match.group(1) +' "<'
  
  return pattern.sub(replace, html_string)


# Example Usage:
html_string = """
<form action='/rst' method='get'>
  <button class='btn'><!--L_RESET-->Reset</button>
  <p><!--L_PARAGRAPH-->This is a paragraph</p>
  <div><!--L_MY_DIV--></div>
  <a href="/test"><!--L_LINK_TEST-->Link</a>
</form>
<p>This is a text without comment</p>
"""

modified_html = replace_comment_tags(html_string)
print(modified_html)