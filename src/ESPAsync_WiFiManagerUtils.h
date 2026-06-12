// ESPAsync_WiFiManagerUtils.h
// Small utilities and shared data-structures moved out of the main Impl header
#ifndef ESPAsync_WiFiManagerUtils_h
#define ESPAsync_WiFiManagerUtils_h

#include <Arduino.h>
#include <ESPAsyncWebServer.h>


// Forward declarations
class AsyncWebServerRequest;


#if defined(ESP8266) ||  defined(ESP32)
    // New macro names (item suffix) to make intent clearer
    #define WM_MEMORY_BLOCK_ITEM(name) { name, nullptr, strlen(name) }
    #define WM_MEMORY_P_BLOCK_ITEM(name) { nullptr, name, strlen_P((PGM_P)name) }
    // Backwards-compatible aliases for existing code
    #define WM_MEMORY_BLOCK2(name) { name, nullptr, nullptr strlen(name) }
    #define WM_MEMORY_P_BLOCK2(name) { nullptr, nullptr, name, strlen_P((PGM_P)name) }
    #define WM_MEMORY_S_BLOCK2(name) { nullptr, &name, nullptr, name.length() }
#else
    // New macro names (item suffix) to make intent clearer
    #define WM_MEMORY_BLOCK_ITEM(name) { name, strlen(name) }
    #define WM_MEMORY_P_BLOCK_ITEM(name) WM_MEMORY_BLOCK_ITEM(name)
    // Backwards-compatible aliases
    #define WM_MEMORY_BLOCK(name) WM_MEMORY_BLOCK_ITEM(name)
    #define WM_MEMORY_P_BLOCK(name) WM_MEMORY_P_BLOCK_ITEM(name)
    #define WM_MEMORY_BLOCK2(name) { name, nullptr, strlen(name) }
    #define WM_MEMORY_P_BLOCK2(name) WM_MEMORY_BLOCK2(name)
    #define WM_MEMORY_S_BLOCK2(name) { nullptr, &name, name.length() }
#endif

#define WM_MEMORY_BLOCK(name) { name, ARRAY_LENGTH(name) }


// Length of __FlashStringHelper string
#define STRLEN_P(pStr) ((pStr) ? strlen_P((PGM_P)pStr) : 0)
#define REPLACE_P(str, substr, pNewSubstr) str.replace(substr, (pNewSubstr) ? pNewSubstr : FPSTR(WM_PK_EMPTY_STR))
#define APPEND_P(str, pStr) if (pStr) str += FPSTR(pStr)
#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))


// Memory block used to store pieces of HTTP responses (RAM or flash)
struct HTTPMemoryBlockItem
{
    const char* pMemBlock;  // Pointer to RAM memory block
#if defined(ESP8266) || defined(ESP32)
    const __FlashStringHelper* pPMemBlock; // Pointer to program memory block
#endif
    size_t size;
};

// Backwards compatibility aliases
using HttpMemoryBlock = HTTPMemoryBlockItem;
using MemoryBlock = HTTPMemoryBlockItem;

// Memory block variant that can point to a String
struct HTTPMemoryBlock2Item
{
    const char* pMemBlock;  // Pointer to RAM memory block
    const String* pString;  // Pointer to String
#if defined(ESP8266) || defined(ESP32)
    const __FlashStringHelper* pPMemBlock; // Pointer to program memory block
#endif
    size_t size;
};

// Backwards compatibility aliases
using HttpMemoryBlock2 = HTTPMemoryBlock2Item;
using MemoryBlock2 = HTTPMemoryBlock2Item;

struct HTTPMemoryBlock
{
    const HTTPMemoryBlockItem* pItems;  // Pointer to array of HTTPMemoryBlock
    size_t count;                  // Number of items in the array
};

struct HTTPMemoryBlock2
{
    const HTTPMemoryBlock2Item* pItems;  // Pointer to array of HTTPMemoryBlock
    size_t count;                  // Number of items in the array
};

struct HTTPHeaderItem
{
    const __FlashStringHelper* pName;  // Pointer to string in program memory
    const __FlashStringHelper* pValue; // Pointer to string in program memory
};

struct HTTPHeaderBlock
{
    const HTTPHeaderItem* pItems;  // Pointer to array of HTTPHeaderItem
    size_t count;                  // Number of items in the array
};

