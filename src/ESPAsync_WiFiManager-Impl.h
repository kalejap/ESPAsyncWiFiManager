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

// Utilities and shared structs
#include "JSONUtils.h"
#include "ESPAsync_WiFiManagerUtils.h"


// index.html
WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(gIndexHtml, WM_PK_INDEX_HTML);
// ota.html
WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(gOTAHtml, WM_PK_OTA_HTML);
// info.html
WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(gInfoHtml, WM_PK_INFO_HTML);
// wifi.html
WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(gWiFiHtml, WM_PK_WIFI_HTML);
#ifdef WM_SUPPORT_HOME_ASSISTANT
// mqtt.html
WM_DEFINE_STATIC_HTML_FILE_AS_SINGLE_BLOCK(gMQTTHtml, WM_PK_MQTT_HTML);
#endif


// style.css
WM_DEFINE_STATIC_CSS_FILE_AS_SINGLE_BLOCK(gStyleCSS, WM_PK_STYLE_CSS);
// ota.js
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gOTAJS, WM_PK_OTA_JS);
// utils.js
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gUtilsJS, WM_PK_UTILS_JS);
// md5_utils.js
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gMD5UtilsJS, WM_PK_MD5_UTILS_JS);
// module_polyfill.js
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gModulePolyfillJS, WM_PK_MODULE_POLYFILL_JS);
// restart.js
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gRestartJS, WM_PK_RESTART_JS);
#ifdef WM_REMOTE_UPDATE
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <IoTDevice.h>
WM_DEFINE_STATIC_JS_FILE_AS_SINGLE_BLOCK(gOTARemoteJS, WM_PK_OTA_REMOTE_JS);
#endif


HTTPMemoryBlockItem gTrainControlHtmlChunks[] = {
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_HEAD_1)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTTP_FIRMWARE_UPDATE_MSG)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_HEAD_2)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTTP_PWMCTRL_SCRIPT)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_STYLE_S)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTTP_TRAIN_STYLE)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_STYLE_E)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_HEAD_3)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTTP_TRAIN_CONTROL)),
    WM_MEMORY_P_BLOCK_ITEM(FPSTR(WM_PK_HTML_FOOT))
};

const HTTPMemoryBlock gTrainControlHtmlBlock = WM_MEMORY_BLOCK(gTrainControlHtmlChunks);

const HTTPResponseBlock gTrainControlHtmlBlockResponse = { 
    FPSTR(WM_HTTP_HEAD_CT_TEXT_HTML), 
    &gTrainControlHtmlBlock,
    &gHTMLHeaders, 
};

//////////////////////////////////////////
// PK
namespace {
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
};
// PK
//////////////////////////////////////////

//////////////////////////////////////////

