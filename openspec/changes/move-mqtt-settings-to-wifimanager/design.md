## Context

See `proposal.md — Why` for motivation. Key structural facts:

- `ESPAsync_WiFiManager::attacheHandlers(ArRequestFilterFunction)` is called once for AP mode (captive portal) and once for STA mode. Any route registered there is available in both modes automatically.
- `ESPAsync_WiFiManager.hpp` already guards MQTT-related members with `#ifdef WM_SUPPORT_HOME_ASSISTANT`.
- `IoTApplication` depends on `ESPAsyncWiFiManager`. PlatformIO with `lib_ldf_mode = deep+` adds all library `src/` directories to the compiler include path transitively — headers from `ESPAsyncWiFiManager/src/` are visible to `IoTApplication` source files.
- `Settings.h` is the base class for all persistent-settings classes in `IoTApplication` (`WiFiSettings`, `AppSettings`, `GeoSettings`, `MQTTSettings`). Moving it to `ESPAsyncWiFiManager` keeps `MQTTSettings` self-contained and avoids a fragile cross-library header reference.

## Goals / Non-Goals

**Goals:**
- `ESPAsync_WiFiManager` owns the full MQTT config request lifecycle (form serve + save).
- `Settings.h` / `MQTTSettings.h` ship with `ESPAsyncWiFiManager` under `WM_SUPPORT_HOME_ASSISTANT`.
- `POST /mqtt` route registered in `attacheHandlers()` — available in AP and STA modes with no extra wiring.
- `IoTApplication` builds unchanged after removing the two files and the route workaround.

**Non-Goals:**
- Reconnecting the MQTT client after settings are saved (requires a device restart; out of scope).
- Encrypting or hashing the MQTT password at rest.
- Moving `WiFiSettings`, `AppSettings`, or `GeoSettings` — those remain in `IoTApplication`.

## Decisions

### Move `Settings.h`/`Settings.cpp` together with `MQTTSettings`

**Decision:** Move both `Settings.h`/`Settings.cpp` and `MQTTSettings.h`/`MQTTSettings.cpp` to `ESPAsyncWiFiManager/src/`.

**Why:** `MQTTSettings` inherits from `Settings`. If only `MQTTSettings` moved, `ESPAsyncWiFiManager` would include `"Settings.h"` from `IoTApplication` — a cross-library header dependency with no formal declaration in `library.properties`. Moving `Settings.h` with it keeps the new home self-contained.

**Alternative considered:** Keep `Settings.h` in `IoTApplication` and rely on the transitive include path. Rejected because it creates an implicit, untracked dependency. Any project that uses `ESPAsyncWiFiManager` standalone (without `IoTApplication`) would fail to compile.

**Impact on `IoTApplication`:** `AppSettings`, `GeoSettings`, `WiFiSettings` all include `"Settings.h"`. Because `IoTApplication` depends on `ESPAsyncWiFiManager`, `ESPAsyncWiFiManager/src/` is on the compiler include path for `IoTApplication` source files — the existing `#include "Settings.h"` lines continue to resolve correctly without modification.

### Non-virtual `handleMQTTSave` in `ESPAsync_WiFiManager`

**Decision:** Implement `handleMQTTSave` as a regular (non-virtual) private method directly in `ESPAsync_WiFiManager`.

**Why:** The full implementation belongs in `ESPAsync_WiFiManager` — it uses `MQTTSettings` which now lives in the same library. No derived class needs to override it. The existing virtual-override pattern (`handleCustomSystemQuery`) was designed for cases where `IoTApplication` must supply context `ESPAsync_WiFiManager` can't have; that is not the case here.

**Alternative considered:** Virtual override in `IoTWiFiManager`. Rejected because it splits ownership — the route would still be in `ESPAsync_WiFiManager` but the save logic would remain in `IoTApplication`, defeating the purpose of the move.

### Route registered via `std::bind` in `attacheHandlers()`

**Decision:** Register `POST /mqtt` in `attacheHandlers()` using `std::bind(&ESPAsync_WiFiManager::handleMQTTSave, this, _1)`, mirroring how `/wifisave` is registered.

**Why:** Consistent with the existing pattern. `attacheHandlers()` is the single point where all routes are wired; adding here ensures the route respects the AP/STA filter and `onNotFound` ordering.

## Risks / Trade-offs

| Risk | Mitigation |
|------|------------|
| `AppSettings`/`GeoSettings`/`WiFiSettings` fail to find `Settings.h` after the move | Verified: PlatformIO `lib_ldf_mode = deep+` resolves headers from all library src dirs transitively. No include paths need changing. |
| `MQTTSettings.cpp` not compiled by PlatformIO (missed source file) | PlatformIO auto-discovers all `.cpp` files under `src/` — no manifest needed. |
| `IoTApplication.cpp` still `#include "MQTTSettings.h"` (used by `handleCustomSystemQuery`) — header must resolve from new location | Same transitive include path argument applies. No code change needed in `IoTApplication.cpp`. |

## Migration Plan

1. Copy `Settings.h` + `Settings.cpp` into `ESPAsyncWiFiManager/src/`.
2. Copy `MQTTSettings.h` + `MQTTSettings.cpp` into `ESPAsyncWiFiManager/src/`.
3. Delete `Settings.h`, `Settings.cpp`, `MQTTSettings.h`, `MQTTSettings.cpp` from `IoTApplication/src/`.
4. Add `handleMQTTSave` declaration to `ESPAsync_WiFiManager.hpp` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`.
5. Add `handleMQTTSave` implementation to `ESPAsync_WiFiManager-Impl.h` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`.
6. Add `#include "MQTTSettings.h"` to `ESPAsync_WiFiManager-Impl.h` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`.
7. Register `POST /mqtt → handleMQTTSave` in `attacheHandlers()` under `#ifdef WM_SUPPORT_HOME_ASSISTANT`.
8. Remove the `/mqtt` POST handler block from `IoTApplication::registerCommonRoutes()`.
9. Build the project and verify no missing-header or multiple-definition errors.

**Rollback:** Reverse steps 1–8. No data migration is required (NVS namespace and keys are unchanged).
