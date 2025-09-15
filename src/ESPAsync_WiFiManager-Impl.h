/****************************************************************************************************************************
  ESPAsync_WiFiManager-Impl.h
  For ESP8266 / ESP32 boards

  ESPAsync_WiFiManager is a library for the ESP8266/Arduino platform, using (ESP)AsyncWebServer to enable easy
  configuration and reconfiguration of WiFi credentials using a Captive Portal.

  Modified from
  1. Tzapu               (https://github.com/tzapu/WiFiManager)
  2. Ken Taylor          (https://github.com/kentaylor)
  3. Alan Steremberg     (https://github.com/alanswx/ESPAsyncWiFiManager)
  4. Khoi Hoang          (https://github.com/khoih-prog/ESP_WiFiManager)

  Built by Khoi Hoang https://github.com/khoih-prog/ESPAsync_WiFiManager
  Licensed under MIT license

  Version: 1.15.1

  Version Modified By  Date      Comments
  ------- -----------  ---------- -----------
  1.0.11  K Hoang      21/08/2020 Initial coding to use (ESP)AsyncWebServer instead of (ESP8266)WebServer. Bump up to v1.0.11
                                  to sync with ESP_WiFiManager v1.0.11
  ...
  1.10.0  K Hoang      29/12/2021 Fix `multiple-definitions` linker error and weird bug related to src_cpp
  1.11.0  K Hoang      17/01/2022 Enable compatibility with old code to include only ESP_WiFiManager.h
  1.12.0  K Hoang      10/02/2022 Add support to new ESP32-S3
  1.12.1  K Hoang      11/02/2022 Add LittleFS support to ESP32-C3. Use core LittleFS instead of Lorol's LITTLEFS for v2.0.0+
  1.12.2  K Hoang      13/03/2022 Optimize code by using passing by `reference` instead of by `value`
  1.13.0  K Hoang      18/08/2022 Using AsynsDNSServer instead of DNSServer
  1.14.0  K Hoang      09/09/2022 Fix ESP32 chipID and add ESP_getChipOUI()
  1.14.1  K Hoang      15/09/2022 Remove dependency on ESP_AsyncWebServer, ESPAsyncTCP and AsyncTCP in `library.properties`
  1.15.0  K Hoang      07/10/2022 Optional display Credentials (SSIDs, PWDs) in Config Portal
  1.15.1  K Hoang      25/10/2022 Using random channel for softAP without password. Add astyle using allman style
 *****************************************************************************************************************************/

#pragma once

#ifndef ESPAsync_WiFiManager_Impl_h
#define ESPAsync_WiFiManager_Impl_h

#ifdef ESP8266
    #define MEMORY_BLOCK(name) { name, nullptr, strlen(name) }
    #define MEMORY_P_BLOCK(name) { nullptr, name, strlen_P((PGM_P)name) }
    #define MEMORY_BLOCK2(name) { name, nullptr, nullptr strlen(name) }
    #define MEMORY_P_BLOCK2(name) { nullptr, nullptr, name, strlen_P((PGM_P)name) }
    #define MEMORY_S_BLOCK2(name) { nullptr, &name, nullptr, name.length() }
#else
    #define MEMORY_BLOCK(name) { name, strlen(name) }
    #define MEMORY_P_BLOCK(name) MEMORY_BLOCK(name)
    #define MEMORY_BLOCK2(name) { name, nullptr, strlen(name) }
    #define MEMORY_P_BLOCK2(name) MEMORY_BLOCK2(name)
    #define MEMORY_S_BLOCK2(name) { nullptr, &name, name.length() }
#endif

// Length of __FlashStringHelper string
#define STRLEN_P(pStr) ((pStr) ? strlen_P((PGM_P)pStr) : 0)
#define REPLACE_P(str, substr, pNewSubstr) str.replace(substr, (pNewSubstr) ? pNewSubstr : FPSTR(WM_PK_EMPTY_STR))
#define APPEND_P(str, pStr) if (pStr) str += FPSTR(pStr)
#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))


struct MemoryBlock
{
    const char* pMemBlock;  // Pointer to RAM memory block
#ifdef ESP8266
    const __FlashStringHelper* pPMemBlock; // Pointer to program memory block
#endif
    size_t size;
};

struct MemoryBlock2
{
    const char* pMemBlock;  // Pointer to RAM memory block
    const String* pString;  // Pointer to String
#ifdef ESP8266
    const __FlashStringHelper* pPMemBlock; // Pointer to program memory block
#endif
    size_t size;
};

MemoryBlock gUpdateOTAHtml[] = {
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_1)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_FIRMWARE_UPDATE_MSG)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_2)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_OTA_SCRIPT)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_STYLE_S)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_STYLE)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_STYLE_E)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_3)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_H2_S)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_FIRMWARE_UPDATE_MSG)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_H2_E)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_OTA_UPDATE)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_FOOT))
};

MemoryBlock gTrainControlHtml[] = {
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_1)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_FIRMWARE_UPDATE_MSG)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_2)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_PWMCTRL_SCRIPT)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_STYLE_S)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_TRAIN_STYLE)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_STYLE_E)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_HEAD_3)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTTP_TRAIN_CONTROL)),
    MEMORY_P_BLOCK(FPSTR(WM_PK_HTML_FOOT))
};

//////////////////////////////////////////
// PK
namespace {
    void CopyBlock(uint8_t *pBufferWithOffset, const MemoryBlock *pBlock, size_t blockOffset, 
        size_t blockSize)
    {
        if (pBlock->pMemBlock != nullptr)
        {
            memcpy(pBufferWithOffset, pBlock->pMemBlock + blockOffset, blockSize);
        }
    #ifdef ESP8266
        else if (pBlock->pPMemBlock != nullptr)
        {
            PGM_P p = reinterpret_cast<PGM_P>(pBlock->pPMemBlock);
            memcpy_P(pBufferWithOffset, p + blockOffset, blockSize);
        }
    #endif
    }

    void CopyBlock(uint8_t *pBufferWithOffset, const MemoryBlock2 *pBlock, size_t blockOffset, 
        size_t blockSize)
    {
        if (pBlock->pMemBlock != nullptr)
        {
            memcpy(pBufferWithOffset, pBlock->pMemBlock + blockOffset, blockSize);
        }
        else if (pBlock->pString != nullptr)
        {
            PGM_P p = reinterpret_cast<PGM_P>(pBlock->pPMemBlock);
            memcpy(pBufferWithOffset, pBlock->pString->c_str() + blockOffset, blockSize);
        }
    #ifdef ESP8266
        else if (pBlock->pPMemBlock != nullptr)
        {
            PGM_P p = reinterpret_cast<PGM_P>(pBlock->pPMemBlock);
            memcpy_P(pBufferWithOffset, p + blockOffset, blockSize);
        }
    #endif
    }

    inline size_t BlockSize(const MemoryBlock2 *pBlocks)
    {
        return pBlocks ? (pBlocks->pString ? pBlocks->pString->length() : pBlocks->size) : 0;
    }

    // Function to send memory block
    size_t SendMemoryBlock (const MemoryBlock *pBlocks, unsigned int countOfBlocks,
        uint8_t *pBuffer, size_t bufLen, size_t index)
    {
        uint8_t *pBufferWithOffset = pBuffer;
        size_t remainingBufLen = (bufLen & 0xFFFC); // Align buffer length to 4
        size_t currentIndex = index;
        size_t blockStart = 0;
        size_t sizeOfCopiedMem = 0;

        //LOGDEBUG1("SendMemoryBlock: bufLen=", bufLen);
        //LOGDEBUG1("SendMemoryBlock: remainingBufLen=", remainingBufLen);
        //LOGDEBUG1("SendMemoryBlock: countOfBlocks=", countOfBlocks);

        for (unsigned int iBlock = 0; iBlock < countOfBlocks && remainingBufLen > 0; iBlock++)
        {

            size_t blockEnd = blockStart + pBlocks[iBlock].size;

            while (blockStart <= currentIndex &&
                   blockEnd > currentIndex &&
                   remainingBufLen > 0)
            {
                size_t blockOffset = currentIndex - blockStart;
                size_t blockSize = pBlocks[iBlock].size - blockOffset;
                if (blockSize >= remainingBufLen)
                {
                    blockSize = remainingBufLen;
                }

                CopyBlock(pBufferWithOffset, pBlocks + iBlock, blockOffset, blockSize);
                pBufferWithOffset += blockSize;
                sizeOfCopiedMem += blockSize;
                currentIndex += blockSize;
                remainingBufLen -= blockSize;
            }

            blockStart = blockEnd;
        }

        //LOGDEBUG1("SendMemoryBlock: sizeOfCopiedMem=", sizeOfCopiedMem);
        return sizeOfCopiedMem;
    }
    
    // Function to send memory block
    size_t SendMemoryBlock (const MemoryBlock2 *pBlocks, unsigned int countOfBlocks,
        uint8_t *pBuffer, size_t bufLen, size_t index)
    {
        uint8_t *pBufferWithOffset = pBuffer;
        size_t remainingBufLen = (bufLen & 0xFFFC); // Align buffer length to 4
        size_t currentIndex = index;
        size_t blockStart = 0;
        size_t sizeOfCopiedMem = 0;

        //LOGDEBUG1("SendMemoryBlock: bufLen=", bufLen);
        //LOGDEBUG1("SendMemoryBlock: remainingBufLen=", remainingBufLen);
        //LOGDEBUG1("SendMemoryBlock: countOfBlocks=", countOfBlocks);

        for (unsigned int iBlock = 0; iBlock < countOfBlocks && remainingBufLen > 0; iBlock++)
        {
            size_t bs = BlockSize(pBlocks + iBlock);
            size_t blockEnd = blockStart + bs;

            while (blockStart <= currentIndex &&
                   blockEnd > currentIndex &&
                   remainingBufLen > 0)
            {
                size_t blockOffset = currentIndex - blockStart;
                size_t blockSize = bs - blockOffset;
                if (blockSize >= remainingBufLen)
                {
                    blockSize = remainingBufLen;
                }

                CopyBlock(pBufferWithOffset, pBlocks + iBlock, blockOffset, blockSize);
                pBufferWithOffset += blockSize;
                sizeOfCopiedMem += blockSize;
                currentIndex += blockSize;
                remainingBufLen -= blockSize;
            }

            blockStart = blockEnd;
        }

        //LOGDEBUG1("SendMemoryBlock: sizeOfCopiedMem=", sizeOfCopiedMem);
        return sizeOfCopiedMem;
    }
    
/*
    void buildHtmlPage(String& page,
        const __FlashStringHelper *pTitle,
        const __FlashStringHelper *pScript,
        const __FlashStringHelper *pStyle,
        const String& customHeadElem,
        const String& headTitle,
        const String& body,
        const String& optScriptElem = "")
    {
        LOGDEBUG(F("buildHtmlPage (start)"));

        size_t sz = STRLEN_P(pTitle) + STRLEN_P(pScript) + STRLEN_P(pStyle) + customHeadElem.length() +
            headTitle.length() + body.length() + optScriptElem.length() + STRLEN_P(FPSTR(WM_PK_HTTP_HTML));

        page.reserve(sz);
        LOGDEBUG1("buildHtmlPage: calc capacity", sz);
        page += FPSTR(WM_PK_HTTP_HTML);
        //LOGDEBUG1("buildHtmlPage: calc capacity", page.capacity());
        page.replace(F("%{body}%"), body);
        LOGDEBUG("buildHtmlPage: after {body}");
        page.replace(F("%{customHeadElem}%"), customHeadElem);
        page.replace(F("%{optScriptElem}%"), optScriptElem);
        page.replace(F("%{headTitle}%"), headTitle);
        REPLACE_P(page, F("%{title}%"), pTitle);
        LOGDEBUG("buildHtmlPage: before {script}");
        REPLACE_P(page, F("%{script}%"), pScript);
        LOGDEBUG("buildHtmlPage: after {script}");
        REPLACE_P(page, F("%{style}%"), pStyle);
        LOGDEBUG("buildHtmlPage: after {style}");
    }
*/

