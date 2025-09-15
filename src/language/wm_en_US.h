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

// General
#define L_GENERAL_PASSWORD "Password"
#define L_GENERAL_USER "User"
#define L_GENERAL_SETTINGS "Settings"
#define L_GENERAL_OPTIONS "Options"
#define L_GENERAL_SAVE "Save"

// Portal options page
#define L_SETTINGS_3DOTS "Settings..."
#define L_INFORMATION_3DOTS "Information..."
#define L_FIRMWARE_UPDATE_3DOTS "Firmware update..."
#define L_RESET "Reset..."
#define L_EXIT "Exit"

// Settings
#define L_CREDENTAILS_SAVED "Credentials Saved"
#define L_CLOSE_SERVER "Close Server"
#define L_RESETTING "Resetting"


// WiFi Settings
#define L_WIFI_SETTINGS "WiFi Settings"
#define L_WIFI_SSID "SSID"
#define L_WIFI_PASSWORD L_GENERAL_PASSWORD
#define L_WIFI_SSID1 "SSID1"

// MQTT Settings
#define L_MQTT_SETTINGS "MQTT Settings"
#define L_MQTT_SERVER L_GENERAL_PASSWORD
#define L_MQTT_PORT "Port"
#define L_MQTT_USER L_GENERAL_USER
#define L_MQTT_PASSWORD "Password"

// OTA Update
#define L_OTA_FIRMWARE_UPDATE "Firmware Update"
#define L_OTA_SETTINGS L_GENERAL_SETTINGS
#define L_OTA_SELECT_FILE "Select File"
#define L_OTA_UPDATE_SUCESSFUL "Update Successful"
#define L_OTA_GO_BACK "Go Back"
#define L_OTA_UNEXPECTED_ERROR "Unexpected Error"
#define L_OTA_PLEASE_TRY_AGAIN "Please try again"
#define L_OTA_HARDWARE_ID "Hardware ID"
#define L_OTA_FIRMWARE_VERSION "Firmware Version"

// Train control
#define L_TC_POWER "Power"
#define L_TC_CAB1 "Cab 1"
#define L_TC_CAB2 "Cab 2"
#define L_TC_DIRECTION "Direction"
#define L_TC_EMERGENCY_STOP "Emergency Stop"

// Device information
#define L_WIFI_INFORMATION "WiFi Information"
#define L_DEVICE_INFO "Device info"
#define L_INFO "Info"

#endif // WM_EN_US_H