ESPAsync_WMParameter::ESPAsync_WMParameter(const char *custom)
{
    _WMParam_data._id = nullptr;
    _WMParam_data._placeholder = nullptr;
    _WMParam_data._length = 0;
    _WMParam_data._value = nullptr;
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
    _server->on("/wifisave", std::bind(&ESPAsync_WiFiManager::handleWiFiSave, this, _1)).setFilter(filterFn);
    _server->on("/close",   std::bind(&ESPAsync_WiFiManager::handleServerClose, this, _1)).setFilter(filterFn);
    _server->on("/i",       std::bind(&ESPAsync_WiFiManager::handleInfo, this, _1)).setFilter(filterFn);
    _server->on("/r",       std::bind(&ESPAsync_WiFiManager::handleReset, this, _1)).setFilter(filterFn);
    _server->on("/sq",   std::bind(&ESPAsync_WiFiManager::handleSystemQuery, this, _1)).setFilter(filterFn);
    _server->on("/scan",    std::bind(&ESPAsync_WiFiManager::handleScan, this, _1)).setFilter(filterFn);
    // OTA update handlers
    _server->on("/ota/start", std::bind(&ESPAsync_WiFiManager::handleOTAUpdateStart, this, _1)).setFilter(filterFn);
    _server->on("/ota/upload", HTTP_POST,
        std::bind(&ESPAsync_WiFiManager::handleOTAUpdateUpload, this, _1),
        std::bind(&ESPAsync_WiFiManager::handleOTAUpdateUploadFile, this, _1, _2, _3, _4, _5, _6));
#ifdef WM_REMOTE_UPDATE
    _server->on("/ota/remote-check",
        std::bind(&ESPAsync_WiFiManager::handleOTARemoteCheck, this, _1)).setFilter(filterFn);
    _server->on("/ota/remote-start",
        std::bind(&ESPAsync_WiFiManager::handleOTARemoteStart, this, _1)).setFilter(filterFn);
#endif

    //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
    _server->on("/fwlink",   std::bind(&ESPAsync_WiFiManager::handleRoot, this, _1)).setFilter(filterFn);

#ifdef WM_SUPPORT_TRAIN_CONTROL
    _server->on("/tcontrol",   std::bind(&ESPAsync_WiFiManager::handleTrainControl, this, _1)).setFilter(filterFn);
#endif

    attachCustomHandlers(filterFn);

    _server->onNotFound (std::bind(&ESPAsync_WiFiManager::handleNotFound, this, _1));
}


//////////////////////////////////////////

String ESPAsync_WiFiManager::networkListAsString()
{
    String pager;

    LOGDEBUG1(F("ESPAsync_WiFiManager::networkListAsString: _wifiSSIDCount="), _wifiSSIDCount);
 
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

#ifdef WM_REMOTE_UPDATE

static String _jsonExtractString(const String& json, const String& key) {
    int ki = json.indexOf("\"" + key + "\"");
    if (ki < 0) return "";
    int ci = json.indexOf(':', ki);
    if (ci < 0) return "";
    int q1 = json.indexOf('"', ci + 1);
    if (q1 < 0) return "";
    int q2 = json.indexOf('"', q1 + 1);
    if (q2 < 0) return "";
    return json.substring(q1 + 1, q2);
}

static String _jsonExtractObject(const String& json, const String& key) {
    int ki = json.indexOf("\"" + key + "\"");
    if (ki < 0) return "{}";
    int ob = json.indexOf('{', ki);
    if (ob < 0) return "{}";
    int depth = 0, i = ob;
    for (; i < (int)json.length(); ++i) {
        if      (json[i] == '{') ++depth;
        else if (json[i] == '}') { if (--depth == 0) break; }
    }
    return json.substring(ob, i + 1);
}


void ESPAsync_WiFiManager::handleOTARemoteCheck(AsyncWebServerRequest *request) {
    if (!request->hasArg("url")) {
        ESPAsync_WiFiManagerUtils::responseApplJson(request, F("{\"error\":\"Missing url\"}"));
        return;
    }
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    if (!http.begin(client, request->arg("url"))) {
        ESPAsync_WiFiManagerUtils::responseApplJson(request, F("{\"error\":\"HTTP begin failed\"}"));
        return;
    }
    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        http.end();
        ESPAsync_WiFiManagerUtils::responseApplJson(request,
            "{\"error\":\"HTTP " + String(code) + "\"}");
        return;
    }
    String body = http.getString();
    http.end();

    String remHwid   = _jsonExtractString(body, "hwid");
    String localHwid = (_hardwareId != nullptr) ? String(FPSTR(_hardwareId)) : String("");
    if (localHwid.length() > 0 && remHwid != localHwid) {
        ESPAsync_WiFiManagerUtils::responseApplJson(request,
            "{\"error\":\"Hardware ID mismatch\","
            "\"remote\":\"" + remHwid + "\","
            "\"local\":\"" + localHwid + "\"}");
        return;
    }

    String remVerStr       = _jsonExtractString(body, "version");
    SemanticVersion remVer = SemanticVersion::fromString(remVerStr);
    SemanticVersion localVer = { SW_MAJOR_VERSION, SW_MINOR_VERSION, SW_PATCH_VERSION };
    String fwJson          = _jsonExtractObject(body, "firmwares");
    bool isNewer           = (remVer > localVer);
    bool isSameVer         = (remVerStr == F(IOT_SW_VERSION_STRING));
    bool canSwitch         = isSameVer && (fwJson.length() > 2);

    ESPAsync_WiFiManagerUtils::responseApplJson(request,
        "{\"available\":"  + String(isNewer   ? "true" : "false") +
        ",\"canSwitch\":"  + String(canSwitch ? "true" : "false") +
        ",\"version\":\""  + remVerStr + "\"" +
        ",\"current\":\""  IOT_SW_VERSION_STRING "\"" +
        ",\"lang\":\""     IOT_LANGUAGE_STRING "\"" +
        ",\"firmwares\":"  + fwJson + "}");
}