    void buildHtmlPage(String& page,
        const __FlashStringHelper *pTitle,
        const __FlashStringHelper *pScript,
        const __FlashStringHelper *pStyle,
        const String& customHeadElem,
        const String& headTitle,
        const String& body,
        const String& optScriptElem = "")
    {
        LOGDEBUG(F("buildHtmlPage2 (start)"));

        size_t sz = STRLEN_P(WM_PK_HTML_HEAD_1) + STRLEN_P(pTitle) + STRLEN_P(WM_PK_HTML_HEAD_2) +
            STRLEN_P(pScript) + optScriptElem.length() + customHeadElem.length() + STRLEN_P(WM_PK_HTML_HEAD_3) +
            body.length() + STRLEN_P(WM_PK_HTML_FOOT);

        if (pStyle) 
        {
            sz += STRLEN_P(WM_PK_HTML_STYLE_S);
            sz += STRLEN_P(pStyle);
            sz += STRLEN_P(WM_PK_HTML_STYLE_E);
        }

        if (!headTitle.isEmpty())
        {
            sz += STRLEN_P(WM_PK_HTML_H2_S);
            sz += headTitle.length();
            sz += STRLEN_P(WM_PK_HTML_H2_E);
        }

        //+ customHeadElem.length()
        //    headTitle.length() + body.length() + optScriptElem.length() + STRLEN_P(FPSTR(WM_PK_HTTP_HTML));

        page.reserve(sz);
        LOGDEBUG1("buildHtmlPage2: calc capacity: ", sz);
        page += FPSTR(WM_PK_HTML_HEAD_1);
        APPEND_P(page, pTitle);
        page += FPSTR(WM_PK_HTML_HEAD_2);
        APPEND_P(page, pScript);
        page += optScriptElem;
        if (pStyle) 
        {
            APPEND_P(page, WM_PK_HTML_STYLE_S);
            APPEND_P(page, pStyle);
            APPEND_P(page, WM_PK_HTML_STYLE_E);
        }
        page += customHeadElem;
        page += FPSTR(WM_PK_HTML_HEAD_3);
        if (!headTitle.isEmpty())
        {
            APPEND_P(page, WM_PK_HTML_H2_S);
            page += headTitle;
            APPEND_P(page, WM_PK_HTML_H2_E);
        }
        page += body;
        page += FPSTR(WM_PK_HTML_FOOT);

        LOGDEBUG1("buildHtmlPage2: length: ", page.length());

        /*
        page.replace(F("%{body}%"), body);
        LOGDEBUG("buildHtmlPage: after {body}");
        page.replace(F("%{customHeadElem}%"), customHeadElem);
        page.replace(F("%{optScriptElem}%"), optScriptElem);
        page.replace(F("%{headTitle}%"), headTitle);
        REPLACE_P(page, F("%{title}%"), pTitle);
        LOGDEBUG("buildHtmlPage: before {script}");
        REPLACE_P(page, F("%{script}%"), pScript);
        LOGDEBUG("buildHtmlPage: after {script}");
        REPLACE_P(page, F("%{style}%"), pStyle);
        LOGDEBUG("buildHtmlPage: after {style}");
        */
    }

    void responseTextHtml(AsyncWebServerRequest *pRequest, const String& page)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        pRequest->send(200, WM_HTTP_HEAD_CT, page);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_CT, page);
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
        pRequest->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");
        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }

    void responseTextHtml(AsyncWebServerRequest *pRequest, const MemoryBlock *pBlocks,
        unsigned int countOfBlocks)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        pRequest->send(200, WM_HTTP_HEAD_CT, page);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        AsyncWebServerResponse *pResponse = pRequest->beginChunkedResponse(WM_HTTP_HEAD_CT,
            [pBlocks, countOfBlocks](uint8_t *pBuffer, size_t bufLen, size_t index) -> size_t
        {
            return SendMemoryBlock(pBlocks, countOfBlocks, pBuffer, bufLen, index);
        });
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
        pRequest->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");
        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }

    void responseTextHtml(AsyncWebServerRequest *pRequest, const MemoryBlock2 *pBlocks,
        unsigned int countOfBlocks)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        pRequest->send(200, WM_HTTP_HEAD_CT, page);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        AsyncWebServerResponse *pResponse = pRequest->beginChunkedResponse(WM_HTTP_HEAD_CT,
            [pBlocks, countOfBlocks](uint8_t *pBuffer, size_t bufLen, size_t index) -> size_t
        {
            return SendMemoryBlock(pBlocks, countOfBlocks, pBuffer, bufLen, index);
        });
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        // For configuring CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
        pRequest->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");
        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }

    void responseApplJson(AsyncWebServerRequest *pRequest, const String& page)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        pRequest->send(200, WM_HTTP_HEAD_CT, page);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else

        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_JSON, page);
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        pResponse->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");
        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }
};
// PK
//////////////////////////////////////////

//////////////////////////////////////////

ESPAsync_WMParameter::ESPAsync_WMParameter(const char *custom)
{
    _WMParam_data._id = NULL;
    _WMParam_data._placeholder = NULL;
    _WMParam_data._length = 0;
    _WMParam_data._value = NULL;
    _WMParam_data._labelPlacement = WFM_LABEL_BEFORE;

    _customHTML = custom;
}

//////////////////////////////////////////

ESPAsync_WMParameter::ESPAsync_WMParameter(const char *id, const char *placeholder, const char *defaultValue,
                                           const int& length, const char *custom, const int& labelPlacement)
{
    init(id, placeholder, defaultValue, length, custom, labelPlacement);
}

//////////////////////////////////////////

// KH, using struct
ESPAsync_WMParameter::ESPAsync_WMParameter(const WMParam_Data& WMParam_data)
{
    init(WMParam_data._id, WMParam_data._placeholder, WMParam_data._value,
        WMParam_data._length, "", WMParam_data._labelPlacement);
}

//////////////////////////////////////////

void ESPAsync_WMParameter::init(const char *id, const char *placeholder, const char *defaultValue,
                                const int& length, const char *custom, const int& labelPlacement)
{
    _WMParam_data._id = id;
    _WMParam_data._placeholder = placeholder;
    _WMParam_data._length = length;
    _WMParam_data._labelPlacement = labelPlacement;

    _WMParam_data._value = new char[_WMParam_data._length + 1];

    if (_WMParam_data._value != NULL)
    {
        memset(_WMParam_data._value, 0, _WMParam_data._length + 1);

        if (defaultValue != NULL)
        {
        strncpy(_WMParam_data._value, defaultValue, _WMParam_data._length);
        }
    }

    _customHTML = custom;
}

//////////////////////////////////////////

ESPAsync_WMParameter::~ESPAsync_WMParameter()
{
    if (_WMParam_data._value != NULL)
    {
        delete[] _WMParam_data._value;
    }
}

//////////////////////////////////////////

// Using Struct to get/set whole data at once
void ESPAsync_WMParameter::setWMParam_Data(const WMParam_Data& WMParam_data)
{
    LOGINFO(F("setWMParam_Data"));

    memcpy(&_WMParam_data, &WMParam_data, sizeof(_WMParam_data));
}

//////////////////////////////////////////

void ESPAsync_WMParameter::getWMParam_Data(WMParam_Data& WMParam_data)
{
    LOGINFO(F("getWMParam_Data"));

    memcpy(&WMParam_data, &_WMParam_data, sizeof(WMParam_data));
}

//////////////////////////////////////////

const char* ESPAsync_WMParameter::getValue()
{
    return _WMParam_data._value;
}

//////////////////////////////////////////

const char* ESPAsync_WMParameter::getID()
{
    return _WMParam_data._id;
}

//////////////////////////////////////////

const char* ESPAsync_WMParameter::getPlaceholder()
{
    return _WMParam_data._placeholder;
}

//////////////////////////////////////////

int ESPAsync_WMParameter::getValueLength()
{
    return _WMParam_data._length;
}

//////////////////////////////////////////

int ESPAsync_WMParameter::getLabelPlacement()
{
    return _WMParam_data._labelPlacement;
}

//////////////////////////////////////////

const char* ESPAsync_WMParameter::getCustomHTML()
{
    return _customHTML;
}

//////////////////////////////////////////

/**
   [getParameters description]
   @access public
*/
ESPAsync_WMParameter** ESPAsync_WiFiManager::getParameters()
{
    return _params;
}

//////////////////////////////////////////
//////////////////////////////////////////

/**
   [getParametersCount description]
   @access public
*/
int ESPAsync_WiFiManager::getParametersCount()
{
    return _paramsCount;
}

//////////////////////////////////////////

char* ESPAsync_WiFiManager::getRFC952_hostname(const char* iHostname)
{
    memset(_RFC952_hostname, 0, sizeof(_RFC952_hostname));

    size_t len = (RFC952_HOSTNAME_MAXLEN < strlen(iHostname)) ? RFC952_HOSTNAME_MAXLEN : strlen(iHostname);

    size_t j = 0;

    for (size_t i = 0; i < len - 1; i++)
    {
        if (isalnum(iHostname[i]) || iHostname[i] == '-')
        {
            _RFC952_hostname[j] = iHostname[i];
            j++;
        }
    }

    // no '-' as last char
    if (isalnum(iHostname[len - 1]) || (iHostname[len - 1] != '-'))
        _RFC952_hostname[j] = iHostname[len - 1];

    return _RFC952_hostname;
}