struct HTTPResponseBlock
{
    const __FlashStringHelper* pContentType; // Pointer to string in program memory
    const HTTPMemoryBlock* pBlock;  // Pointer to HttpMemoryBlock
    const HTTPHeaderBlock* pHeader; // Pointer to HTTPHeaderBlock
};

struct HTTPResponseBlock2
{
    const __FlashStringHelper* pContentType; // Pointer to string in program memory
    const HTTPMemoryBlock2* pBlock;  // Pointer to HttpMemoryBlock
    const HTTPHeaderBlock* pHeader; // Pointer to HTTPHeaderBlock
};

extern const HTTPHeaderBlock gHTMLHeaders;
extern const HTTPHeaderBlock gCSSHeaders;
extern const HTTPHeaderBlock gJSHeaders;


#define WM_DEFINE_STATIC_FILE_AS_SINGLE_BLOCK(NAME, CONTENT, CONTENT_TYPE, HEADERS) \
    HTTPMemoryBlockItem NAME##Chunks[] = { \
        WM_MEMORY_P_BLOCK_ITEM(FPSTR(CONTENT)) \
    }; \
    const HTTPMemoryBlock NAME##Block = WM_MEMORY_BLOCK(NAME##Chunks); \
    const HTTPResponseBlock NAME = { \
        FPSTR(CONTENT_TYPE), \
        &NAME##Block, \
        &HEADERS, \
    }

// Macro to define a static HTML file stored in program memory as a single memory block
#define WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(NAME, CONTENT) \
    WM_DEFINE_STATIC_FILE_AS_SINGLE_BLOCK(NAME, CONTENT, WM_HTTP_HEAD_CT_TEXT_HTML, gHTMLHeaders)

// Macro to define a static Javascript file stored in program memory as a single memory block
#define WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(NAME, CONTENT) \
    WM_DEFINE_STATIC_FILE_AS_SINGLE_BLOCK(NAME, CONTENT, WM_HTTP_HEAD_CT_TEXT_JAVASCRIPT, gHTMLHeaders)

// Macro to define a static Javascript file stored in program memory as a single memory block
#define WM_DEFINE_STATIC_CSS_FILE_AS_SINGLE_BLOCK(NAME, CONTENT) \
    WM_DEFINE_STATIC_FILE_AS_SINGLE_BLOCK(NAME, CONTENT, WM_HTTP_HEAD_CT_TEXT_CSS, gCSSHeaders)



////////////////////////////////////////////////////

const char WM_HTTP_HEAD_CL[]         = "Content-Length";
const char WM_HTTP_HEAD_CT_TEXT_HTML[] = "text/html";
const char WM_HTTP_HEAD_CT_TEXT_PLAIN[] = "text/plain";
const char WM_HTTP_HEAD_CT_TEXT_CSS[] = "text/css";
const char WM_HTTP_HEAD_CT_TEXT_JAVASCRIPT[] = "text/javascript";
const char WM_HTTP_HEAD_CT_JSON[]    ="application/json";

////////////////////////////////////////////////////

const char WM_HTTP_CACHE_CONTROL[]   = "Cache-Control";
const char WM_HTTP_NO_STORE[]        = "no-cache, no-store, must-revalidate";
const char WM_HTTP_PRAGMA[]          = "Pragma";
const char WM_HTTP_NO_CACHE[]        = "no-cache";
const char WM_HTTP_EXPIRES[]         = "Expires";
const char WM_HTTP_CORS[]            = "Access-Control-Allow-Origin";
const char WM_HTTP_CORS_ALLOW_ALL[]  = "*";


namespace ESPAsync_WiFiManagerUtils {
    // Utility function to respond with text/html content type
    void responseTextHtml(AsyncWebServerRequest *pRequest, const String& page);

    // Utility function to respond with application/json content type
    void responseApplJson(AsyncWebServerRequest *pRequest, const String& page);

    // Utility function to respond with static content and specified content type
    void responseText(AsyncWebServerRequest *pRequest, const HTTPResponseBlock *pBlock);

    // Utility function to respond with static content and specified content type
    void responseText(AsyncWebServerRequest *pRequest, const HTTPResponseBlock2 *pBlock);
}

#endif // ESPAsync_WiFiManagerUtils_h