void ESPAsync_WiFiManager::handleOTARemoteStart(AsyncWebServerRequest *request) {
    if (!request->hasArg("url")) {
        request->send(400, "text/plain", "Missing url");
        return;
    }
    _pendingRemoteUrl     = request->arg("url");
    _pendingRemoteStartMs = millis() + 1000;

    if (_preOTAUpdateCallback) _preOTAUpdateCallback();

    request->send(200, "text/plain", "Remote update scheduled");
}

#endif  // WM_REMOTE_UPDATE

void ESPAsync_WiFiManager::safeLoop()
{
#ifdef WM_REMOTE_UPDATE
    if (_pendingRemoteUrl.length() > 0 && millis() >= _pendingRemoteStartMs) {
        String url = _pendingRemoteUrl;
        _pendingRemoteUrl = "";

        WiFiClientSecure client;
        client.setInsecure();

        ESPhttpUpdate.onProgress([this](int cur, int total) {
            if (_progressOTAUpdateCallback)
                _progressOTAUpdateCallback((size_t)cur, (size_t)total);
        });

        t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);
        bool success = (ret == HTTP_UPDATE_OK);

        if (_postOTAUpdateCallback) _postOTAUpdateCallback(success);
        if (success && _auto_reboot) {
            _reboot_request_millis = millis();
            _reboot = true;
        }
    }
