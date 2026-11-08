## Purpose

Enables the WiFiManager to persist MQTT broker connection settings (host, port, username, password) submitted through the MQTT configuration form, in both STA and captive-portal (AP) modes.

## ADDED Requirements

### Requirement: MQTT settings are saved on form submission
When the MQTT configuration form is submitted, the system SHALL read the current settings from non-volatile storage, apply the submitted field values, and persist them if any value has changed.

#### Scenario: Form submitted with new host
- **WHEN** a POST request to `/mqtt` is received with a non-empty `host` field that differs from the stored value
- **THEN** the new host is saved to NVS and the client is redirected to `/`

#### Scenario: No write when values are unchanged
- **WHEN** a POST request to `/mqtt` is received with field values identical to the currently stored values
- **THEN** no NVS write occurs and the client is redirected to `/`

#### Scenario: Partial form submission
- **WHEN** a POST request to `/mqtt` is received with only a subset of fields present
- **THEN** only the present fields are updated; absent fields retain their stored values

### Requirement: MQTT save handler is available in both AP and STA modes
The system SHALL register the `POST /mqtt` route in both captive-portal (AP) mode and station (STA) mode via the shared `attacheHandlers()` registration path.

#### Scenario: Save in STA mode
- **WHEN** the device is connected to WiFi (STA mode) and the user submits the MQTT form
- **THEN** settings are saved and the client is redirected to `/`

#### Scenario: Save in captive portal mode
- **WHEN** the device is running the captive portal (AP mode) and the user submits the MQTT form
- **THEN** settings are saved and the client is redirected to `/`

### Requirement: MQTTSettings and Settings classes are part of ESPAsyncWiFiManager
When `WM_SUPPORT_HOME_ASSISTANT` is defined, the system SHALL provide `Settings` (base class for NVS-backed persistent settings) and `MQTTSettings` (MQTT broker credentials) as part of the `ESPAsyncWiFiManager` library.

#### Scenario: MQTTSettings accessible from dependent libraries
- **WHEN** a project includes `ESPAsyncWiFiManager` with `WM_SUPPORT_HOME_ASSISTANT` defined
- **THEN** `MQTTSettings` and `Settings` are available for use without separately including `IoTApplication`
