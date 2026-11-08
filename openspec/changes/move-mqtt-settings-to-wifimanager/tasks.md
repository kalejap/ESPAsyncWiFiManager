## 1. Move Settings and MQTTSettings into ESPAsyncWiFiManager

- [x] 1.1 Copy `Settings.h` from `IoTApplication/src/` to `ESPAsyncWiFiManager/src/`
- [x] 1.2 Copy `Settings.cpp` from `IoTApplication/src/` to `ESPAsyncWiFiManager/src/`
- [x] 1.3 Copy `MQTTSettings.h` from `IoTApplication/src/` to `ESPAsyncWiFiManager/src/`
- [x] 1.4 Copy `MQTTSettings.cpp` from `IoTApplication/src/` to `ESPAsyncWiFiManager/src/`
- [x] 1.5 Delete `Settings.h` and `Settings.cpp` from `IoTApplication/src/`
- [x] 1.6 Delete `MQTTSettings.h` and `MQTTSettings.cpp` from `IoTApplication/src/`

## 2. Add handleMQTTSave to ESPAsync_WiFiManager

- [x] 2.1 Add `#include "MQTTSettings.h"` in `ESPAsync_WiFiManager-Impl.h` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`
- [x] 2.2 Declare `void handleMQTTSave(AsyncWebServerRequest *request)` in `ESPAsync_WiFiManager.hpp` under `#ifdef WM_SUPPORT_HOME_ASSISTANT` (alongside other private handler declarations)
- [x] 2.3 Implement `handleMQTTSave` in `ESPAsync_WiFiManager-Impl.h`: read `MQTTSettings`, apply `host`/`port`/`user`/`pwd` args, save if dirty, redirect to `/`

## 3. Register POST /mqtt route

- [x] 3.1 In `attacheHandlers()` in `ESPAsync_WiFiManager-Impl.h`, add `_server->on("/mqtt", HTTP_POST, std::bind(&ESPAsync_WiFiManager::handleMQTTSave, this, _1)).setFilter(filterFn)` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`

## 4. Remove workaround from IoTApplication

- [x] 4.1 Remove the `POST /mqtt` handler block from `IoTApplication::registerCommonRoutes()` in `IoTApplication.cpp` (the `#ifdef WM_SUPPORT_HOME_ASSISTANT` block added as workaround)

## 5. Verify build

- [ ] 5.1 Build `SolidFuelBoilerController` and confirm no missing-header or multiple-definition errors
- [ ] 5.2 Flash to device and verify MQTT settings are saved when submitting the form in STA mode
- [ ] 5.3 Verify MQTT settings are saved when submitting the form in captive-portal (AP) mode
