# ESPAsyncWiFiManager

Asynchronous WiFi connection manager for **ESP8266** and **ESP32**, forked from [khoih-prog/ESPAsync_WiFiManager](https://github.com/khoih-prog/ESPAsync_WiFiManager).

Provides a non-blocking captive portal for WiFi credential configuration, OTA firmware updates, NTP timezone setup, and extensible custom parameters — all served via `ESPAsyncWebServer`.

---

## Features

- Captive portal (AP mode) for WiFi credential entry and reconfiguration
- Stores up to **2 WiFi credentials** (primary + fallback) with automatic reconnection
- **Modal** (blocking) and **modeless** (non-blocking) portal modes
- Built-in **OTA firmware update** — local upload and remote manifest-based update
- **NTP timezone** auto-detection via browser JavaScript
- Extensible **custom parameter** system — add application fields to the portal form
- Customisable portal index and settings page buttons via PROGMEM hooks
- Optional **CORS** support and **static IP** configuration for both AP and STA modes
- Platform-unified API across ESP8266 and ESP32

---

## Dependencies

| Library | Purpose |
|---|---|
| `ESPAsyncWebServer` | Async HTTP server for the portal |
| `ESPAsyncDNSServer` | Captive portal DNS interception |
| `ESPAsyncTCP` / `AsyncTCP` | Async TCP layer (ESP8266 / ESP32) |
| `ESP_DoubleResetDetector` | Detect double-reset to trigger config portal |

---

## Quick start

```cpp
#include <ESPAsync_WiFiManager.h>

AsyncWebServer  server(80);
AsyncDNSServer  dns;

ESPAsync_WiFiManager wm(&server, &dns, "MyDevice");

void setup()
{
    // Blocks until connected or portal timeout
    wm.autoConnect("MyDevice-AP");
}

void loop()
{
    // nothing — connection is maintained automatically
}
```

For **modeless** (non-blocking) operation:

```cpp
wm.startConfigPortalModeless("MyDevice-AP", "password");

void loop()
{
    wm.loop();   // must be called regularly
    // your other code here
}
```

---

## Portal web endpoints

| Endpoint | Description |
|---|---|
| `/` | Main menu |
| `/wifi` | WiFi network selection and credential entry |
| `/wifisave` | Save WiFi credentials |
| `/settings` | OTA and advanced settings |
| `/i` | Device information |
| `/scan` | WiFi scan results (JSON) |
| `/sq` | System query (JSON) |
| `/r` | Reset settings and reboot |
| `/close` | Exit portal and connect in STA mode |
| `/ota/start` | OTA firmware update UI |
| `/ota/upload` | Firmware binary upload (POST) |
| `/ota/remote-check` | Check remote manifest for new firmware |
| `/ota/remote-start` | Start remote OTA update (POST) |
| `/ota/remote-progress` | Remote OTA progress (JSON) |

---

## API reference

### Connection

```cpp
bool autoConnect();
bool autoConnect(char const *apName, char const *apPassword = NULL);

bool startConfigPortal();
bool startConfigPortal(char const *apName, char const *apPassword = NULL);

void startConfigPortalModeless(char const *apName, char const *apPassword,
                               bool shouldConnectWiFi = true);

void handleSTA();   // set up STA-mode web routes after manual WiFi connect
void loop();        // must be called in loop() when using modeless mode
void resetSettings();
```

### Timeouts

```cpp
void setConfigPortalTimeout(const unsigned long& seconds);
void setConnectTimeout(const unsigned long& seconds);
```

### WiFi credentials

```cpp
void setCredentials(const char* ssid, const char* pwd,
                    const char* ssid1,  const char* pwd1);

String getSSID();           // primary SSID
String getPW();             // primary password
String getSSID1();          // fallback SSID
String getPW1();            // fallback password
String getSSID(uint8_t i);  // by index (0 or 1)
String getPW(uint8_t i);
```

### Static IP

```cpp
// Access Point
void setAPStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn);
void setAPStaticIPConfig(const WiFi_AP_IPConfig& cfg);
void getAPStaticIPConfig(WiFi_AP_IPConfig& cfg);

// Station
void setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn);
void setSTAStaticIPConfig(const WiFi_STA_IPConfig& cfg);
void getSTAStaticIPConfig(WiFi_STA_IPConfig& cfg);

// With DNS (requires USE_CONFIGURABLE_DNS)
void setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn,
                          const IPAddress& dns1, const IPAddress& dns2);
```

### Custom parameters

```cpp
bool addParameter(ESPAsync_WMParameter *p);   // returns false when full
ESPAsync_WMParameter** getParameters();
int  getParametersCount();
```

Example:

```cpp
ESPAsync_WMParameter mqttServer("mqtt_server", "MQTT Server", "192.168.1.10", 40);
wm.addParameter(&mqttServer);

// After portal save:
String value = mqttServer.getValue();
```

### Portal customisation

```cpp
void setCustomHeadElement(const char* html);      // inject into <head>
void setCustomIndexButtons(PGM_P html);           // extra buttons on index page
void setCustomSettingsButtons(PGM_P html);        // extra buttons on settings page
void setMinimumSignalQuality(const int& quality); // filter weak networks (default 8%)
void setRemoveDuplicateAPs(bool remove);
void setConfigPortalChannel(const int& channel);  // 0 = random
void setDebugOutput(bool debug);
```

### Callbacks

```cpp
// Called when AP mode is started
void setAPCallback(std::function<void(ESPAsync_WiFiManager*)> fn);

// Called when the user saves portal settings
void setSaveConfigCallback(std::function<void()> fn);

// OTA lifecycle
void onOTAStart(std::function<void()> fn);
void onOTAProgress(std::function<void(size_t current, size_t total)> fn);
void onOTAEnd(std::function<void(bool success)> fn);
void onPreReboot(std::function<void()> fn);
```

### Timezone / NTP

```cpp
String getTimezoneName();
void   setTimezoneName(const String& name);
const char* getTZ(const char* timezoneName);
```

Requires `USE_ESP_WIFIMANAGER_NTP` (enabled by default).

### Web server access

```cpp
AsyncWebServer* getWebServer();
void attachCustomHandlers(ArRequestFilterFunction filter);
```

---

## Compile-time flags

| Flag | Default | Description |
|---|---|---|
| `_ESPASYNC_WIFIMGR_LOGLEVEL_` | `1` | Log level: 0=off, 1=error, 2=warn, 3=info, 4=debug |
| `USE_CONFIGURABLE_DNS` | `false` | Enable manual DNS server fields |
| `USE_STATIC_IP_CONFIG_IN_CP` | `true` | Show static IP fields in portal |
| `DISPLAY_STORED_CREDENTIALS_IN_CP` | `true` | Pre-fill SSID/password in portal |
| `USE_ESP_WIFIMANAGER_NTP` | `true` | Enable NTP timezone selection |
| `USE_CLOUDFLARE_NTP` | `true` (ESP8266) | Load timezone JS from Cloudflare CDN |
| `USING_CORS_FEATURE` | `false` | Enable CORS response headers |
| `USE_DYNAMIC_PARAMS` | — | Enable dynamic `addParameter()` array growth |
| `WM_REMOTE_UPDATE` | — | Enable remote manifest-based OTA |
| `WM_SUPPORT_HOME_ASSISTANT` | — | Enable Home Assistant / MQTT integration |
| `LANGUAGE_EN_US` / `LANGUAGE_SK_SK` / `LANGUAGE_DE_DE` | `EN_US` | Portal UI language |
| `IOT_CHIP_NAME` | auto | Override chip name string |

---

## License

LGPL 2.1 — see [LICENSE](LICENSE) file.
