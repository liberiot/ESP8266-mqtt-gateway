/**
 * Copyright (c) 2016 LIBERiot/panStamp <dberenguer@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/16/2016
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include "wifi.h"
#include "rfmodem.h"

/**
 * Debug option
 */
#define DEBUG_ENABLED  1

/**
 * Modem reset line
 */
#define MODEM_RESET_LINE  15

/**
 * LED pins
 */
#define LED1    5
#define LED2    4

/**
 *  RF modem object
 */
RFMODEM modem;

/**
 * Description string
 */
const char* description = "MQTT-GWAP gateway";

// Wifi client
WiFiClient espClient;

// If true, enter Wifi AP mode
bool enterApMode = false;
bool apMode = false;

/**
 * enterWifiApMode
 * 
 * Enter WiFi AP mode
 */
void enterWifiApMode(void)
{
  enterApMode = true;
}

/**
 * Main setup function
 */
void setup()
{ 
  // Modem reset ESP pin
  pinMode(MODEM_RESET_LINE, OUTPUT);
  digitalWrite(MODEM_RESET_LINE, LOW);

  // ESP8266 GPIO0 pin
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(0, enterWifiApMode, FALLING);

  // Config LED outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Config pseudo-EEPROM space
  EEPROM.begin(512);

  #ifdef DEBUG_ENABLED
  Serial.begin(38400);
  #endif

  // Read Wifi config from EEPROM
  if (readConfigFromEeprom())
  {
    // Connect to WiFi network
    WiFi.begin(ssid, password);

    #ifdef DEBUG_ENABLED
    Serial.print("\n\r \n\rWorking to connect");
    #endif
  
    // Wait for connection
    while ((WiFi.status() != WL_CONNECTED) && !enterApMode)
    {
      digitalWrite(LED1, !digitalRead(LED1));
      delay(500);
  
      #ifdef DEBUG_ENABLED
      Serial.print(".");
      #endif
    }
    digitalWrite(LED1, LOW);

    if (WiFi.status() == WL_CONNECTED)
    {
      #ifdef DEBUG_ENABLED
      Serial.println("");
      Serial.println(apName);
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      #endif
    
      // Connect to MQTT server
      mqttConnect();
      mqttHandle();
    
      // Initialize web interface
      initWebServer();  
    
      #ifdef DEBUG_ENABLED
      Serial.print("Opening modem");
      #endif
    
      // Reset modem
      Serial.end();
      digitalWrite(MODEM_RESET_LINE, HIGH);
      delay(1000);
      Serial.begin(38400);
       
      // Start conversations with modem
      modem.begin();
    }
  }
  else
    enterApMode = true;
}

/**
 * Endless loop
 */
void loop()
{ 
  if (modem.available())
  {
    if (modem.check())
    {
      char buf[128];
      modem.read(buf);
      // Avoid packets broadcasted with other protocols
      if (buf[6] != '0' || buf[7] != '0')
      {
        // Skip too short frames
        if (strlen(buf) >= 34)
          mqttSend(buf);
      }
    }
  }
  else if (enterApMode)
  {
    #ifdef DEBUG_ENABLED
    Serial.println("Entering AP mode");
    #endif
    enterApMode = false;
    apMode = true;
    setupWiFiAP();
    initWebServer();
    digitalWrite(LED2, HIGH);
  }
  else
  {
    mqttHandle();
    httpHandle();
  }
}

/**
 * setupWiFiAP
 * 
 * Setup wifi access point
 */
void setupWiFiAP()
{
  WiFi.mode(WIFI_AP);
  
  // Append last two bytes of the MAC to the SSID
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  sprintf(ssid, "%s %X%X", apName, mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);
  WiFi.softAP(ssid, apPassword);
}

/**
 * readConfigFromEeprom
 *
 * Read EEPROM contents and set global variables
 * 
 * @return true if config found. Return false otherwise
 */
bool readConfigFromEeprom(void)
{
  bool found = false;
  
  // Read config from EEPROM
  // WiFi network SSID
  for(int i=0 ; i<EEPROM_MAX_PARAM_LENGTH ; i++)
  {
    ssid[i] = EEPROM.read(EEPROM_WIFI_SSID + i);
    if (ssid[i] == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    found = false;
    // WiFi password
    for(int i=0 ; i<EEPROM_MAX_PARAM_LENGTH ; i++)
    {
      password[i] = EEPROM.read(EEPROM_WIFI_PWD + i);
      if (password[i] == 0)
      {
        found = true;
        break;
      }
    }
  }
  
  return found;
}

/**
 * writeConfigToEeprom
 *
 * Write Wifi config in EEPROM
 * 
 * @param wSsid : WiFi SSID
 * @param wPwd : WiFi password
 */
void writeConfigToEeprom(char *wSsid, char *wPwd)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(wSsid) ; i++)
    EEPROM.write(EEPROM_WIFI_SSID + i, wSsid[i]);
  EEPROM.write(EEPROM_WIFI_SSID + i, 0);

  for(i=0 ; i<strlen(wPwd) ; i++)
    EEPROM.write(EEPROM_WIFI_PWD + i, wPwd[i]);
  EEPROM.write(EEPROM_WIFI_PWD + i, 0);

  EEPROM.commit();
}

