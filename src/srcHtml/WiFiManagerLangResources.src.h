/*************************************************************
 Programatically generated source file
 -------------------------------------
 Peter Kaleja      31/12/2024 New file
 *************************************************************/

#pragma once

#ifndef WiFiManagerLangResource_h
#define WiFiManagerLangResource_h

#include "LanguageSupport.h"
#include "DeviceDefines.h"

////////////////////////////////////////////////////

const char WM_PK_HTML_HEAD_1[] PROGMEM = "%HEAD_1%";
// Title
const char WM_PK_HTML_HEAD_2[] PROGMEM = "</title>";
// Script
const char WM_PK_HTML_STYLE_S[] PROGMEM = "<style>";
// Style
const char WM_PK_HTML_STYLE_E[] PROGMEM = "</style>";
// Optional custom element
const char WM_PK_HTML_HEAD_3[] PROGMEM = "</head><body><div class='container'>";
// h2 start
const char WM_PK_HTML_H2_S[] PROGMEM = "<h2>";
// h2 end
const char WM_PK_HTML_H2_E[] PROGMEM = "</h2>";
// body
const char WM_PK_HTML_FOOT[] PROGMEM = "</div></body></html>";

/*
const char WM_PK_HTTP_HTML[] PROGMEM = 
"<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/><title>%{title}%</title>"
"%{script}%"
"%{optScriptElem}%"
"<style>%{style}%</style>"
"%{customHeadElem}%"
"</head><body><div class='container'>"
"<h2>%{headTitle}%</h2>"
"%{body}%"
"</div></body></html>";
*/

// Javascript files
const char WM_PK_UTILS_JS[] PROGMEM = "%WM_PK_UTILS_JS%";
const char WM_PK_MD5_UTILS_JS[] PROGMEM = "%WM_PK_MD5_UTILS_JS%";
const char WM_PK_OTA_JS[] PROGMEM = "%WM_PK_OTA_JS%";
const char WM_PK_MODULE_POLYFILL_JS[] PROGMEM = "%WM_PK_MODULE_POLYFILL_JS%";
const char WM_PK_RESTART_JS[] PROGMEM = "%WM_PK_RESTART_JS%";
const char WM_PK_HW_STATUS_JS[] PROGMEM = "%WM_PK_HW_STATUS_JS%";

// HTML files
const char WM_PK_INDEX_HTML[] PROGMEM = "%WM_PK_INDEX_HTML%";
#ifdef WM_REMOTE_UPDATE
const char WM_PK_OTA_REMOTE_JS[] PROGMEM = "%WM_PK_OTA_REMOTE_JS%";
const char WM_PK_OTA_HTML[] PROGMEM = "%WM_PK_OTA_REMOTE_HTML%";
#else
const char WM_PK_OTA_HTML[] PROGMEM = "%WM_PK_OTA_HTML%";
#endif
const char WM_PK_INFO_HTML[] PROGMEM = "%WM_PK_INFO_HTML%";
const char WM_PK_WIFI_HTML[] PROGMEM = "%WM_PK_WIFI_HTML%";
#ifdef WM_SUPPORT_HOME_ASSISTANT
const char WM_PK_MQTT_HTML[] PROGMEM = "%WM_PK_MQTT_HTML%";
#endif


// CSS files
const char WM_PK_STYLE_CSS[] PROGMEM = "%WM_PK_STYLE_CSS%";



const char WM_PK_HTTP_COMMON_SCRIPT[] PROGMEM = "<script>%COMMON_SCRIPT%</script>";
const char WM_PK_HTTP_WM_SCRIPT[] PROGMEM = "<script>%WM_SCRIPT%</script>";
const char WM_PK_HTTP_OTA_SCRIPT[] PROGMEM = "<script type=\"module\" crossorigin>%OTA_SCRIPT%</script>";
const char WM_PK_HTTP_STYLE[] PROGMEM = "%STYLE%";
const char WM_PK_HTTP_OPTIONS_MSG[] PROGMEM = L_GENERAL_OPTIONS;
const char WM_PK_HTTP_SETTINGS_MSG[] PROGMEM = L_GENERAL_SETTINGS;
const char WM_PK_HTTP_CLOSE_SERVER_MSG[] PROGMEM = L_CLOSE_SERVER;
const char WM_PK_HTTP_CREDENTIAL_SAVED_MSG[] PROGMEM = L_CREDENTAILS_SAVED;
const char WM_PK_HTTP_WIFI_INFORMATION_MSG[] PROGMEM = L_WIFI_INFORMATION;
const char WM_PK_HTTP_DEVICE_INFO_MSG[] PROGMEM = L_DEVICE_INFO;
const char WM_PK_HTTP_INFO_MSG[] PROGMEM = L_INFO;
const char WM_PK_HTTP_RESETTING_MSG[] PROGMEM = L_RESETTING;
const char WM_PK_HTTP_FIRMWARE_UPDATE_MSG[] PROGMEM = L_OTA_FIRMWARE_UPDATE;
const char WM_PK_HTTP_MESSAGE[] PROGMEM = "<div class=\"msg\">%{msg}%</div>";
const char WM_PK_HTTP_NETWORK_INFO[] PROGMEM = "%NETWORK_INFO%";
const char WM_PK_HTTP_FLDSET_START[] PROGMEM = "<fieldset>";
const char WM_PK_HTTP_FLDSET_END[] PROGMEM = "</fieldset>";
const char WM_PK_HTTP_OTA_UPDATE[] PROGMEM = "%OTA_UPDATE%";
#ifdef WM_SUPPORT_HOME_ASSISTANT
    const char WM_PK_HTTP_FORM_MQTT_SETTINGS[] PROGMEM = "%MQTT_SETTINGS%";
#endif
#ifdef WM_SUPPORT_TRAIN_CONTROL
    const char WM_PK_HTTP_TRAIN_CONTROL_OPTION[] PROGMEM = "%TRAIN_CONTROL_OPTION%";
    const char WM_PK_HTTP_TRAIN_CONTROL[] PROGMEM = "%TRAIN_CONTROL%";
    const char WM_PK_HTTP_PWMCTRL_SCRIPT[] PROGMEM = "<script>%PWMCTRL_JS%</script>";
    const char WM_PK_HTTP_TRAIN_STYLE[] PROGMEM = "%TRAIN_STYLE%";
#endif

const char WM_PK_HTTP_FORM_START[] PROGMEM = "%FORM_START%";
const char WM_PK_HTTP_FORM_END[] PROGMEM = "%FORM_END%";
const char WM_PK_HTTP_FORM_LABEL_BEFORE[]  PROGMEM   = "%FORM_LABEL_BEFORE%";
const char WM_PK_HTTP_FORM_LABEL_AFTER[]   PROGMEM   = "%FORM_LABEL_AFTER%";
const char WM_PK_HTTP_FORM_LABEL[] PROGMEM = "%FORM_LABEL%";
const char WM_PK_HTTP_FORM_PARAM[] PROGMEM = "%FORM_PARAM%";
const char WM_PK_HTTP_SAVED[] PROGMEM = "%SAVED%";
const char WM_PK_HTTP_CHIP_INFO_ESP8266[] PROGMEM = "%CHIP_INFO_ESP8266%";
const char WM_PK_HTTP_ITEM[] PROGMEM = "%ITEM%";
const char WM_PK_JSON_ITEM[] PROGMEM = "%JSON_ITEM%";


#endif    // WiFiManagerLangResource_h
