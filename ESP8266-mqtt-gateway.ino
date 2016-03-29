/**
 * Copyright (c) 2016 panStamp <contact@panstamp.com>
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

/**
 * Main setup function
 */
void setup()
{ 
  // Modem reset ESP pin
  pinMode(MODEM_RESET_LINE, OUTPUT);
  digitalWrite(MODEM_RESET_LINE, LOW);

  // Config LED outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  #ifdef DEBUG_ENABLED
  Serial.begin(38400);
  #endif
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);

  #ifdef DEBUG_ENABLED
  Serial.print("\n\r \n\rWorking to connect");
  #endif

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED1, !digitalRead(LED1));
    delay(500);

    #ifdef DEBUG_ENABLED
    Serial.print(".");
    #endif
  }
  digitalWrite(LED1, LOW);
  
  #ifdef DEBUG_ENABLED
  Serial.println("");
  Serial.println("esp-io Web Server");
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

/**
 * Endless loop
 */
void loop()
{
  mqttHandle();
  httpHandle();
  
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
}

