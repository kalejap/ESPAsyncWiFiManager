/*
  wm_en_US.h - localization for English - United States

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

#pragma once

#ifndef WM_EN_US_H
#define WM_EN_US_H

#define LANGUAGE_LCID 2057
// HTML (ISO 639-1) Language Code
#define L_HTML_LANGUAGE "en"
#undef  IOT_LANGUAGE_STRING
#define IOT_LANGUAGE_STRING "en-us"

// General
#define L_GENERAL_PASSWORD "Password"
#define L_GENERAL_USER "User"
#define L_GENERAL_SETTINGS "Settings"
#define L_GENERAL_OPTIONS "Options"
#define L_GENERAL_SAVE "Save"
#define L_GENERAL_BACK "Back"
#define L_GENERAL_REFRESH "Refresh"
#define L_GENERAL_YES "Yes"
#define L_GENERAL_NO  "No"
#define L_GENERAL_ON  "On"
#define L_GENERAL_OFF "Off"
#define L_RESTART_IN_PROGRESS "Restart in progress..."

// Portal options page
#define L_SETTINGS_3DOTS "Settings..."
#define L_INFORMATION_3DOTS "Information..."
#define L_WIFI_CONFIGURATION_3DOTS "WiFi Configuration..."
#define L_MQTT_CONFIGURATION_3DOTS "MQTT Configuration..."
#define L_FIRMWARE_UPDATE_3DOTS "Firmware update..."
#define L_RESTART_3DOTS "Restart..."
#define L_RESTART_CONFIRM "Do you want to restart the device?"
#define L_EXIT "Exit"

// Settings
#define L_CREDENTAILS_SAVED "Credentials Saved"
#define L_CLOSE_SERVER "Close Server"
#define L_RESETTING "Resetting"


// WiFi Settings
#define L_WIFI_SSID "SSID"
#define L_WIFI_PASSWORD L_GENERAL_PASSWORD
#define L_WIFI_SSID1 "SSID1"
#define L_WIFI_CONFIGURATION     "WiFi Configuration"
#define L_WIFI_PRIMARY_NETWORK   "Primary Network"
#define L_WIFI_SSID_1            "SSID 1"
#define L_WIFI_PASSWORD_1        "Password 1"
#define L_WIFI_SSID_2            "SSID 2"
#define L_WIFI_PASSWORD_2        "Password 2"
#define L_WIFI_ALT_NETWORK       "Alternative Network (Optional)"
#define L_WIFI_OPEN_NETWORK_HINT "Leave password empty for open networks."
#define L_WIFI_SAVE              "Save WiFi"

// MQTT Settings
#define L_MQTT_SERVER "Server"
#define L_MQTT_PORT "Port"
#define L_MQTT_USER L_GENERAL_USER
#define L_MQTT_PASSWORD "Password"
#define L_MQTT_CONFIGURATION "MQTT Configuration"
#define L_MQTT_HOST          "MQTT Host"
#define L_MQTT_USERNAME      "Username"
#define L_MQTT_BROKER_HINT   "If your broker requires TLS or a username/password, fill the fields; otherwise leave blank."
#define L_MQTT_SAVE          "Save MQTT"

// OTA Update
#define L_OTA_FIRMWARE_UPDATE "Firmware Update"
#define L_OTA_SETTINGS L_GENERAL_SETTINGS
#define L_OTA_SELECT_FILE "Select File"
#define L_OTA_UPDATE_SUCESSFUL "Update Successful"
#define L_OTA_UNEXPECTED_ERROR "Unexpected Error"
#define L_OTA_PLEASE_TRY_AGAIN "Please try again"
#define L_OTA_HARDWARE_ID "Hardware ID"
#define L_OTA_FIRMWARE_VERSION "Firmware Version"
#define L_OTA_TITLE                  "OTA Update"
#define L_OTA_HEADING                "Firmware OTA Update"
#define L_OTA_NO_FILE_CHOSEN         "No file chosen"
#define L_OTA_FILESIZE_LABEL         "Filesize:"
#define L_OTA_FIRMWARE_VERSION_LABEL "Firmware version:"
#define L_OTA_HARDWARE_ID_LABEL      "Hardware ID:"
#define L_OTA_LANGUAGE_LABEL         "Language:"
#define L_OTA_UPLOADING_FILE         "Uploading file."
#define L_OTA_UPLOAD_UPDATE          "Upload & Update"
#define L_OTA_STATUS_HINT            "Select a firmware file (.bin) and press Upload & Update."
#define L_OTA_UPDATE_IN_PROGRESS     "Firmware update in progress.\\nDo not turn off the device."

// ota_remote
#define L_OTA_REMOTE_HEADING         "Remote Update"
#define L_OTA_REMOTE_URL_LABEL       "Update server URL:"
#define L_OTA_REMOTE_CHECK_BTN       "Check for Update"
#define L_OTA_REMOTE_STATUS_HINT     "Enter the update server URL and press Check for Update."
#define L_OTA_REMOTE_CHECKING        "Checking..."
#define L_OTA_REMOTE_ERROR           "Error: "
#define L_OTA_REMOTE_CHECK_FAILED    "Check failed: "
#define L_OTA_REMOTE_UP_TO_DATE      "Firmware is up to date. Version: "
#define L_OTA_REMOTE_NEW_VERSION     "New version available: "
#define L_OTA_REMOTE_CURRENT         "Current: "
#define L_OTA_REMOTE_SWITCH_LANG     "Switch language (version "
#define L_OTA_REMOTE_LANGUAGE_LABEL  "Language:"
#define L_OTA_REMOTE_UPDATE_BTN      "Update"
#define L_OTA_REMOTE_START_FAILED    "Start failed: "

// ota_script.js
#define L_OTA_JS_UPLOAD_FAILED "Upload failed"
#define L_OTA_JS_OTA_FAILED "Start OTA process failed"
#define L_OTA_JS_UPLOAD_ONE_BIN_FILE "You can only upload one (.bin) file at a time."
#define L_OTA_JS_UPLOAD_ONLY_BIN_FILES "You can only upload (.bin) files."
#define L_OTA_JS_SERVER_RETURNED_STATUS "Server returned status code "
#define L_OTA_JS_ILEGAL_ARGUMENT "Illegal argument "
#define L_OTA_JS_HWID_MISMATCH "Hardware ID mismatch. This firmware is not for this device."

// Train control
#define L_TC_POWER "Power"
#define L_TC_CAB1 "Cab 1"
#define L_TC_CAB2 "Cab 2"
#define L_TC_DIRECTION "Direction"
#define L_TC_EMERGENCY_STOP "Emergency Stop"

// Device information
#define L_WIFI_INFORMATION "WiFi Information"
#define L_DEVICE_INFO "Device Info"
#define L_DEVICE_STATUS "Device Status"
#define L_INFO "Info"

// Chip info table
#define L_CHIP_INFO_NAME            "Name"
#define L_CHIP_INFO_VALUE           "Value"
#define L_CHIP_INFO_CHIP_ID         "Chip ID"
#define L_CHIP_INFO_FLASH_CHIP_ID   "Flash Chip ID"
#define L_CHIP_INFO_IDE_FLASH_SIZE  "IDE Flash Size"
#define L_CHIP_INFO_REAL_FLASH_SIZE "Real Flash Size"
#define L_CHIP_INFO_AP_IP           "Access Point IP"
#define L_CHIP_INFO_AP_MAC          "Access Point MAC"
#define L_CHIP_INFO_STA_IP          "Station IP"
#define L_CHIP_INFO_STA_MAC         "Station MAC"

// Device status table (root page)
#define L_STATUS_SENSOR  "Sensor"
#define L_STATUS_VALUE   "Value"
#define L_STATUS_UNIT    "Unit"
#define L_STATUS_LOADING "Loading\xe2\x80\xa6"

// Application settings
#define L_APP_SETTINGS           "Application Settings"
#define L_TEMPERATURE_UNIT       "Temperature unit"
#define L_TEMPERATURE_CELSIUS    "Celsius (\xc2\xb0""C)"
#define L_TEMPERATURE_FAHRENHEIT "Fahrenheit (\xc2\xb0""F)"


#endif // WM_EN_US_H
