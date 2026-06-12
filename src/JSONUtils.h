/*
  JSONUtils.h - Simple JSON utilities for serialization
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  MIT License
*/

#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <Arduino.h>

namespace JSONUtils {
    inline const __FlashStringHelper* ObjectStart()
    {
        return F("{");
    }

    inline const __FlashStringHelper* ObjectEnd()
    {
        return F("}");
    }

    inline const __FlashStringHelper* ArrayStart()
    {
        return F("[");
    }

    inline const __FlashStringHelper* ArrayEnd()
    {
        return F("]"); 
    }

    inline const __FlashStringHelper* ItemSeparator()
    {
        return F(",");
    }

    String Pair(const char* key, const char* pValue, bool isFirst = false);
    String Pair(const char* key, const String& value, bool isFirst = false);
    String Pair(const char* key, unsigned int value, bool isFirst = false);
    String Pair(const char* key, int value, bool isFirst = false);
    String Pair(const char* key, float value, bool isFirst = false);
    String Pair(const char* key, bool value, bool isFirst = false);

    String Pair(const __FlashStringHelper* key, const char* pValue, bool isFirst = false);
    String Pair(const __FlashStringHelper* key, const String& value, bool isFirst = false);
    String Pair(const __FlashStringHelper* key, unsigned int value, bool isFirst = false);
    String Pair(const __FlashStringHelper* key, int value, bool isFirst = false);
    String Pair(const __FlashStringHelper* key, float value, bool isFirst = false);
    String Pair(const __FlashStringHelper* key, bool value, bool isFirst = false);

    String Pair(const __FlashStringHelper* key, const __FlashStringHelper* pValue, bool isFirst = false);

    String NameValueRow(const __FlashStringHelper* name, const String& value, bool isFirst = false);
    String NameValueRow(const __FlashStringHelper* name, const char* value, bool isFirst = false);
    String NameValueRow(const __FlashStringHelper* name, unsigned int value, bool isFirst = false);
    String NameValueRow(const __FlashStringHelper* name, int value, bool isFirst = false);
    String NameValueRow(const __FlashStringHelper* name, float value, bool isFirst = false);
    String NameValueRow(const __FlashStringHelper* name, bool value, bool isFirst = false);

    String EncloseObject(const String& content);
    String EncloseArray(const String& content);
}

#endif // JSONUTILS_H