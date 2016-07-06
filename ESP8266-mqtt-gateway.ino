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

/**
 * Increase MQTT_MAX_PACKET_SIZE to 256 in PubSubClient.h
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include "userdata.h"
#include "rfmodem.h"

/**
 * Debug option
 */
//#define DEBUG_ENABLED  1

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
 * User config
 */
USERDATA config;

/**
 *  RF modem object
 */
RFMODEM modem;

// Wifi client
WiFiClient espClient;

// If true, enter Wifi AP mode
volatile bool enterApMode = false;
bool apMode = false;

/**
 * Description string
 */
char deviceId[32];

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

  // Initialize config (pseudo-EEPROM) space
  config.begin();

  // Append last two bytes of the MAC to the device ID
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  sprintf(deviceId, "%s %X%X", apName, mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);

  char buf[3];
  // Set Gateway ID from MAC address
  memset((uint8_t*)config.gatewayKey, 0, sizeof(config.gatewayKey));
  for(int i; i<WL_MAC_ADDR_LENGTH ; i++)
  {
    if (mac[i] < 0x10)
      sprintf(buf, "0%X", mac[i]);
    else
      sprintf(buf, "%X", mac[i]);

    config.gatewayKey[i*2] = buf[0];
    config.gatewayKey[i*2+1] = buf[1];
  }

  Serial.begin(38400);

  // Read config from EEPROM
  if (config.readConfig())
  {
    // Connect to WiFi network
    WiFi.begin(config.ssid, config.password);

    #ifdef DEBUG_ENABLED
    Serial.print("\n\r \n\rWorking to connect");
    #endif
  
    // Wait for connection
    while ((WiFi.status() != WL_CONNECTED) && (!enterApMode))
    {
      digitalWrite(LED1, !digitalRead(LED1));
      delay(500);
  
      #ifdef DEBUG_ENABLED
      Serial.print(".");
      #endif

      Serial.print("");  // Keep this blank character printing to make this loop work!!
    }

    digitalWrite(LED1, LOW);

    if (WiFi.status() == WL_CONNECTED)
    {
      #ifdef DEBUG_ENABLED
      Serial.println("");
      Serial.println(apName);
      Serial.print("Connected to ");
      Serial.println(config.ssid);
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
  WiFi.softAP(deviceId, apPassword);
}