//////////////////////////////////////////

ESPAsync_WiFiManager::ESPAsync_WiFiManager(AsyncWebServer * webserver, AsyncDNSServer *dnsserver, const char *iHostname)
{
    _server    = webserver;
    _dnsServer = dnsserver;

    init(iHostname);
 }

ESPAsync_WiFiManager::ESPAsync_WiFiManager(AsyncWebServer * server,
    const char * username, const char * password, const char *iHostname)
{
    _server = server;
    _dnsServer = nullptr;

    //setAuth(username, password);

#if defined(TARGET_RP2040)
    if (!__isPicoW) {
      ELEGANTOTA_DEBUG_MSG("RP2040: Not a Pico W, skipping OTA setup\n");
      return;
    }
#endif

    init(iHostname);
    handleSTA();
}

void ESPAsync_WiFiManager::init(const char *iHostname)
{
  #if USE_DYNAMIC_PARAMS
    _max_params = WIFI_MANAGER_MAX_PARAMS;
    _params = (ESPAsync_WMParameter**)malloc(_max_params * sizeof(ESPAsync_WMParameter*));
  #endif

    //WiFi not yet started here, must call WiFi.mode(WIFI_STA) and modify function WiFiGenericClass::mode(wifi_mode_t m) !!!
    WiFi.mode(WIFI_STA);

    if (iHostname[0] == 0)
    {
    #ifdef ESP8266
        String _hostname = "ESP8266-" + String(ESP.getChipId(), HEX);
    #else    //ESP32
        String _hostname = "ESP32-" + String(ESP_getChipId(), HEX);
    #endif

        _hostname.toUpperCase();

        getRFC952_hostname(_hostname.c_str());
    }
    else
    {
        // Prepare and store the hostname only not NULL
        getRFC952_hostname(iHostname);
    }

    LOGWARN1(F("RFC925 Hostname ="), _RFC952_hostname);

    setHostname();
}


//////////////////////////////////////////

ESPAsync_WiFiManager::~ESPAsync_WiFiManager()
{
#if USE_DYNAMIC_PARAMS

    if (_params != NULL)
    {
        LOGINFO(F("freeing allocated params!"));

        free(_params);
    }

#endif

    if (_networkIndices)
    {
        free(_networkIndices); //indices array no longer required so free memory
    }
}

//////////////////////////////////////////

#if USE_DYNAMIC_PARAMS
  bool ESPAsync_WiFiManager::addParameter(ESPAsync_WMParameter *p)
#else
  void ESPAsync_WiFiManager::addParameter(ESPAsync_WMParameter *p)
#endif
{
#if USE_DYNAMIC_PARAMS

    if (_paramsCount == _max_params)
    {
        // rezise the params array
        _max_params += WIFI_MANAGER_MAX_PARAMS;

        LOGINFO1(F("Increasing _max_params to:"), _max_params);

        ESPAsync_WMParameter** new_params = (ESPAsync_WMParameter**)realloc(_params,
                                                                            _max_params * sizeof(ESPAsync_WMParameter*));

        if (new_params != NULL)
        {
            _params = new_params;
        }
        else
        {
            LOGINFO(F("ERROR: failed to realloc params, size not increased!"));

            return false;
        }
    }

    _params[_paramsCount] = p;
    _paramsCount++;

    LOGINFO1(F("Adding parameter"), p->getID());

    return true;

#else

    // Danger here. Better to use Tzapu way here
    if (_paramsCount < (WIFI_MANAGER_MAX_PARAMS))
    {
        _params[_paramsCount] = p;
        _paramsCount++;

        LOGINFO1(F("Adding parameter"), p->getID());
    }
    else
    {
        LOGINFO("Can't add parameter. Full");
    }

#endif
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setupConfigPortal()
{
    _stopConfigPortal = false; //Signal not to close config portal

    /*This library assumes autoconnect is set to 1. It usually is
      but just in case check the setting and turn on autoconnect if it is off.
      Some useful discussion at https://github.com/esp8266/Arduino/issues/1615*/
    if (WiFi.getAutoConnect() == 0)
        WiFi.setAutoConnect(1);

#if !( USING_ESP32_S2 || USING_ESP32_C3 )
#ifdef ESP8266
    // KH, mod for Async
    _server->reset();
#else   //ESP32
    _server->reset();
#endif

    if (!_dnsServer)
        _dnsServer = new AsyncDNSServer;

#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

    // optional soft ip config
    // Must be put here before dns _server start to take care of the non-default ConfigPortal AP IP.
    // Check (https://github.com/khoih-prog/ESP_WiFiManager/issues/58)
    if (_WiFi_AP_IPconfig._ap_static_ip)
    {
        LOGWARN3(F("Custom AP IP/GW/Subnet = "), _WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw,
                _WiFi_AP_IPconfig._ap_static_sn);

        WiFi.softAPConfig(_WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw, _WiFi_AP_IPconfig._ap_static_sn);
    }

    /* Setup the DNS _server redirecting all the domains to the apIP */
    if (_dnsServer)
    {
        _dnsServer->setErrorReplyCode(AsyncDNSReplyCode::NoError);

        // AsyncDNSServer started with "*" domain name, all DNS requests will be passsed to WiFi.softAPIP()
        if (! _dnsServer->start(_DNS_PORT, "*", WiFi.softAPIP()))
        {
            // No socket available
            LOGERROR(F("Can't start DNS Server. No available socket"));
        }
    }

    _configPortalStart = millis();

    LOGWARN1(F("\nConfiguring AP SSID ="), _apName);

    if (_apPassword != NULL)
    {
        if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63)
        {
            // fail passphrase to short or long!
            LOGERROR(F("Invalid AccessPoint password. Ignoring"));

            _apPassword = NULL;
        }

        LOGWARN1(F("AP PWD ="), _apPassword);
    }

    // KH, To enable dynamic/random channel
    static int channel;

    // Use random channel if  _WiFiAPChannel == 0
    if (_WiFiAPChannel == 0)
        channel = (_configPortalStart % MAX_WIFI_CHANNEL) + 1;
    else
        channel = _WiFiAPChannel;

    LOGWARN1(F("AP Channel ="), channel);

    WiFi.softAP(_apName, _apPassword, channel);

    delay(500); // Without delay I've seen the IP address blank

    LOGWARN1(F("AP IP address ="), WiFi.softAPIP());

    /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */

    attacheHandlers(ON_AP_FILTER);

    _server->begin(); // Web _server start

    LOGWARN(F("HTTP _server started"));
}

//////////////////////////////////////////

bool ESPAsync_WiFiManager::autoConnect()
{
#ifdef ESP8266
  String ssid = "ESP_" + String(ESP.getChipId());
#else   //ESP32
  String ssid = "ESP_" + String(ESP_getChipId());
#endif

  return autoConnect(ssid.c_str(), NULL);
}

//////////////////////////////////////////

/* This is not very useful as there has been an assumption that device has to be
  told to _connect but Wifi already does it's best to _connect in background. Calling this
  method will block until WiFi connects. Sketch can avoid
  blocking call then use (WiFi.status()==WL_CONNECTED) test to see if connected yet.
  See some discussion at https://github.com/tzapu/WiFiManager/issues/68
*/

// To permit autoConnect() to use STA static IP or DHCP IP.
#ifndef AUTOCONNECT_NO_INVALIDATE
  #define AUTOCONNECT_NO_INVALIDATE true
#endif

//////////////////////////////////////////

