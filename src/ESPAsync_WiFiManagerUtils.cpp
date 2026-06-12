/*
  ESPASync_WiFiManagerUtils.cpp - Simple JSON utilities for serialization
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  MIT License
*/

#include "ESPAsync_WiFiManagerUtils.h"

// Definition of global variable for HTML headers to prevent caching
const HTTPHeaderItem gHTMLHeaderItems[] = {
    { FPSTR("Cache-Control"), FPSTR("no-cache, no-store, must-revalidate") }, // HTTP 1.1.
    { FPSTR("Pragma"), FPSTR("no-cache") },                                   // HTTP 1.0.
    { FPSTR("Expires"), FPSTR("-1") }                                       // Proxies.
};
const HTTPHeaderBlock gHTMLHeaders = { gHTMLHeaderItems, ARRAY_LENGTH(gHTMLHeaderItems) };

// Definition of global variable for CSS headers
const HTTPHeaderItem gCSSHeaderItems[] = {
    { FPSTR("Cache-Control"), FPSTR("max-age=604800") }, // Cache for 1 week
};
const HTTPHeaderBlock gCSSHeaders = { gCSSHeaderItems, ARRAY_LENGTH(gCSSHeaderItems) };

// Definition of global variable for Javascript headers
const HTTPHeaderItem gJSHeaderItems[] = {
    { FPSTR("Cache-Control"), FPSTR("max-age=604800") }, // Cache for 1 week
};
const HTTPHeaderBlock gJSHeaders = { gJSHeaderItems, ARRAY_LENGTH(gJSHeaderItems) };

namespace {
    void CopyBlock(uint8_t *pBufferWithOffset, const HTTPMemoryBlockItem *pBlock, size_t blockOffset, 
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

    void CopyBlock(uint8_t *pBufferWithOffset, const HTTPMemoryBlock2Item *pBlock, size_t blockOffset, 
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

    inline size_t BlockItemSize(const HTTPMemoryBlock2Item *pBlocks)
    {
        return pBlocks ? (pBlocks->pString ? pBlocks->pString->length() : pBlocks->size) : 0;
    }

    // Function to send memory block
    size_t SendMemoryBlock (const HTTPMemoryBlock *pBlock, uint8_t *pBuffer, size_t bufLen, size_t index)
    {
        uint8_t *pBufferWithOffset = pBuffer;
        size_t remainingBufLen = (bufLen & 0xFFFC); // Align buffer length to 4
        size_t currentIndex = index;
        size_t blockStart = 0;
        size_t sizeOfCopiedMem = 0;

        //LOGDEBUG1("SendMemoryBlock: bufLen=", bufLen);
        //LOGDEBUG1("SendMemoryBlock: remainingBufLen=", remainingBufLen);
        //LOGDEBUG1("SendMemoryBlock: countOfBlocks=", countOfBlocks);

        for (size_t iBlock = 0; iBlock < pBlock->count && remainingBufLen > 0; iBlock++)
        {
            size_t blockEnd = blockStart + pBlock->pItems[iBlock].size;

            while (blockStart <= currentIndex &&
                   blockEnd > currentIndex &&
                   remainingBufLen > 0)
            {
                size_t blockOffset = currentIndex - blockStart;
                size_t blockSize = pBlock->pItems[iBlock].size - blockOffset;
                if (blockSize >= remainingBufLen)
                {
                    blockSize = remainingBufLen;
                }

                CopyBlock(pBufferWithOffset, pBlock->pItems + iBlock, blockOffset, blockSize);
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
    size_t SendMemoryBlock (const HTTPMemoryBlock2 *pBlock,
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

        for (size_t iBlock = 0; iBlock < pBlock->count && remainingBufLen > 0; iBlock++)
        {
            size_t bs = BlockItemSize(pBlock->pItems + iBlock);
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

                CopyBlock(pBufferWithOffset, pBlock->pItems + iBlock, blockOffset, blockSize);
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
}

namespace ESPAsync_WiFiManagerUtils {

    /*
    // Helper function to send memory block in chunks
    size_t SendMemoryBlock(const MemoryBlock& memBlock, uint8_t *pBuffer, size_t bufLen, size_t index)
    {
        size_t bytesSent = 0;
        size_t memSize = memBlock.size;

        if (index >= memSize)
        {
            return 0; // No more data to send
        }

        size_t bytesToSend = memSize - index;
        if (bytesToSend > bufLen)
        {
            bytesToSend = bufLen;
        }

        memcpy(pBuffer, reinterpret_cast<const uint8_t*>(memBlock.pData) + index, bytesToSend);
        bytesSent = bytesToSend;

        return bytesSent;
    }
*/

    void responseTextHtml(AsyncWebServerRequest *pRequest, const String& page)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        pRequest->send(200, WM_HTTP_HEAD_CT, page);

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_CT_TEXT_HTML, page);
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

    void responseText(AsyncWebServerRequest *pRequest, const HTTPResponseBlock *pBlock)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        // Ensure we send the configured content-type from the response block
        String _ct = String(pBlock->pContentType);
        pRequest->send(200, _ct.c_str(), "");

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        // Resolve content type from flash to a runtime string, then pass it to beginChunkedResponse
        String _ct = String(pBlock->pContentType);
        //LOGDEBUG1(F("responseText"), _ct);

        AsyncWebServerResponse *pResponse = pRequest->beginChunkedResponse(_ct.c_str(),
            [pBlock](uint8_t *pBuffer, size_t bufLen, size_t index) -> size_t
        {
            return SendMemoryBlock(pBlock->pBlock, pBuffer, bufLen, index);
        });

        for (size_t i = 0; i < pBlock->pHeader->count; i++)
        {
            pResponse->addHeader(pBlock->pHeader->pItems[i].pName, pBlock->pHeader->pItems[i].pValue);
        }
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }

    void responseText(AsyncWebServerRequest *pRequest, const HTTPResponseBlock2 *pBlock)
    {
    #if ( USING_ESP32_S2 || USING_ESP32_C3 )
        // Ensure we send the configured content-type from the response block
        String _ct = String(pBlock->pContentType);
        pRequest->send(200, _ct.c_str(), "");

        // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
        delay(1);
    #else
        // Resolve content type from flash to a runtime string, then pass it to beginChunkedResponse
        String _ct = String(pBlock->pContentType);
        AsyncWebServerResponse *pResponse = pRequest->beginChunkedResponse(_ct.c_str(),
            [pBlock](uint8_t *pBuffer, size_t bufLen, size_t index) -> size_t
        {
            return SendMemoryBlock(pBlock->pBlock, pBuffer, bufLen, index);
        });

        for (size_t i = 0; i < pBlock->pHeader->count; i++)
        {
            pResponse->addHeader(pBlock->pHeader->pItems[i].pName, pBlock->pHeader->pItems[i].pValue);
        }
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

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

        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_CT_JSON, page);
        pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));

    #if USING_CORS_FEATURE
        pResponse->addHeader(FPSTR(WM_HTTP_CORS), _CORS_Header);
    #endif

        pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
        pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");
        pRequest->send(pResponse);
    #endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )
    }

} // namespace ESPAsync_WiFiManagerUtils

