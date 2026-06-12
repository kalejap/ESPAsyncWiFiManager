/*
  wm_sk_SK.h - localization for Slovak - Slovakia

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

#ifndef WM_SK_SK_H
#define WM_SK_SK_H

#define LANGUAGE_LCID 1051
// HTML (ISO 639-1) Language Code
#define L_HTML_LANGUAGE "sk"
#undef  IOT_LANGUAGE_STRING
#define IOT_LANGUAGE_STRING "sk-sk"

// General
#define L_GENERAL_PASSWORD "Heslo"
#define L_GENERAL_USER "Používateľ"
#define L_GENERAL_SETTINGS "Nastavenia"
#define L_GENERAL_OPTIONS "Voľby"
#define L_GENERAL_SAVE "Uložiť"
#define L_GENERAL_BACK "Späť"
#define L_GENERAL_REFRESH "Obnoviť"
#define L_GENERAL_YES "Áno"
#define L_GENERAL_NO  "Nie"
#define L_GENERAL_ON  "Zapnut\xc3\xa9"
#define L_GENERAL_OFF "Vypnut\xc3\xa9"
#define L_RESTART_IN_PROGRESS "Reštart prebieha..."

// Portal options page
#define L_SETTINGS_3DOTS "Nastavenia..."
#define L_INFORMATION_3DOTS "Informácie..."
#define L_WIFI_CONFIGURATION_3DOTS "Konfigurácia WiFi..."
#define L_MQTT_CONFIGURATION_3DOTS "Konfigurácia MQTT..."
#define L_FIRMWARE_UPDATE_3DOTS "Aktualizácia firmvéru..."
#define L_RESTART_3DOTS "Reštart..."
#define L_RESTART_CONFIRM "Chcete reštartovať zariadenie?"
#define L_EXIT "Koniec"

// Settings
#define L_CREDENTAILS_SAVED "Prihlasovacie údaje uložené"
#define L_CLOSE_SERVER "Zatvoriť server"
#define L_RESETTING "Reštartujem"


// WiFi Settings
#define L_WIFI_SSID "SSID"
#define L_WIFI_PASSWORD L_GENERAL_PASSWORD
#define L_WIFI_SSID1 "SSID1"
#define L_WIFI_CONFIGURATION     "Konfigurácia WiFi"
#define L_WIFI_PRIMARY_NETWORK   "Primárna sieť"
#define L_WIFI_SSID_1            "SSID 1"
#define L_WIFI_PASSWORD_1        "Heslo 1"
#define L_WIFI_SSID_2            "SSID 2"
#define L_WIFI_PASSWORD_2        "Heslo 2"
#define L_WIFI_ALT_NETWORK       "Alternatívna sieť (voliteľné)"
#define L_WIFI_OPEN_NETWORK_HINT "Pre otvorené siete nechajte heslo prázdne."
#define L_WIFI_SAVE              "Uložiť WiFi"

// MQTT Settings
#define L_MQTT_SERVER "Server"
#define L_MQTT_PORT "Port"
#define L_MQTT_USER L_GENERAL_USER
#define L_MQTT_PASSWORD "Heslo"
#define L_MQTT_CONFIGURATION "Konfigurácia MQTT"
#define L_MQTT_HOST          "MQTT Host"
#define L_MQTT_USERNAME      "Používateľské meno"
#define L_MQTT_BROKER_HINT   "Ak váš broker vyžaduje TLS alebo meno/heslo, vyplňte príslušné polia; inak ich nechajte prázdne."
#define L_MQTT_SAVE          "Uložiť MQTT"

// OTA Update
#define L_OTA_FIRMWARE_UPDATE "Aktualizácia firmvéru"
#define L_OTA_SETTINGS L_GENERAL_SETTINGS
#define L_OTA_SELECT_FILE "Vybrať súbor"
#define L_OTA_UPDATE_SUCESSFUL "Aktualizácia úspešná"
#define L_OTA_UNEXPECTED_ERROR "Neočakávaná chyba"
#define L_OTA_PLEASE_TRY_AGAIN "Skúste znova"
#define L_OTA_HARDWARE_ID "Hardware ID"
#define L_OTA_FIRMWARE_VERSION "Verzia firmvéru"
#define L_OTA_TITLE                  "OTA aktualizácia"
#define L_OTA_HEADING                "OTA aktualizácia firmvéru"
#define L_OTA_NO_FILE_CHOSEN         "Žiadny súbor nie je vybraný"
#define L_OTA_FILESIZE_LABEL         "Veľkosť súboru:"
#define L_OTA_FIRMWARE_VERSION_LABEL "Verzia firmvéru:"
#define L_OTA_HARDWARE_ID_LABEL      "Hardware ID:"
#define L_OTA_LANGUAGE_LABEL         "Jazyk:"
#define L_OTA_UPLOADING_FILE         "Nahrávam súbor."
#define L_OTA_UPLOAD_UPDATE          "Nahrať a aktualizovať"
#define L_OTA_STATUS_HINT            "Vyberte súbor firmvéru (.bin) a stlačte Nahrať a aktualizovať."
#define L_OTA_UPDATE_IN_PROGRESS     "Prebieha aktualizácia firmvéru.\\nNevypínajte zariadenie."

// ota_remote
#define L_OTA_REMOTE_HEADING         "Vzdialená aktualizácia"
#define L_OTA_REMOTE_URL_LABEL       "URL aktualizačného servera:"
#define L_OTA_REMOTE_CHECK_BTN       "Skontrolovať aktualizáciu"
#define L_OTA_REMOTE_STATUS_HINT     "Zadajte URL servera a stlačte Skontrolovať aktualizáciu."
#define L_OTA_REMOTE_CHECKING        "Kontrolujem..."
#define L_OTA_REMOTE_ERROR           "Chyba: "
#define L_OTA_REMOTE_CHECK_FAILED    "Kontrola zlyhala: "
#define L_OTA_REMOTE_UP_TO_DATE      "Firmvér je aktuálny. Verzia: "
#define L_OTA_REMOTE_NEW_VERSION     "Dostupná nová verzia: "
#define L_OTA_REMOTE_CURRENT         "Aktuálna: "
#define L_OTA_REMOTE_SWITCH_LANG     "Zmena jazyka (verzia "
#define L_OTA_REMOTE_LANGUAGE_LABEL  "Jazyk:"
#define L_OTA_REMOTE_UPDATE_BTN      "Aktualizovať"
#define L_OTA_REMOTE_START_FAILED    "Spustenie zlyhalo: "

// ota_script.js
#define L_OTA_JS_UPLOAD_FAILED "Nahrávanie zlyhalo"
#define L_OTA_JS_OTA_FAILED "Spustenie OTA procesu zlyhalo"
#define L_OTA_JS_UPLOAD_ONE_BIN_FILE "Môžete nahrať iba jeden súbor (.bin) naraz."
#define L_OTA_JS_UPLOAD_ONLY_BIN_FILES "Môžete nahrávať iba súbory (.bin)."
#define L_OTA_JS_SERVER_RETURNED_STATUS "Server vrátil stavový kód "
#define L_OTA_JS_ILEGAL_ARGUMENT "Neplatn\xc3\xbd argument "
#define L_OTA_JS_HWID_MISMATCH "Nezhodn\xc3\xa1 Hardware ID. Tento firmv\xc3\xa9r nie je pre toto zariadenie."

// Train control
#define L_TC_POWER "Napájanie"
#define L_TC_CAB1 "Kabína 1"
#define L_TC_CAB2 "Kabína 2"
#define L_TC_DIRECTION "Smer"
#define L_TC_EMERGENCY_STOP "Núdzové zastavenie"

// Device information
#define L_WIFI_INFORMATION "Informácie o WiFi"
#define L_DEVICE_INFO "Informácie o zariadení"
#define L_DEVICE_STATUS "Stav zariadenia"
#define L_INFO "Info"

// Chip info table
#define L_CHIP_INFO_NAME            "Názov"
#define L_CHIP_INFO_VALUE           "Hodnota"
#define L_CHIP_INFO_CHIP_ID         "Chip ID"
#define L_CHIP_INFO_FLASH_CHIP_ID   "Flash Chip ID"
#define L_CHIP_INFO_IDE_FLASH_SIZE  "IDE Flash Size"
#define L_CHIP_INFO_REAL_FLASH_SIZE "Skutočná veľkosť Flash"
#define L_CHIP_INFO_AP_IP           "IP prístupového bodu"
#define L_CHIP_INFO_AP_MAC          "MAC prístupového bodu"
#define L_CHIP_INFO_STA_IP          "IP stanice"
#define L_CHIP_INFO_STA_MAC         "MAC stanice"

// Device status table (root page)
#define L_STATUS_SENSOR  "Senzor"
#define L_STATUS_VALUE   "Hodnota"
#define L_STATUS_UNIT    "Jednotka"
#define L_STATUS_LOADING "Načítavam\xe2\x80\xa6"

// Application settings
#define L_APP_SETTINGS           "Nastavenia aplikácie"
#define L_TEMPERATURE_UNIT       "Jednotka teploty"
#define L_TEMPERATURE_CELSIUS    "Celsius (°C)"
#define L_TEMPERATURE_FAHRENHEIT "Fahrenheit (°F)"

#endif // WM_SK_SK_H