bool ESPAsync_WiFiManager::autoConnect(char const *apName, char const *apPassword)
{
#if AUTOCONNECT_NO_INVALIDATE
    LOGINFO(F("\nAutoConnect using previously saved SSID/PW, but keep previous settings"));
    // Connect to previously saved SSID/PW, but keep previous settings
    connectWifi();
#else
    LOGINFO(F("\nAutoConnect using previously saved SSID/PW, but invalidate previous settings"));
    // Connect to previously saved SSID/PW, but invalidate previous settings
    connectWifi(WiFi_SSID(), WiFi_Pass());
#endif

    unsigned long startedAt = millis();

    while (millis() - startedAt < 10000)
    {
        //delay(100);
        delay(200);

        if (WiFi.status() == WL_CONNECTED)
        {
        float waited = (millis() - startedAt);

        LOGWARN1(F("Connected after waiting (s) :"), waited / 1000);
        LOGWARN1(F("Local ip ="), WiFi.localIP());

        return true;
        }
    }

    return startConfigPortal(apName, apPassword);
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::handleSTA()
{
    _isSTAmode = true;
    attacheHandlers(ON_STA_FILTER);
}

void ESPAsync_WiFiManager::attacheHandlers(ArRequestFilterFunction filterFn)
{
    using namespace std::placeholders;  // for _1, _2, _3..

    _server->on("/", std::bind(&ESPAsync_WiFiManager::handleRoot, this, _1)).setFilter(filterFn);
    _server->on("/settings", std::bind(&ESPAsync_WiFiManager::handleSettings, this, _1)).setFilter(filterFn);
    _server->on("/settingssave", std::bind(&ESPAsync_WiFiManager::handleSettingsSave, this, _1)).setFilter(filterFn);
    _server->on("/close",   std::bind(&ESPAsync_WiFiManager::handleServerClose, this, _1)).setFilter(filterFn);
    _server->on("/i",       std::bind(&ESPAsync_WiFiManager::handleInfo, this, _1)).setFilter(filterFn);
    _server->on("/r",       std::bind(&ESPAsync_WiFiManager::handleReset, this, _1)).setFilter(filterFn);
    _server->on("/state",   std::bind(&ESPAsync_WiFiManager::handleState, this, _1)).setFilter(filterFn);
    _server->on("/scan",    std::bind(&ESPAsync_WiFiManager::handleScan, this, _1)).setFilter(filterFn);
    // OTA update handlers
    _server->on("/update",  std::bind(&ESPAsync_WiFiManager::handleOTAUpdate, this, _1)).setFilter(filterFn);
    _server->on("/ota/start", std::bind(&ESPAsync_WiFiManager::handleOTAUpdateStart, this, _1)).setFilter(filterFn);
    _server->on("/ota/upload", HTTP_POST, 
        std::bind(&ESPAsync_WiFiManager::handleOTAUpdateUpload, this, _1),
        std::bind(&ESPAsync_WiFiManager::handleOTAUpdateUploadFile, this, _1, _2, _3, _4, _5, _6));
                                      

    //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
    _server->on("/fwlink",   std::bind(&ESPAsync_WiFiManager::handleRoot, this, _1)).setFilter(filterFn);

#if WM_SUPPORT_TRAIN_CONTROL
    _server->on("/tcontrol",   std::bind(&ESPAsync_WiFiManager::handleTrainControl, this, _1)).setFilter(filterFn);
#endif

    _server->onNotFound (std::bind(&ESPAsync_WiFiManager::handleNotFound, this, _1));
}


//////////////////////////////////////////

String ESPAsync_WiFiManager::networkListAsString()
{
    String pager;

    //display networks in page
    for (int i = 0; i < _wifiSSIDCount; i++)
    {
        if (_wifiSSIDs[i].duplicate == true)
            continue; // skip dups

        int quality = getRSSIasQuality(_wifiSSIDs[i].RSSI);

        if (_minimumQuality == -1 || _minimumQuality < quality)
        {
            String item = FPSTR(WM_PK_HTTP_ITEM);
            String rssiQ;

            rssiQ += quality;
            item.replace("%{v}%", _wifiSSIDs[i].SSID);
            item.replace("%{r}%", rssiQ);

        #if defined(ESP8266)
            if (_wifiSSIDs[i].encryptionType != ENC_TYPE_NONE)
        #else
            if (_wifiSSIDs[i].encryptionType != WIFI_AUTH_OPEN)
        #endif
            {
                item.replace("%{i}%", "l");
            }
            else
            {
                item.replace("%{i}%", "");
            }

            pager += item;
        }
        else
        {
            LOGDEBUG(F("Skipping due to quality"));
        }
    }

    return pager;
}

//////////////////////////////////////////

String ESPAsync_WiFiManager::scanModal()
{
    _shouldscan = true;
    scan();

    String _pager = networkListAsString();

    return _pager;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::scan()
{
    if (!_shouldscan)
        return;

    LOGDEBUG(F("About to scan"));

    if (_wifiSSIDscan)
    {
        delay(100);
    }

    if (_wifiSSIDscan)
    {
        LOGDEBUG(F("Start scan"));

        wifi_ssid_count_t n = WiFi.scanNetworks(false, true);

        LOGDEBUG(F("Scan done"));

        if (n == WIFI_SCAN_FAILED)
        {
           LOGDEBUG(F("WIFI_SCAN_FAILED!"));
        }
        else if (n == WIFI_SCAN_RUNNING)
        {
           LOGDEBUG(F("WIFI_SCAN_RUNNING!"));
        }
        else if (n < 0)
        {
            LOGDEBUG(F("Failed, unknown error code!"));
        }
        else if (n == 0)
        {
            LOGDEBUG(F("No network found"));
            // page += F("No networks found. Refresh to scan again.");
        }
        else
        {
            if (_wifiSSIDscan)
            {
                /* WE SHOULD MOVE THIS IN PLACE ATOMICALLY */
                if (_wifiSSIDs)
                    delete [] _wifiSSIDs;

                _wifiSSIDs     = new WiFiResult[n];
                _wifiSSIDCount = n;

                if (n > 0)
                    _shouldscan = false;

                for (wifi_ssid_count_t i = 0; i < n; i++)
                {
                    _wifiSSIDs[i].duplicate = false;

                #if defined(ESP8266)
                    WiFi.getNetworkInfo(i, _wifiSSIDs[i].SSID, _wifiSSIDs[i].encryptionType, _wifiSSIDs[i].RSSI, _wifiSSIDs[i].BSSID,
                                        _wifiSSIDs[i].channel, _wifiSSIDs[i].isHidden);
                #else
                    WiFi.getNetworkInfo(i, _wifiSSIDs[i].SSID, _wifiSSIDs[i].encryptionType, _wifiSSIDs[i].RSSI, _wifiSSIDs[i].BSSID,
                                        _wifiSSIDs[i].channel);
                #endif
                }

                // RSSI SORT
                // old sort
                for (int i = 0; i < n; i++)
                {
                    for (int j = i + 1; j < n; j++)
                    {
                        if (_wifiSSIDs[j].RSSI > _wifiSSIDs[i].RSSI)
                        {
                          std::swap(_wifiSSIDs[i], _wifiSSIDs[j]);
                        }
                    }
                }

                // remove duplicates ( must be RSSI sorted )
                if (_removeDuplicateAPs)
                {
                    String cssid;

                    for (int i = 0; i < n; i++)
                    {
                        if (_wifiSSIDs[i].duplicate == true)
                            continue;

                        cssid = _wifiSSIDs[i].SSID;

                        for (int j = i + 1; j < n; j++)
                        {
                            if (cssid == _wifiSSIDs[j].SSID)
                            {
                                LOGDEBUG("DUP AP: " + _wifiSSIDs[j].SSID);
                                // set dup aps to NULL
                                _wifiSSIDs[j].duplicate = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::startConfigPortalModeless(char const *apName, char const *apPassword, bool shouldConnectWiFi)
{
    _modeless     = true;
    _apName       = apName;
    _apPassword   = apPassword;

    WiFi.mode(WIFI_AP_STA);

    LOGDEBUG("SET AP STA");

    // try to _connect
    if (shouldConnectWiFi && connectWifi("", "") == WL_CONNECTED)
    {
        LOGDEBUG1(F("IP Address:"), WiFi.localIP());

        if ( _savecallback != NULL)
        {
            //todo: check if any custom parameters actually exist, and check if they really changed maybe
            _savecallback();
        }
    }

    if ( _apcallback != NULL)
    {
        _apcallback(this);
    }

    _connect = false;
    setupConfigPortal();
    _scannow = -1 ;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::loop()
{
    safeLoop();
    criticalLoop();
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setInfo()
{
    if (_needInfo)
    {
        _pager       = infoAsString();
        _wifiStatus  = WiFi.status();
        _needInfo    = false;
    }
}

//////////////////////////////////////////

// Anything that accesses WiFi, ESP or EEPROM goes here

void ESPAsync_WiFiManager::criticalLoop()
{
    //LOGDEBUG(F("criticalLoop: Enter"));

    if (_modeless)
    {
        if (_scannow == -1 || ( millis() > _scannow + TIME_BETWEEN_MODELESS_SCANS) )
        {
            LOGDEBUG(F("criticalLoop: modeless scan"));

            scan();
            _scannow = millis();
        }

        if (_connect)
        {
            _connect = false;

            LOGDEBUG(F("criticalLoop: Connecting to new AP"));

            // using user-provided  _ssid, _pass in place of system-stored ssid and pass
            if (connectWifi(_ssid, _pass) != WL_CONNECTED)
            {
                LOGDEBUG(F("criticalLoop: Failed to _connect."));
            }
            else
            {
                //connected
                // alanswx - should we have a config to decide if we should shut down AP?
                // WiFi.mode(WIFI_STA);
                //notify that configuration has changed and any optional parameters should be saved
                if ( _savecallback != NULL)
                {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }

                return;
            }

            if (_shouldBreakAfterConfig)
            {
                //flag set to exit after config after trying to _connect
                //notify that configuration has changed and any optional parameters should be saved
                if ( _savecallback != NULL)
                {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }
            }
        }
    }

    // Check if 2 seconds have passed since _reboot_request_millis was set
    if (_reboot && millis() - _reboot_request_millis > 2000)
    {
        LOGDEBUG("Rebooting...\n");
        #if defined(ESP8266) || defined(ESP32)
            ESP.restart();
        #elif defined(TARGET_RP2040)
            rp2040.reboot();
        #endif
        _reboot = false;
    }

}

//////////////////////////////////////////

// Anything that doesn't access WiFi, ESP or EEPROM can go here

void ESPAsync_WiFiManager::safeLoop()
{
}

///////////////////////////////////////////////////////////

bool ESPAsync_WiFiManager::startConfigPortal()
{
#ifdef ESP8266
    String ssid = "ESP_" + String(ESP.getChipId());
#else   //ESP32
    String ssid = "ESP_" + String(ESP_getChipId());
#endif

    ssid.toUpperCase();

    return startConfigPortal(ssid.c_str(), NULL);
}

//////////////////////////////////////////

bool ESPAsync_WiFiManager::startConfigPortal(char const *apName, char const *apPassword)
{
    WiFi.mode(WIFI_AP_STA);

    _apName = apName;
    _apPassword = apPassword;

    //notify we entered AP mode
    if (_apcallback != NULL)
    {
        LOGINFO("_apcallback");

        _apcallback(this);
    }

    _connect = false;

    setupConfigPortal();

    bool TimedOut = true;

    LOGINFO("startConfigPortal : Enter loop");

    _scannow = -1 ;

    while (_configPortalTimeout == 0 || ( millis() < _configPortalStart + _configPortalTimeout) )
      {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else

        //
        //  we should do a scan every so often here and
        //  try to reconnect to AP while we are at it
        //
        if ( _scannow == -1 || ( millis() > _scannow + TIME_BETWEEN_MODAL_SCANS) )
        {
          LOGDEBUG(F("startConfigPortal: About to modal scan"));

          // since we are modal, we can scan every time
          _shouldscan = true;

    #if defined(ESP8266)
          // we might still be connecting, so that has to stop for scanning
          ETS_UART_INTR_DISABLE ();
          wifi_station_disconnect ();
          ETS_UART_INTR_ENABLE ();
    #else
          WiFi.disconnect (false);
    #endif

          scan();

          //if (_tryConnectDuringConfigPortal)
          //  WiFi.begin(); // try to reconnect to AP

          _scannow = millis() ;
        }

    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

        // yield before processing our flags "_connect" and/or "_stopConfigPortal"
        yield();

        if (_connect)
        {
            TimedOut = false;
            delay(2000);

            LOGERROR(F("Connecting to new AP"));

            // using user-provided  _ssid, _pass in place of system-stored ssid and pass
            if (connectWifi(_ssid, _pass) != WL_CONNECTED)
            {
                LOGERROR(F("Failed to _connect"));

                WiFi.mode(WIFI_AP); // Dual mode becomes flaky if not connected to a WiFi network.
            }
            else
            {
                //notify that configuration has changed and any optional parameters should be saved
                if (_savecallback != NULL)
                {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }

                break;
            }

            if (_shouldBreakAfterConfig)
            {
                //flag set to exit after config after trying to _connect
                //notify that configuration has changed and any optional parameters should be saved
                if (_savecallback != NULL)
                {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }

                break;
            }
        }

        if (_stopConfigPortal)
        {
            TimedOut = false;

            LOGERROR("Stop ConfigPortal");

            _stopConfigPortal = false;
            break;
        }

    #if ( defined(TIME_BETWEEN_CONFIG_PORTAL_LOOP) && (TIME_BETWEEN_CONFIG_PORTAL_LOOP > 0) )
    #if (_ESPASYNC_WIFIMGR_LOGLEVEL_ > 3)
    #warning Using delay in startConfigPortal loop
    #endif

        delay(TIME_BETWEEN_CONFIG_PORTAL_LOOP);
    #endif
    }

    WiFi.mode(WIFI_STA);

    if (TimedOut)
    {
        setHostname();

        // New v1.0.8 to fix static IP when CP not entered or timed-out
        setWifiStaticIP();

        WiFi.begin();
        int connRes = waitForConnectResult();

        LOGERROR1("Timed out connection result:", getStatus(connRes));
    }

  #if !( USING_ESP32_S2 || USING_ESP32_C3 )
      _server->reset();
      _dnsServer->stop();
  #endif

      return  (WiFi.status() == WL_CONNECTED);
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setWifiStaticIP()
{
#if USE_CONFIGURABLE_DNS

    if (_WiFi_STA_IPconfig._sta_static_ip)
    {
        LOGWARN(F("Custom STA IP/GW/Subnet"));

        //***** Added section for DNS config option *****
        if (_WiFi_STA_IPconfig._sta_static_dns1 && _WiFi_STA_IPconfig._sta_static_dns2)
        {
            LOGWARN(F("DNS1 and DNS2 set"));

            WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn,
                        _WiFi_STA_IPconfig._sta_static_dns1, _WiFi_STA_IPconfig._sta_static_dns2);
        }
        else if (_WiFi_STA_IPconfig._sta_static_dns1)
        {
            LOGWARN(F("Only DNS1 set"));

            WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn,
                        _WiFi_STA_IPconfig._sta_static_dns1);
        }
        else
        {
            LOGWARN(F("No DNS _server set"));

            WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn);
        }

        //***** End added section for DNS config option *****

        LOGINFO1(F("setWifiStaticIP IP ="), WiFi.localIP());
    }
    else
    {
      LOGWARN(F("Can't use Custom STA IP/GW/Subnet"));
    }

#else

    // check if we've got static_ip settings, if we do, use those.
    if (_WiFi_STA_IPconfig._sta_static_ip)
    {
        WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn);

        LOGWARN1(F("Custom STA IP/GW/Subnet : "), WiFi.localIP());
    }

#endif
}

//////////////////////////////////////////

int ESPAsync_WiFiManager::reconnectWifi()
{
    int connectResult;

    // using user-provided  _ssid, _pass in place of system-stored ssid and pass
    if ( ( connectResult = connectWifi(_ssid, _pass) ) != WL_CONNECTED)
    {
        LOGERROR1(F("Failed to _connect to"), _ssid);

        if ( ( connectResult = connectWifi(_ssid1, _pass1) ) != WL_CONNECTED)
        {
            LOGERROR1(F("Failed to _connect to"), _ssid1);
        }
        else
        {
            LOGERROR1(F("Connected to"), _ssid1);
        }
    }
    else
    {
        LOGERROR1(F("Connected to"), _ssid);
    }

    return connectResult;
}

//////////////////////////////////////////

int ESPAsync_WiFiManager::connectWifi(const String& ssid, const String& pass)
{
    // Add option if didn't input/update SSID/PW => Use the previous saved Credentials.
    // But update the Static/DHCP options if changed.
    if ( (ssid != "") || ( (ssid == "") && (WiFi_SSID() != "") ) )
    {
        //fix for auto _connect racing issue to avoid resetSettings()
        if (WiFi.status() == WL_CONNECTED)
        {
            LOGWARN(F("Already connected. Bailing out."));

            return WL_CONNECTED;
        }

        if (ssid != "")
            resetSettings();

      #ifdef ESP8266
          setWifiStaticIP();
      #endif

          WiFi.mode(WIFI_AP_STA); //It will start in station mode if it was previously in AP mode.

          setHostname();

          // KH, Fix ESP32 staticIP after exiting CP
      #ifdef ESP32
          setWifiStaticIP();
      #endif

        if (ssid != "")
        {
            // Start Wifi with new values.
            LOGWARN(F("Connect to new WiFi using new IP parameters"));

            WiFi.begin(ssid.c_str(), pass.c_str());
        }
        else
        {
            // Start Wifi with old values.
            LOGWARN(F("Connect to previous WiFi using new IP parameters"));

            WiFi.begin();
        }
    }
    else if (WiFi_SSID() == "")
    {
        LOGWARN(F("No saved credentials"));
    }

    int connRes = waitForConnectResult();

    LOGWARN1("Connection result: ", getStatus(connRes));

    //not connected, WPS enabled, no pass - first attempt
    if (_tryWPS && connRes != WL_CONNECTED && pass == "")
    {
        startWPS();
        //should be connected at the end of WPS
        connRes = waitForConnectResult();
    }

    return connRes;
}

//////////////////////////////////////////

wl_status_t ESPAsync_WiFiManager::waitForConnectResult()
{
    if (_connectTimeout == 0)
    {
        unsigned long startedAt = millis();

        // In ESP8266, WiFi.waitForConnectResult() @return wl_status_t (0-255) or -1 on timeout !!!
        // In ESP32, WiFi.waitForConnectResult() @return wl_status_t (0-255)
        // So, using int for connRes to be safe
        //int connRes = WiFi.waitForConnectResult();
        WiFi.waitForConnectResult();

        float waited = (millis() - startedAt);

        LOGWARN1(F("Connected after waiting (s) :"), waited / 1000);
        LOGWARN1(F("Local ip ="), WiFi.localIP());

        // Fix bug, connRes is sometimes not correct.
        //return connRes;
        return WiFi.status();
    }
    else
    {
        LOGERROR(F("Waiting WiFi connection with time out"));

        unsigned long start = millis();
        bool keepConnecting = true;
        wl_status_t status;

        while (keepConnecting)
        {
            status = WiFi.status();

            if (millis() > start + _connectTimeout)
            {
                keepConnecting = false;
                LOGERROR(F("Connection timed out"));
            }

      #if ( ESP8266 && (USING_ESP8266_CORE_VERSION >= 30000) )

            if (status == WL_CONNECTED || status == WL_CONNECT_FAILED || status == WL_WRONG_PASSWORD)
      #else
            if (status == WL_CONNECTED || status == WL_CONNECT_FAILED)
      #endif
            {
                keepConnecting = false;
            }

            delay(100);
        }

        return status;
    }
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::startWPS()
{
#ifdef ESP8266
    LOGINFO("START WPS");
    WiFi.beginWPSConfig();
    LOGINFO("END WPS");
#else   //ESP32
    // TODO
    LOGINFO("ESP32 WPS TODO");
#endif
}

//////////////////////////////////////////

//Convenient for debugging but wasteful of program space.
//Remove if short of space
const char* ESPAsync_WiFiManager::getStatus(const int& status)
{
    switch (status)
    {
        case WL_IDLE_STATUS:
            return "WL_IDLE_STATUS";

        case WL_NO_SSID_AVAIL:
            return "WL_NO_SSID_AVAIL";

        case WL_CONNECTED:
            return "WL_CONNECTED";

        case WL_CONNECT_FAILED:
            return "WL_CONNECT_FAILED";

    #if ( ESP8266 && (USING_ESP8266_CORE_VERSION >= 30000) )

        case WL_WRONG_PASSWORD:
            return "WL_WRONG_PASSWORD";
    #endif

        case WL_DISCONNECTED:
            return "WL_DISCONNECTED";

        default:
            return "UNKNOWN";
    }
}

//////////////////////////////////////////

String ESPAsync_WiFiManager::getConfigPortalSSID()
{
    return _apName;
}

//////////////////////////////////////////

String ESPAsync_WiFiManager::getConfigPortalPW()
{
    return _apPassword;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::resetSettings()
{
    LOGINFO(F("Previous settings invalidated"));

#ifdef ESP8266
    WiFi.disconnect(true);
#else
    WiFi.disconnect(true, true);

    // Temporary fix for issue of not clearing WiFi SSID/PW from flash of ESP32
    // See https://github.com/khoih-prog/ESPAsync_WiFiManager/issues/25 and https://github.com/espressif/arduino-esp32/issues/400
    WiFi.begin("0", "0");
#endif

    delay(200);

    return;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setTimeout(const unsigned long& seconds)
{
    setConfigPortalTimeout(seconds);
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setConfigPortalTimeout(const unsigned long& seconds)
{
    _configPortalTimeout = seconds * 1000;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setConnectTimeout(const unsigned long& seconds)
{
    _connectTimeout = seconds * 1000;
}

void ESPAsync_WiFiManager::setDebugOutput(bool debug)
{
    _debug = debug;
}

//////////////////////////////////////////

// KH, To enable dynamic/random channel
int ESPAsync_WiFiManager::setConfigPortalChannel(const int& channel)
{
    // If channel < MIN_WIFI_CHANNEL - 1 or channel > MAX_WIFI_CHANNEL => channel = 1
    // If channel == 0 => will use random channel from MIN_WIFI_CHANNEL to MAX_WIFI_CHANNEL
    // If (MIN_WIFI_CHANNEL <= channel <= MAX_WIFI_CHANNEL) => use it
    if ( (channel < MIN_WIFI_CHANNEL - 1) || (channel > MAX_WIFI_CHANNEL) )
        _WiFiAPChannel = 1;
    else if ( (channel >= MIN_WIFI_CHANNEL - 1) && (channel <= MAX_WIFI_CHANNEL) )
        _WiFiAPChannel = channel;

    return _WiFiAPChannel;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setAPStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
    LOGINFO(F("setAPStaticIPConfig"));

    _WiFi_AP_IPconfig._ap_static_ip = ip;
    _WiFi_AP_IPconfig._ap_static_gw = gw;
    _WiFi_AP_IPconfig._ap_static_sn = sn;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setAPStaticIPConfig(const WiFi_AP_IPConfig& WM_AP_IPconfig)
{
    LOGINFO(F("setAPStaticIPConfig"));

    memcpy((void *) &_WiFi_AP_IPconfig, &WM_AP_IPconfig, sizeof(_WiFi_AP_IPconfig));
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::getAPStaticIPConfig(WiFi_AP_IPConfig  &WM_AP_IPconfig)
{
    LOGINFO(F("getAPStaticIPConfig"));

    memcpy((void *) &WM_AP_IPconfig, &_WiFi_AP_IPconfig, sizeof(WM_AP_IPconfig));
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
    LOGINFO(F("setSTAStaticIPConfig"));

    _WiFi_STA_IPconfig._sta_static_ip = ip;
    _WiFi_STA_IPconfig._sta_static_gw = gw;
    _WiFi_STA_IPconfig._sta_static_sn = sn;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setSTAStaticIPConfig(const WiFi_STA_IPConfig& WM_STA_IPconfig)
{
    LOGINFO(F("setSTAStaticIPConfig"));

    memcpy((void *) &_WiFi_STA_IPconfig, &WM_STA_IPconfig, sizeof(_WiFi_STA_IPconfig));
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::getSTAStaticIPConfig(WiFi_STA_IPConfig& WM_STA_IPconfig)
{
    LOGINFO(F("getSTAStaticIPConfig"));

    memcpy((void *) &WM_STA_IPconfig, &_WiFi_STA_IPconfig, sizeof(WM_STA_IPconfig));
}


//////////////////////////////////////////

#if USE_CONFIGURABLE_DNS
void ESPAsync_WiFiManager::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn,
                                                const IPAddress& dns_address_1, const IPAddress& dns_address_2)
{
    LOGINFO(F("setSTAStaticIPConfig for USE_CONFIGURABLE_DNS"));

    _WiFi_STA_IPconfig._sta_static_ip = ip;
    _WiFi_STA_IPconfig._sta_static_gw = gw;
    _WiFi_STA_IPconfig._sta_static_sn = sn;
    _WiFi_STA_IPconfig._sta_static_dns1 = dns_address_1; //***** Added argument *****
    _WiFi_STA_IPconfig._sta_static_dns2 = dns_address_2; //***** Added argument *****
}
#endif

//////////////////////////////////////////

void ESPAsync_WiFiManager::setMinimumSignalQuality(const int& quality)
{
    _minimumQuality = quality;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::setBreakAfterConfig(bool shouldBreak)
{
    _shouldBreakAfterConfig = shouldBreak;
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::reportStatus(String& page)
{
    page += FPSTR(WM_HTTP_SCRIPT_NTP_MSG);

    if (WiFi_SSID() != "")
    {
        page += F("Configured to connect to AP <b>");
        page += WiFi_SSID();

        if (WiFi.status() == WL_CONNECTED)
        {
            page += F(" and connected</b> on IP <a href=\"http://");
            page += WiFi.localIP().toString();
            page += F("/\">");
            page += WiFi.localIP().toString();
            page += F("</a>");
        }
        else
        {
            page += F(" but not connected.</b>");
        }
    }
    else
    {
        page += F("No network configured.");
    }
}

//////////////////////////////////////////

// Handle root or redirect to captive portal
/*
void ESPAsync_WiFiManager::handleRoot(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Handle root"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;

    if (captivePortal(request))
    {
        // If captive portal redirect instead of displaying the error page.
        return;
    }

    String page = FPSTR(WM_HTTP_HEAD_START);
    String headTitle = _apName;
    if (WiFi_SSID() != "")
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            headTitle += " on ";
            headTitle += WiFi_SSID();
        }
        else
        {
            headTitle += " <s>on ";
            headTitle += WiFi_SSID();
            headTitle += "</s>";
        }
    }

    String body(FPSTR(WM_PK_HTTP_FLDSET_START));
    body += FPSTR(WM_PK_HTTP_PORTAL_OPTIONS);
    String configurableOptions;
#if WM_SUPPORT_TRAIN_CONTROL
    configurableOptions += FPSTR(WM_PK_HTTP_TRAIN_CONTROL_OPTION);
#endif
    body.replace("%{customCtrl}%", configurableOptions);
    body += F("<div class=\"msg\">");

    reportStatus(body);

    body += F("</div>");
    body += FPSTR(WM_PK_HTTP_FLDSET_END);

    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_OPTIONS_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        headTitle,
        body,
        FPSTR(WM_HTTP_SCRIPT_NTP));

    responseTextHtml(request, page);
}
*/

//////////////////////////////////////////

void ESPAsync_WiFiManager::handleRoot(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Handle root"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;

    if (captivePortal(request))
    {
        // If captive portal redirect instead of displaying the error page.
        return;
    }

    String page = FPSTR(WM_HTTP_HEAD_START);
    String headTitle = _apName;
    if (WiFi_SSID() != "")
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            headTitle += " on ";
            headTitle += WiFi_SSID();
        }
        else
        {
            headTitle += " <s>on ";
            headTitle += WiFi_SSID();
            headTitle += "</s>";
        }
    }

    String body(FPSTR(WM_PK_HTTP_FLDSET_START));
    body += FPSTR(WM_PK_HTTP_PORTAL_OPTIONS);
    String configurableOptions;
#if WM_SUPPORT_TRAIN_CONTROL
    configurableOptions += FPSTR(WM_PK_HTTP_TRAIN_CONTROL_OPTION);
#endif
    body.replace("%{customCtrl}%", configurableOptions);
    body += F("<div class=\"msg\">");

    reportStatus(body);

    body += F("</div>");
    body += FPSTR(WM_PK_HTTP_FLDSET_END);

    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_OPTIONS_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        headTitle,
        body,
        FPSTR(WM_HTTP_SCRIPT_NTP));

    responseTextHtml(request, page);
}

//////////////////////////////////////////


// Wifi config page handler
void ESPAsync_WiFiManager::handleSettings(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Handle Settings"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;

    String body;

#if !( USING_ESP32_S2 || USING_ESP32_C3 )

    _wifiSSIDscan = false;
    LOGDEBUG(F("handleWifi: Scan done"));

    if (_wifiSSIDCount == 0)
    {
        LOGDEBUG(F("handleWifi: No network found"));

        body += F("No network found. Refresh to scan again.");
    }
    else
    {
        body += FPSTR(WM_PK_HTTP_FLDSET_START);

        //display networks in page
        body += networkListAsString();

        body += FPSTR(WM_PK_HTTP_FLDSET_END);
        body += "<br/>";
    }

    _wifiSSIDscan = true;

#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

    body += "<small>*Hint: To reuse the saved WiFi credentials, leave SSID and PWD fields empty</small>";

    body += FPSTR(WM_PK_HTTP_FORM_SETTINGS_START);
    body.replace("%{action}%", "wifisave");

#if DISPLAY_STORED_CREDENTIALS_IN_CP
    // Populate SSIDs and PWDs if valid
    body.replace("%{ssid}%", _ssid);
    body.replace("%{pwd}%",  _pass);
    body.replace("%{ssid1}%", _ssid1);
    body.replace("%{pwd1}%", _pass1);
#endif

#if WM_SUPPORT_MQTT
    body += FPSTR(WM_PK_HTTP_FORM_MQTT_SETTINGS);

#if DISPLAY_STORED_CREDENTIALS_IN_CP
    // Populate SSIDs and PWDs if valid
    body.replace("%{mqttserv}%", "");
    body.replace("%{mqttport}%", "");
    body.replace("%{mqttuser}%", "");
    body.replace("%{mqttpwd}%", "");
#endif

#endif


    char parLength[2];

    // add the extra parameters to the form
    for (int i = 0; i < _paramsCount; i++)
    {
        if (_params[i] == NULL)
        {
          break;
        }

        String pitem;

        switch (_params[i]->getLabelPlacement())
        {
          case WFM_LABEL_BEFORE:
            pitem = FPSTR(WM_PK_HTTP_FORM_LABEL_BEFORE);
            break;

          case WFM_LABEL_AFTER:
            pitem = FPSTR(WM_PK_HTTP_FORM_LABEL_AFTER);
            break;

          default:
            // WFM_NO_LABEL
            pitem = FPSTR(WM_PK_HTTP_FORM_PARAM);
            break;
        }

        if (_params[i]->getID() != NULL)
        {
          pitem.replace("%{i}%", _params[i]->getID());
          pitem.replace("%{n}%", _params[i]->getID());
          pitem.replace("%{p}%", _params[i]->getPlaceholder());

          snprintf(parLength, 2, "%d", _params[i]->getValueLength());

          pitem.replace("%{l}%", parLength);
          pitem.replace("%{v}%", _params[i]->getValue());
          pitem.replace("%{c}%", _params[i]->getCustomHTML());
        }
        else
        {
          pitem = _params[i]->getCustomHTML();
        }

        body += pitem;
    }

    if (_params[0] != NULL)
    {
        body += "<br/>";
    }

    LOGDEBUG1(F("Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());

    // KH, Comment out to permit changing from DHCP to static IP, or vice versa
    // and add staticIP label in CP

    // To permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
    // You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
    // You have to explicitly specify false to disable the feature.

    String staticIPInputs;

  #if !USE_STATIC_IP_CONFIG_IN_CP

    if (_WiFi_STA_IPconfig._sta_static_ip)
  #endif
    {
        String item = FPSTR(WM_PK_HTTP_FORM_LABEL);

        item += FPSTR(WM_PK_HTTP_FORM_PARAM);

        item.replace("%{i}%", "ip");
        item.replace("%{n}%", "ip");
        item.replace("%{p}%", "Static IP");
        item.replace("%{l}%", "15");
        item.replace("%{v}%", _WiFi_STA_IPconfig._sta_static_ip.toString());

        staticIPInputs = item;

        item = FPSTR(WM_PK_HTTP_FORM_LABEL);
        item += FPSTR(WM_PK_HTTP_FORM_PARAM);

        item.replace("%{i}%", "gw");
        item.replace("%{n}%", "gw");
        item.replace("%{p}%", "Gateway IP");
        item.replace("%{l}%", "15");
        item.replace("%{v}%", _WiFi_STA_IPconfig._sta_static_gw.toString());

        staticIPInputs += item;

        item = FPSTR(WM_PK_HTTP_FORM_LABEL);
        item += FPSTR(WM_PK_HTTP_FORM_PARAM);

        item.replace("%{i}%", "sn");
        item.replace("%{n}%", "sn");
        item.replace("%{p}%", "Subnet");
        item.replace("%{l}%", "15");
        item.replace("%{v}%", _WiFi_STA_IPconfig._sta_static_sn.toString());


    #if USE_CONFIGURABLE_DNS
        //***** Added for DNS address options *****
        staticIPInputs += item;
    
        item = FPSTR(WM_HTTP_FORM_LABEL);
        item += FPSTR(WM_HTTP_FORM_PARAM);

        item.replace("%{i}%", "dns1");
        item.replace("%{n}%", "dns1");
        item.replace("%{p}%", "DNS1 IP");
        item.replace("%{l}%", "15");
        item.replace("%{v}%", _WiFi_STA_IPconfig._sta_static_dns1.toString());

        staticIPInputs += item;

        item = FPSTR(WM_PK_HTTP_FORM_LABEL);
        item += FPSTR(WM_PK_HTTP_FORM_PARAM);

        item.replace("%{i}%", "dns2");
        item.replace("%{n}%", "dns2");
        item.replace("%{p}%", "DNS2 IP");
        item.replace("%{l}%", "15");
        item.replace("%{v}%", _WiFi_STA_IPconfig._sta_static_dns2.toString());
        //***** End added for DNS address options *****
    #endif

        staticIPInputs += item;
    }

    body.replace("%{staticIP}%", staticIPInputs);

    body += FPSTR(WM_HTTP_SCRIPT_NTP_HIDDEN);
    body += FPSTR(WM_PK_HTTP_FORM_SETTINGS_END);

    String page;
    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_SETTINGS_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        FPSTR(WM_PK_HTTP_SETTINGS_MSG),
        body,
        FPSTR(WM_HTTP_SCRIPT_NTP));

    responseTextHtml(request, page);
    LOGDEBUG(F("Sent config page"));
}

//////////////////////////////////////////

// Handle the WLAN save form and redirect to WLAN config page again
void ESPAsync_WiFiManager::handleSettingsSave(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Settings save"));

    //SAVE/_connect here
    _ssid = request->arg("s").c_str();
    _pass = request->arg("p").c_str();

    _ssid1 = request->arg("s1").c_str();
    _pass1 = request->arg("p1").c_str();

    ///////////////////////

#if USE_ESP_WIFIMANAGER_NTP

    if (request->hasArg("timezone"))
    {
        _timezoneName = request->arg("timezone");   //.c_str();

        LOGDEBUG1(F("TZ ="), _timezoneName);
    }
    else
    {
        LOGDEBUG(F("No TZ arg"));
    }

#endif

    ///////////////////////

    //parameters
    for (int i = 0; i < _paramsCount; i++)
    {
        if (_params[i] == NULL)
        {
            break;
        }

        //read parameter
        String value = request->arg(_params[i]->getID()).c_str();

        //store it in array
        value.toCharArray(_params[i]->_WMParam_data._value, _params[i]->_WMParam_data._length);

        LOGDEBUG2(F("Parameter and value :"), _params[i]->getID(), value);
    }

    if (request->hasArg("ip"))
    {
        String ip = request->arg("ip");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_ip, ip.c_str());

        LOGDEBUG1(F("New Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());
    }

    if (request->hasArg("gw"))
    {
        String gw = request->arg("gw");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_gw, gw.c_str());

        LOGDEBUG1(F("New Static Gateway ="), _WiFi_STA_IPconfig._sta_static_gw.toString());
    }

    if (request->hasArg("sn"))
    {
        String sn = request->arg("sn");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_sn, sn.c_str());

        LOGDEBUG1(F("New Static Netmask ="), _WiFi_STA_IPconfig._sta_static_sn.toString());
    }

#if USE_CONFIGURABLE_DNS

    //*****  Added for DNS Options *****
    if (request->hasArg("dns1"))
    {
        String dns1 = request->arg("dns1");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_dns1, dns1.c_str());

        LOGDEBUG1(F("New Static DNS1 ="), _WiFi_STA_IPconfig._sta_static_dns1.toString());
    }

    if (request->hasArg("dns2"))
    {
        String dns2 = request->arg("dns2");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_dns2, dns2.c_str());

        LOGDEBUG1(F("New Static DNS2 ="), _WiFi_STA_IPconfig._sta_static_dns2.toString());
    }

    //*****  End added for DNS Options *****
#endif

    String body(FPSTR(WM_PK_HTTP_SAVED));
    body.replace("%{v}%", _apName);
    body.replace("%{x}%", _ssid);
    body.replace("%{x1}%", _ssid1);

    String page;
    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_CREDENTIAL_SAVED_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        "",
        body,
        "");


    //LOGDEBUG1(F("page ="), page);
    responseTextHtml(request, page);
    LOGDEBUG(F("Sent wifi save page"));

    _connect = true; //signal ready to _connect/reset
}

//////////////////////////////////////////

// Handle shut down the _server page
void ESPAsync_WiFiManager::handleServerClose(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Server Close"));

    String body(FPSTR(WM_PK_HTTP_NETWORK_INFO));
    body.replace("%{ssid}%", WiFi_SSID());
    body.replace("%{ip}%", WiFi.localIP().toString());

    //body += F("Push button on device to restart configuration _server!");

    String page;
    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_CLOSE_SERVER_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        "",
        body,
        "");

    responseTextHtml(request, page);

    _stopConfigPortal = true; //signal ready to shutdown config portal

    LOGDEBUG(F("Sent _server close page"));
}

//////////////////////////////////////////

// Handle the info page
void ESPAsync_WiFiManager::handleInfo(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Info"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;

    String customHeadElem(_customHeadElement);
 
    if (_connect)
        customHeadElem += F("<meta http-equiv=\"refresh\" content=\"5; url=/i\">");

    String customBlock(F("<dl>"));

    if (_connect)
    {
        customBlock += F("<dt>Trying to connect</dt><dd>");
        customBlock += _wifiStatus;
        customBlock += F("</dd>");
    }

    customBlock += _pager;


    String body;
    reportStatus(body);

#ifdef ESP8266
    body += FPSTR(WM_PK_HTTP_CHIP_INFO_ESP8266);
#else
    body += FPSTR(WM_PK_HTTP_CHIP_INFO_ESP32);
#endif

#ifdef ESP8266
    body.replace("%{chipid}%", String(ESP.getChipId(), HEX));
#else   //ESP32
    body.replace("%{chipid}%", String(ESP_getChipId(), HEX));

    /*
    page += F("<tr><td>Chip OUI</td><td>");
    page += F("0x");
    page += String(getChipOUI(), HEX);
    page += F("</td></tr>");

    page += F("<tr><td>Chip Model</td><td>");
    page += ESP.getChipModel();
    page += F(" Rev");
    page += ESP.getChipRevision();
    */
#endif


#ifdef ESP8266
    body.replace("%{flashid}%", String(ESP.getFlashChipId(), HEX));
#else   //ESP32
    // TODO
#endif

    body.replace("%{ideFlashSize}%", String(ESP.getFlashChipSize()));
 
#ifdef ESP8266
    body.replace("%{realFlashSize}%", String(ESP.getFlashChipRealSize()));
#else   //ESP32
    // TODO
#endif

    body.replace("%{apip}%", WiFi.softAPIP().toString());
    body.replace("%{apmac}%", WiFi.softAPmacAddress());

    body.replace("%{ssid}%", WiFi_SSID());
    body.replace("%{staip}%", WiFi.localIP().toString());

    body.replace("%{stamac}%",  WiFi.macAddress());

    String page;
    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_INFO_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        customHeadElem,
        FPSTR(WM_PK_HTTP_DEVICE_INFO_MSG),
        body,
        WM_HTTP_SCRIPT_NTP);

    responseTextHtml(request, page);

    LOGDEBUG(F("Info page sent"));
}

//////////////////////////////////////////

// Handle the state page
void ESPAsync_WiFiManager::handleState(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("State-Json"));

    String page = F("{\"Soft_AP_IP\":\"");

    page += WiFi.softAPIP().toString();
    page += F("\",\"Soft_AP_MAC\":\"");
    page += WiFi.softAPmacAddress();
    page += F("\",\"Station_IP\":\"");
    page += WiFi.localIP().toString();
    page += F("\",\"Station_MAC\":\"");
    page += WiFi.macAddress();
    page += F("\",");

    if (WiFi.psk() != "")
    {
      page += F("\"Password\":true,");
    }
    else
    {
      page += F("\"Password\":false,");
    }

    page += F("\"SSID\":\"");
    page += WiFi_SSID();
    page += F("\"}");

    responseApplJson(request, page);

    LOGDEBUG(F("Sent state page in json format"));
}

//////////////////////////////////////////

/** Handle the scan page */
void ESPAsync_WiFiManager::handleScan(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Scan"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;   //KH

    LOGDEBUG(F("Scan-Json"));

    String page = F("{\"Access_Points\":[");

    // KH, display networks in page using previously scan results
    for (int i = 0; i < _wifiSSIDCount; i++)
    {
        if (_wifiSSIDs[i].duplicate == true)
            continue; // skip dups

        if (i != 0)
            page += F(", ");

        LOGDEBUG1(F("Index ="), i);
        LOGDEBUG1(F("SSID ="), _wifiSSIDs[i].SSID);
        LOGDEBUG1(F("RSSI ="), _wifiSSIDs[i].RSSI);

        int quality = getRSSIasQuality(_wifiSSIDs[i].RSSI);

        if (_minimumQuality == -1 || _minimumQuality < quality)
        {
            String item = FPSTR(WM_PK_JSON_ITEM);
            String rssiQ;

            rssiQ += quality;
            item.replace("%{v}%", _wifiSSIDs[i].SSID);
            item.replace("%{r}%", rssiQ);

        #if defined(ESP8266)

            if (_wifiSSIDs[i].encryptionType != ENC_TYPE_NONE)
        #else
            if (_wifiSSIDs[i].encryptionType != WIFI_AUTH_OPEN)
        #endif
            {
                item.replace("%{i}%", "true");
            }
            else
            {
                item.replace("%{i}%", "false");
            }

            page += item;
            delay(0);
        }
        else
        {
            LOGDEBUG(F("Skipping due to quality"));
        }
    }

    page += F("]}");

    responseApplJson(request, page);

    LOGDEBUG(F("Sent WiFiScan Data in Json format"));
}

//////////////////////////////////////////

// Handle the reset page
void ESPAsync_WiFiManager::handleReset(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Reset"));

    String body(FPSTR(WM_PK_HTTP_RESETTING_MSG));
    String page;
    buildHtmlPage(page,
        FPSTR(WM_PK_HTTP_WIFI_INFORMATION_MSG),
        FPSTR(WM_PK_HTTP_WM_SCRIPT),
        FPSTR(WM_PK_HTTP_STYLE),
        _customHeadElement,
        "",
        body,
        "");

    responseTextHtml(request, page);

    LOGDEBUG(F("Sent reset page"));
    delay(5000);

    // Temporary fix for issue of not clearing WiFi SSID/PW from flash of ESP32
    // See https://github.com/khoih-prog/ESP_WiFiManager/issues/25 and https://github.com/espressif/arduino-esp32/issues/400
    resetSettings();
    //WiFi.disconnect(true); // Wipe out WiFi credentials.
    //////

#ifdef ESP8266
    ESP.reset();
#else   //ESP32
    ESP.restart();
#endif

    delay(2000);
}

void ESPAsync_WiFiManager::handleOTAUpdate(AsyncWebServerRequest *pRequest)
{
    LOGDEBUG(F("handleUpdate"));

    responseTextHtml(pRequest, gUpdateOTAHtml, ARRAY_LENGTH(gUpdateOTAHtml));
}

void ESPAsync_WiFiManager::handleOTAUpdateStart(AsyncWebServerRequest *pRequest)
{
    // Get file MD5 hash from arg
    if (pRequest->hasParam("hash"))
    {
        String hash = pRequest->getParam("hash")->value();
        LOGDEBUG(String("MD5: "+hash+"\n").c_str());
        if (!Update.setMD5(hash.c_str()))
        {
            LOGDEBUG(F("ERROR: MD5 hash not valid\n"));
            return pRequest->send(400, "text/plain", "MD5 parameter invalid");
        }
    }

    #if UPDATE_DEBUG == 1
        // Serial output must be active to see the callback serial prints
        Serial.setDebugOutput(true);
    #endif

    // Pre-OTA update callback
    if (_preOTAUpdateCallback != NULL)
    {
        _preOTAUpdateCallback();
    }

    // Start update process
    #if defined(ESP8266)
        uint32_t update_size = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        Update.runAsync(true);
        if (!Update.begin(update_size, U_FLASH))
        {
            LOGDEBUG(F("Failed to start update process\n"));
            // Save error to string
            StreamString str;
            Update.printError(str);
            _update_error_str = str.c_str();
            _update_error_str.concat("\n");
            LOGDEBUG(_update_error_str.c_str());
        }
    #elif defined(ESP32)  
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
        {
            LOGDEBUG("Failed to start update process\n");
            // Save error to string
            StreamString str;
            Update.printError(str);
            _update_error_str = str.c_str();
            _update_error_str.concat("\n");
            LOGDEBUG(_update_error_str.c_str());
        }        
    #endif

    return pRequest->send((Update.hasError()) ? 400 : 200, "text/plain", (Update.hasError()) ? _update_error_str.c_str() : "OK");
}

void ESPAsync_WiFiManager::handleOTAUpdateUpload(AsyncWebServerRequest *pRequest) 
{
    // Post-OTA update callback
    if (_postOTAUpdateCallback != NULL)
    {
         _postOTAUpdateCallback(!Update.hasError());
    }

    AsyncWebServerResponse *pResponse = pRequest->beginResponse((Update.hasError()) ? 400 : 200, "text/plain", (Update.hasError()) ? _update_error_str.c_str() : "OK");
    pResponse->addHeader("Connection", "close");
    pResponse->addHeader("Access-Control-Allow-Origin", "*");
    pRequest->send(pResponse);

    // Set reboot flag
    if (!Update.hasError())
    {
        if (_auto_reboot)
        {
            _reboot_request_millis = millis();
            _reboot = true;
        }
    }
}

void ESPAsync_WiFiManager::handleOTAUpdateUploadFile(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) 
{
    if (!index) 
    {
        // Reset progress size on first frame
        _current_progress_size = 0;
    }

    // Write chunked data to the free sketch space
    if (len)
    {
        if (Update.write(data, len) != len) 
        {
            return request->send(400, "text/plain", "Failed to write chunked data to free space");
        }
        _current_progress_size += len;
        // Progress update callback
        if (_progressOTAUpdateCallback != NULL)
        {
            _progressOTAUpdateCallback(_current_progress_size, request->contentLength());
        }
    }
        
    if (final)
    { // if the final flag is set then this is the last frame of data
        if (!Update.end(true)) 
        { //true to set the size to the current progress
            // Save error to string
            StreamString str;
            Update.printError(str);
            _update_error_str = str.c_str();
            _update_error_str.concat("\n");
            LOGDEBUG(_update_error_str.c_str());
        }
    }
    else
    {
        return;
    }
}

//////////////////////////////////////////

void ESPAsync_WiFiManager::handleNotFound(AsyncWebServerRequest *request)
{
    if (captivePortal(request))
    {
        // If captive portal redirect instead of displaying the error page.
        return;
    }

    String message = "File Not Found\n\n";

    message += "URI: ";
    message += request->url();
    message += "\nMethod: ";
    message += (request->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for (uint8_t i = 0; i < request->args(); i++)
    {
        message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
    }

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
    request->send(200, WM_HTTP_HEAD_CT, message);

    // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
    delay(1);
#else

    AsyncWebServerResponse *response = request->beginResponse( 404, WM_HTTP_HEAD_CT2, message );

    response->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));
    response->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
    response->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

    request->send(response);
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
}

//////////////////////////////////////////

/**
   HTTPD redirector
   Redirect to captive portal if we got a request for another domain.
   Return true in that case so the page handler do not try to handle the request again.
*/
bool ESPAsync_WiFiManager::captivePortal(AsyncWebServerRequest *request)
{
    if (!isIp(request->host()))
    {
        LOGINFO(F("Request redirected to captive portal"));
        LOGINFO1(F("Location http://"), toStringIp(request->client()->localIP()));

        AsyncWebServerResponse *response = request->beginResponse(302, WM_HTTP_HEAD_CT2, "");

        response->addHeader("Location", String("http://") + toStringIp(request->client()->localIP()));

        request->send(response);

        return true;
    }

    LOGDEBUG1(F("request host IP ="), request->host());

    return false;
}

//////////////////////////////////////////

// start up config portal callback
void ESPAsync_WiFiManager::setAPCallback(std::function<void(ESPAsync_WiFiManager*)> func)
{
    _apcallback = func;
}

//////////////////////////////////////////

// start up save config callback
void ESPAsync_WiFiManager::setSaveConfigCallback(std::function<void()> func)
{
    _savecallback = func;
}

//////////////////////////////////////////

// sets a custom element to add to head, like a new style tag
void ESPAsync_WiFiManager::setCustomHeadElement(const char* element)
{
    _customHeadElement = element;
}

//////////////////////////////////////////

// if this is true, remove duplicated Access Points - defaut true
void ESPAsync_WiFiManager::setRemoveDuplicateAPs(bool removeDuplicates)
{
    _removeDuplicateAPs = removeDuplicates;
}

//////////////////////////////////////////

int ESPAsync_WiFiManager::getRSSIasQuality(const int& RSSI)
{
    int quality = 0;

    if (RSSI <= -100)
    {
        quality = 0;
    }
    else if (RSSI >= -50)
    {
        quality = 100;
    }
    else
    {
        quality = 2 * (RSSI + 100);
    }

    return quality;
}

//////////////////////////////////////////

// Is this an IP?
bool ESPAsync_WiFiManager::isIp(const String& str)
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        int c = str.charAt(i);

        if ( (c != '.') && (c != ':') && ( (c < '0') || (c > '9') ) )
        {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////

// IP to String
String ESPAsync_WiFiManager::toStringIp(const IPAddress& ip)
{
    String res = "";

    for (int i = 0; i < 3; i++)
    {
      res += String((ip >> (8 * i)) & 0xFF) + ".";
    }

    res += String(((ip >> 8 * 3)) & 0xFF);

    return res;
}

void ESPAsync_WiFiManager::setAutoReboot(bool enable)
{
    _auto_reboot = enable;
}

void ESPAsync_WiFiManager::onOTAStart(std::function<void()> callable)
{
    _preOTAUpdateCallback = callable;
}

void ESPAsync_WiFiManager::onOTAProgress(std::function<void(size_t current, size_t final)> callable)
{
    _progressOTAUpdateCallback= callable;
}

void ESPAsync_WiFiManager::onOTAEnd(std::function<void(bool success)> callable)
{
    _postOTAUpdateCallback = callable;
}


//////////////////////////////////////////

#if WM_SUPPORT_TRAIN_CONTROL
void ESPAsync_WiFiManager::handleTrainControl(AsyncWebServerRequest *pRequest)
{
    LOGDEBUG(F("Handle train control"));

    if (pRequest->args() > 0)
    {
        String pwmValue = pRequest->arg("pwr");

        String page = F("{\"pwr_request\":");

        page += pwmValue;
        page += F(",\"pwr_current\":");
        page += pwmValue;
        page += F("}");

    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        request->send(200, WM_HTTP_HEAD_CT, response);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else

        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_JSON, page);
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        pResponse->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

        LOGDEBUG(F("Sent train control page in json format"));

        return;
    }

    responseTextHtml(pRequest, gTrainControlHtml, ARRAY_LENGTH(gTrainControlHtml));
}

#endif

//////////////////////////////////////////

#ifdef ESP32
// We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
// SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
// Have to create a new function to store in EEPROM/SPIFFS for this purpose

String ESPAsync_WiFiManager::getStoredWiFiSSID()
{
    if (WiFi.getMode() == WIFI_MODE_NULL)
    {
        return String();
    }

    wifi_ap_record_t info;

    if (!esp_wifi_sta_get_ap_info(&info))
    {
        return String(reinterpret_cast<char*>(info.ssid));
    }
    else
    {
        wifi_config_t conf;

        esp_wifi_get_config(WIFI_IF_STA, &conf);

        return String(reinterpret_cast<char*>(conf.sta.ssid));
    }

    return String();
}

//////////////////////////////////////////

String ESPAsync_WiFiManager::getStoredWiFiPass()
{
    if (WiFi.getMode() == WIFI_MODE_NULL)
    {
        return String();
    }

    wifi_config_t conf;

    esp_wifi_get_config(WIFI_IF_STA, &conf);

    return String(reinterpret_cast<char*>(conf.sta.password));
}

//////////////////////////////////////////

uint32_t getChipID()
{
    uint64_t chipId64 = 0;

    for (int i = 0; i < 6; i++)
    {
        chipId64 |= ( ( (uint64_t) ESP.getEfuseMac() >> (40 - (i * 8)) ) & 0xff ) << (i * 8);
    }

    return (uint32_t) (chipId64 & 0xFFFFFF);
}

//////////////////////////////////////////

uint32_t getChipOUI()
{
    uint64_t chipId64 = 0;

    for (int i = 0; i < 6; i++)
    {
        chipId64 |= ( ( (uint64_t) ESP.getEfuseMac() >> (40 - (i * 8)) ) & 0xff ) << (i * 8);
    }

    return (uint32_t) (chipId64 >> 24);
}

#endif

//////////////////////////////////////////

#endif    // ESPAsync_WiFiManager_Impl_h
