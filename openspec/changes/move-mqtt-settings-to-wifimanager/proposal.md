## Why

`ESPAsyncWiFiManager` serves `mqtt.html` (the MQTT broker configuration form) but has no POST handler for `/mqtt`, so submitted settings are never saved. The handler and the `MQTTSettings` persistence class currently live in `IoTApplication`, which is the wrong library — the WiFiManager owns the form and should own the full request lifecycle for it. Moving everything here also makes the MQTT save handler available in captive-portal (AP) mode automatically, since `attacheHandlers()` is called for both AP and STA modes.

## What Changes

- Move `Settings.h` / `Settings.cpp` from `IoTApplication` to `ESPAsyncWiFiManager/src/`
- Move `MQTTSettings.h` / `MQTTSettings.cpp` from `IoTApplication` to `ESPAsyncWiFiManager/src/`
- Add `handleMQTTSave(AsyncWebServerRequest*)` to `ESPAsync_WiFiManager` (declaration in `.hpp`, implementation in `-Impl.h`)
- Register `POST /mqtt → handleMQTTSave` in `attacheHandlers()` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`
- Remove the `/mqtt` POST workaround from `IoTApplication::registerCommonRoutes()`
- Remove `MQTTSettings.h` include and usage from `IoTApplication.cpp` where it was wired to the workaround handler (the `dx=mqtt` system query path remains unchanged)

## Capabilities

### New Capabilities

- `mqtt-config-save`: WiFiManager persists MQTT broker settings (host, port, user, password) when the MQTT configuration form is submitted via POST `/mqtt`, in both STA and AP (captive portal) modes.

### Modified Capabilities

_(none — no existing spec requirements are changing)_

## Impact

- **ESPAsyncWiFiManager**: gains `Settings.h`, `Settings.cpp`, `MQTTSettings.h`, `MQTTSettings.cpp`; gains `handleMQTTSave()` method and `POST /mqtt` route registered in `attacheHandlers()`.
- **IoTApplication**: loses `MQTTSettings.h`, `MQTTSettings.cpp`; removes `/mqtt` POST handler from `registerCommonRoutes()`. `AppSettings`, `GeoSettings`, and `WiFiSettings` continue to include `Settings.h` — PlatformIO's `lib_ldf_mode = deep+` resolves it from `ESPAsyncWiFiManager/src/` transitively.
- **No API changes** to callers of `MQTTSettings` in `IoTApplication.cpp` (class interface is identical, only the header location changes).
- **No breaking changes** to existing `attacheHandlers()` callers — the new route is additive and guarded by `WM_SUPPORT_HOME_ASSISTANT`.
