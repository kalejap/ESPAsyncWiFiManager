/*
  Version.h - Defines current software version
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DEVICEDEFINES_H
#define DEVICEDEFINES_H

#include "LanguageSupport.h"

#ifndef IOT_MAX_COMPONENTS
    #define IOT_MAX_COMPONENTS 8
#endif

#ifndef SW_MAJOR_VERSION
    #define SW_MAJOR_VERSION 1
#endif

#ifndef SW_MINOR_VERSION
    #define SW_MINOR_VERSION 0
#endif

#ifndef SW_PATCH_VERSION
    #define SW_PATCH_VERSION 0
#endif

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

#define IOT_SW_VERSION_STRING STRINGIZE(SW_MAJOR_VERSION) "." STRINGIZE(SW_MINOR_VERSION) "." STRINGIZE(SW_PATCH_VERSION)

///////////////////////////////////////////////////////

// IOT_CHIP_NAME: auto-detected from framework predefined macros.
// Override by defining IOT_CHIP_NAME before including this header.
#ifndef IOT_CHIP_NAME
    #if defined(ESP8266)
        #define IOT_CHIP_NAME "ESP8266"
    #elif defined(ESP32)
        #define IOT_CHIP_NAME "ESP32"
    #else
        #pragma message("IOT_CHIP_NAME: unknown chip — define IOT_CHIP_NAME manually.")
        #define IOT_CHIP_NAME "Unknown"
    #endif
#endif

#ifdef _IOT_DEVICE_NAME
    #define IOT_DEVICE_NAME STRINGIZE(_IOT_DEVICE_NAME)
#else
    #pragma message("_IOT_DEVICE_NAME is not defined. Defaulting to IoT Demo")
    #define IOT_DEVICE_NAME "IoT Demo"
#endif

#ifdef _IOT_DEVICE_MODEL
    #define IOT_DEVICE_MODEL STRINGIZE(_IOT_DEVICE_MODEL)
#else
    #pragma message("_IOT_DEVICE_MODEL is not defined. Defaulting to X1")
    #define IOT_DEVICE_MODEL "X1"
#endif

#ifdef _IOT_DEVICE_MANUFACTURER
    #define IOT_DEVICE_MANUFACTURER STRINGIZE(_IOT_DEVICE_MANUFACTURER)
#else
    #pragma message("_IOT_DEVICE_MANUFACTURER is not defined. Defaulting to DIY")
    #define IOT_DEVICE_MANUFACTURER "DIY"
#endif

#ifdef _IOT_COPYRIGHT
    #define IOT_COPYRIGHT STRINGIZE(_IOT_COPYRIGHT)
#else
    #pragma message("_IOT_COPYRIGHT is not defined. Defaulting to DIY 2025")
    #define IOT_COPYRIGHT "DIY 2025"
#endif


#ifdef _IOT_MAGIC_PREFIX
    #define IOT_MAGIC_PREFIX STRINGIZE(_IOT_MAGIC_PREFIX)
#else
    #pragma message("IOT_MAGIC_PREFIX is not defined. Defaulting to @*MAGic*@")
    #define IOT_MAGIC_PREFIX "@*MAGic*@"
#endif

#ifdef _IOT_OTA_UPDATE_URL
    #define IOT_OTA_UPDATE_URL STRINGIZE(_IOT_OTA_UPDATE_URL)
#else
    #pragma message("IOT_OTA_UPDATE_URL is not defined. Defaulting to http://0.0.0.0")
    #define IOT_OTA_UPDATE_URL "http://0.0.0.0"
#endif

#define IOT_APPLICATION_TITLE IOT_DEVICE_NAME " " IOT_DEVICE_MODEL

#define IOT_MAKE_HARDWARE_ID \
    IOT_CHIP_NAME "|" IOT_DEVICE_NAME "|" IOT_DEVICE_MODEL "|" IOT_DEVICE_MANUFACTURER

#define IOT_HARDWARE_TAG_PREFIX  IOT_MAGIC_PREFIX ":hw:"
#define IOT_VERSION_TAG_PREFIX   IOT_MAGIC_PREFIX ":ve:"
#define IOT_LANGUAGE_TAG_PREFIX  IOT_MAGIC_PREFIX ":lg:"

#ifndef IOT_LANGUAGE_STRING
    #define IOT_LANGUAGE_STRING "en-us"
#endif

#endif // DEVICEDEFINES_H
