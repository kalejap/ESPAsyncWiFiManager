/*
  JSONUtils.cpp - Simple JSON utilities for serialization
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  MIT License
*/

#include "JSONUtils.h"

namespace JSONUtils{
    String Pair(const char* key, const char* pValue, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        if (pValue == nullptr)
        {
            item += "null";
        }
        else
        {
            item += "\"" + String(pValue) + "\"";
        }
        return item;
    }

    String Pair(const char* key, const String& value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += "\"" + value + "\"";
        return item;
    }

    String Pair(const char* key, unsigned int value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const char* key, int value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const char* key, float value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const char* key, bool value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value ? "true" : "false");
        return item;
    }


    String Pair(const __FlashStringHelper* key, const char* pValue, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        if (pValue == nullptr)
        {
            item += "null";
        }
        else
        {
            item += "\"" + String(pValue) + "\"";
        }
        return item;
    }

    String Pair(const __FlashStringHelper* key, const String& value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += "\"" + value + "\"";
        return item;
    }

    String Pair(const __FlashStringHelper* key, unsigned int value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const __FlashStringHelper* key, int value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const __FlashStringHelper* key, float value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value);
        return item;
    }

    String Pair(const __FlashStringHelper* key, bool value, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        item += String(value ? "true" : "false");
        return item;
    }

    String Pair(const __FlashStringHelper* key, const __FlashStringHelper* pValue, bool isFirst)
    {
        String item = (isFirst ? "\"" : ",\"") + String(key) + "\":";
        if (pValue == nullptr)
        {
            item += "null";
        }
        else
        {
            item += "\"" + String(pValue) + "\"";
        }
        return item;
    }


    String NameValueRow(const __FlashStringHelper* name, const String& value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String NameValueRow(const __FlashStringHelper* name, const char* value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String NameValueRow(const __FlashStringHelper* name, unsigned int value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String NameValueRow(const __FlashStringHelper* name, int value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String NameValueRow(const __FlashStringHelper* name, float value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String NameValueRow(const __FlashStringHelper* name, bool value, bool isFirst)
    {
        String item = (isFirst ? "{" : ",{") + Pair(F("name"), name, true) + Pair(F("value"), value, false) + "}";
        return item;
    }

    String EncloseObject(const String& content)
    {
        return ObjectStart() + content + ObjectEnd() ;
    }

    String EncloseArray(const String& content)
    {
        return ArrayStart() + content + ArrayEnd() ;
    }
}