#endif
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

        if (_reboot && millis() - _reboot_request_millis > 2000)
        {
            LOGDEBUG("startConfigPortal: OTA complete, rebooting...\n");
            #if defined(ESP8266) || defined(ESP32)
                ESP.restart();
            #elif defined(TARGET_RP2040)
                rp2040.reboot();
            #endif
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

        #if (ESP8266 && (USING_ESP8266_CORE_VERSION >= 30000))
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


void ESPAsync_WiFiManager::handleRoot(AsyncWebServerRequest *pRequest)
{
    LOGDEBUG(F("handleRoot"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;

    if (captivePortal(pRequest))
    {
        // If captive portal redirect instead of displaying the error page.
        return;
    }

    /*
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
 
#ifdef WM_SUPPORT_TRAIN_CONTROL
    configurableOptions += FPSTR(WM_PK_HTTP_TRAIN_CONTROL_OPTION);
#endif
    */

    ESPAsync_WiFiManagerUtils::responseText(pRequest, &gIndexHtml);
}

// Handle the WLAN save form and redirect to WLAN config page again
inline void ESPAsync_WiFiManager::handleWiFiSave(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("ESPAsync_WiFiManager::handleWiFiSave"));

    //SAVE/_connect here
    _ssid = request->arg("ssid1").c_str();
    _pass = request->arg("pwd1").c_str();

    _ssid1 = request->arg("ssid2").c_str();
    _pass1 = request->arg("pwd2").c_str();

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

    if (request->hasArg("staticip1"))
    {
        String ip = request->arg("staticip1");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_ip, ip.c_str());

        LOGDEBUG1(F("New Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());
    }

    if (request->hasArg("staticgw1"))
    {
        String gw = request->arg("staticgw1");

        optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_gw, gw.c_str());

        LOGDEBUG1(F("New Static Gateway ="), _WiFi_STA_IPconfig._sta_static_gw.toString());
    }

    if (request->hasArg("staticsubnet1"))
    {
        String sn = request->arg("staticsubnet1");

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
    ESPAsync_WiFiManagerUtils::responseTextHtml(request, page);
    LOGDEBUG(F("Sent wifi save page"));

    _connect = true; //signal ready to _connect/reset
}

//////////////////////////////////////////

// Handle shut down the _server page
inline void ESPAsync_WiFiManager::handleServerClose(AsyncWebServerRequest *request)
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

    ESPAsync_WiFiManagerUtils::responseTextHtml(request, page);

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

    ESPAsync_WiFiManagerUtils::responseTextHtml(request, page);

    LOGDEBUG(F("Info page sent"));
}

//////////////////////////////////////////

// Handle the state page
inline void ESPAsync_WiFiManager::handleSystemQuery(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("ESPAsync_WiFiManager::handleSystemQuery"));

    if (handleCustomSystemQuery(request))
    {
        LOGDEBUG(F("ESPAsync_WiFiManager::handleSystemQuery: Handled by user custom handler"));
        return;
    }

    String page;

    if (request->hasArg("dx"))
    {
        String dx = request->arg("dx");

        if (dx == "status")
        {
            page = JSONUtils::EncloseObject(
                JSONUtils::Pair(F("Soft_AP_IP"),  WiFi.softAPIP().toString(), true) +
                JSONUtils::Pair(F("Soft_AP_MAC"), WiFi.softAPmacAddress()) +
                JSONUtils::Pair(F("Station_IP"), WiFi.localIP().toString()) +
                JSONUtils::Pair(F("Station_MAC"), WiFi.macAddress()) +
                JSONUtils::Pair(F("SSID"), WiFi_SSID()) +
                JSONUtils::Pair(F("Password"), (WiFi.psk() != "") ? true : false));
        }
        else if(dx == "hwinfo")
        {
            page = JSONUtils::EncloseArray(
            #ifdef ESP8266
                JSONUtils::NameValueRow(F("Chip ID"), String(ESP.getFlashChipId(), HEX), true) +
                JSONUtils::NameValueRow(F("IDE Flash Size"), ESP.getFlashChipSize()) +
                JSONUtils::NameValueRow(F("Real Flash Size"), ESP.getFlashChipRealSize()) +
            #else
                // TODO
                JSONUtils::NameValueRow(F("Chip ID"), "---", true) +
                JSONUtils::NameValueRow(F("IDE Flash Size"), ESP.getFlashChipSize()) +
                // TODO
                JSONUtils::NameValueRow(F("Real Flash Size"), "---") +
            #endif

                JSONUtils::NameValueRow(F("Access Point IP"), WiFi.softAPIP().toString()) +
                JSONUtils::NameValueRow(F("Access Point MAC"), WiFi.softAPmacAddress()) +
                JSONUtils::NameValueRow(F("SSID"), WiFi_SSID()) +
                JSONUtils::NameValueRow(F("Station IP"), WiFi.localIP().toString()) +
                JSONUtils::NameValueRow(F("Station MAC"), WiFi.macAddress()));
        }
        else
        {
            LOGDEBUG(F("ESPAsync_WiFiManager::handleSystemQuery: Unsupported value of parameter dx"));
            page = JSONUtils::EncloseObject("");
        }
    }
    else
    {
        page = JSONUtils::EncloseObject(JSONUtils::Pair(F("Error"), F("DX argument missing."), true));
    }


    ESPAsync_WiFiManagerUtils::responseApplJson(request, page);
}

//////////////////////////////////////////

/** Handle the scan page */
void ESPAsync_WiFiManager::handleScan(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Scan"));

    // Disable _configPortalTimeout when someone accessing Portal to give some time to config
    _configPortalTimeout = 0;   //KH

    LOGDEBUG(F("Scan-Json"));

     String jsonStr;
    // KH, display networks in page using previously scan results
    for (int i = 0; i < _wifiSSIDCount; i++)
    {
        if (_wifiSSIDs[i].duplicate == true)
            continue; // skip dups

        LOGDEBUG1(F("Index ="), i);
        LOGDEBUG1(F("SSID ="), _wifiSSIDs[i].SSID);
        LOGDEBUG1(F("RSSI ="), _wifiSSIDs[i].RSSI);

        int quality = getRSSIasQuality(_wifiSSIDs[i].RSSI);

        if (_minimumQuality == -1 || _minimumQuality < quality)
        {
            // {\"SSID\":\"%{v}%\", \"Encryption\":%{i}%, \"Quality\":\"%{r}%\"}
            String item = FPSTR(WM_PK_JSON_ITEM);
            String rssiQ;

            rssiQ += quality;
            bool encryption = false;

        #if defined(ESP8266)
            if (_wifiSSIDs[i].encryptionType != ENC_TYPE_NONE)
        #else
            if (_wifiSSIDs[i].encryptionType != WIFI_AUTH_OPEN)
        #endif
            {
                encryption = true;
            }

            String row = JSONUtils::EncloseObject(
                            JSONUtils::Pair(F("SSID"), _wifiSSIDs[i].SSID, true) +
                            JSONUtils::Pair(F("Encryption"), encryption) +
                            JSONUtils::Pair(F("Quality"), String(rssiQ))
                     );

            if (!jsonStr.isEmpty())
            {
                jsonStr += JSONUtils::ItemSeparator();
            }

            jsonStr += row;
            delay(0);
        }
        else
        {
            LOGDEBUG(F("Skipping due to quality"));
        }
    }

    jsonStr = JSONUtils::EncloseObject(JSONUtils::Pair(F("Access_Points"), jsonStr, true));

    ESPAsync_WiFiManagerUtils::responseApplJson(request, jsonStr);

    LOGDEBUG(F("Sent WiFiScan Data in Json format"));
}

//////////////////////////////////////////

// Handle the reset page
void ESPAsync_WiFiManager::handleReset(AsyncWebServerRequest *request)
{
    LOGDEBUG(F("Reset"));

    // Redirect browser to root so it is no longer on /r when the device reboots.
    // Without this the browser retries /r after reconnect, causing an infinite restart loop.
    request->redirect("/");

    delay(2000);

#ifdef ESP8266
    ESP.reset();
#else   //ESP32
    ESP.restart();
#endif
}

void ESPAsync_WiFiManager::handleOTAUpdateStart(AsyncWebServerRequest *pRequest)
{
    // Get file MD5 hash from arg
    if (pRequest->hasParam("hash"))
    {
        String hash = pRequest->getParam("hash")->value();
        LOGDEBUG(String("MD5: "+hash+"\n").c_str());
        if (_hardwareId)
        {
            if (!pRequest->hasParam("hwid"))
            {
                LOGDEBUG(F("ERROR: HWID missing\n"));
                return pRequest->send(400, "text/plain", "Missing parameter!");
            }

            String hwid = pRequest->getParam("hwid")->value();
            LOGDEBUG(String("HWID: "+hwid+"\n").c_str());
            String current_hwid = String(_hardwareId);
            if (hwid != current_hwid)
            {
                LOGDEBUG(F("ERROR: HWID not valid\n"));
                return pRequest->send(200, "text/plain", "HWID is not valid. Selected firmware is not for this device!");
            }
        }
        else
        {
            LOGDEBUG(F("ERROR: HWID missing\n"));
            return pRequest->send(400, "text/plain", "Missing parameter!");
        }

        if (!Update.setMD5(hash.c_str()))
        {
            LOGDEBUG(F("ERROR: MD5 hash not valid\n"));
            return pRequest->send(400, "text/plain", "MD5 parameter is invalid!");
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

void ESPAsync_WiFiManager::handleNotFound(AsyncWebServerRequest *pRequest)
{
    if (captivePortal(pRequest))
    {
        // If captive portal redirect instead of displaying the error page.
        return;
    }

    if (handleStaticFileRequest(pRequest))
    {
        // If static file request handled, return.
        return;
    }

    String message = "File Not Found\n\n";

    message += "URI: ";
    message += pRequest->url();
    message += "\nMethod: ";
    message += (pRequest->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += pRequest->args();
    message += "\n";

    for (uint8_t i = 0; i < pRequest->args(); i++)
    {
        message += " " + pRequest->argName(i) + ": " + pRequest->arg(i) + "\n";
    }

#if ( USING_ESP32_S2 || USING_ESP32_C3 )
    request->send(200, WM_HTTP_HEAD_CT, message);

    // Fix ESP32-S2 issue with WebServer (https://github.com/espressif/arduino-esp32/issues/4348)
    delay(1);
#else

    AsyncWebServerResponse *pResponse = pRequest->beginResponse( 404, WM_HTTP_HEAD_CT_TEXT_PLAIN, message );

    pResponse->addHeader(FPSTR(WM_HTTP_CACHE_CONTROL), FPSTR(WM_HTTP_NO_STORE));
    pResponse->addHeader(FPSTR(WM_HTTP_PRAGMA), FPSTR(WM_HTTP_NO_CACHE));
    pResponse->addHeader(FPSTR(WM_HTTP_EXPIRES), "-1");

    pRequest->send(pResponse);
#endif    // ( USING_ESP32_S2 || USING_ESP32_C3 )

    LOGWARN1(F("ESPAsync_WiFiManager::handleNotFound: "), message);
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

        AsyncWebServerResponse *response = request->beginResponse(302, WM_HTTP_HEAD_CT_TEXT_PLAIN, "");

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

inline void ESPAsync_WiFiManager::attachCustomHandlers(ArRequestFilterFunction /*filter*/)
{}

bool ESPAsync_WiFiManager::handleStaticFileRequest(AsyncWebServerRequest *pRequest)
{
    const static std::map<String, const HTTPResponseBlock*> handlerMap =
    {
        // HTML pages
        { "/ota", &gOTAHtml },
        { "/info", &gInfoHtml },
        { "/index", &gIndexHtml },
        { "/wifi", &gWiFiHtml },
    #ifdef WM_SUPPORT_HOME_ASSISTANT
        { "/mqtt", &gMQTTHtml },
    #endif
    
        // CSS styles
        { "/style.css", &gStyleCSS },

        // JS scripts
        { "/ota.js", &gOTAJS },
        { "/utils.js", &gUtilsJS },
        { "/md5_utils.js", &gMD5UtilsJS },
        { "/module_polyfill.js", &gModulePolyfillJS },
        { "/restart.js", &gRestartJS },
#ifdef WM_REMOTE_UPDATE
        { "/ota-remote.js", &gOTARemoteJS }
#endif
    };

    auto it = handlerMap.find(pRequest->url());
    if (it == handlerMap.end())
    {
        return false;
    }

    LOGDEBUG1(F("ESPAsync_WiFiManager::handleStaticFileRequest: ulr="), pRequest->url());

    ESPAsync_WiFiManagerUtils::responseText(pRequest, it->second);
    return true;
}

inline bool ESPAsync_WiFiManager::handleCustomSystemQuery(AsyncWebServerRequest *request)
{
    return false;
}

//////////////////////////////////////////

#ifdef WM_SUPPORT_TRAIN_CONTROL
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

        AsyncWebServerResponse *pResponse = pRequest->beginResponse(200, WM_HTTP_HEAD_CT_JSON, page);
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

    ESPAsync_WiFiManagerUtils::responseText(pRequest, &gTrainControlHtmlBlockResponse);
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
