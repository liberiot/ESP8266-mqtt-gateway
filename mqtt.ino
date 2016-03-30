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

#include <PubSubClient.h>

/**
 * MQTT connection
 */
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define MAX_MQTT_TOPIC_LENGTH  128   // Max topic length

// Last time this gateway sent a periodic heartbeat
uint32_t hBeatTime = 0;
const uint32_t hBeatPeriod = 60000; // Transmit heartbeat every 60 sec

const char TOPIC_NETWORK[] = "liberiot/panStamp/workcenter/network";
const char TOPIC_CONTROL[] = "panStamp/control";
const char TOPIC_GATEWAY[] = "liberiot/panStamp/workcenter/gateway";

// MQTT client
PubSubClient client(espClient);

void mqttConnect(void)
{
  //client.setServer(mqtt_server, 1883); // Connect to MQTT broker
  client.setServer("mqtt.liberiot.org", 3001); // Connect to MQTT broker
  
  client.setCallback(mqttReceive);     // Call this function whenever a MQTT message is received
}

void mqttReconnect()
{
  // Skip whilst in WiFi AP mode
  if (apMode)
    return;
    
  // Loop until we're reconnected
  while (!client.connected())
  {
    #ifdef DEBUG_ENABLED
    Serial.print("Attempting MQTT connection...");
    #endif
    
    // Attempt to connect
    if (client.connect(description)) // Anonymous connection to broker
    //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) // Authenticated connection with broker
    {
      #ifdef DEBUG_ENABLED
      Serial.println("connected");
      #endif
      
      // Append "/#" at the end of the topic
      char topic[sizeof(TOPIC_CONTROL) + 2];
      sprintf(topic, "%s/#", TOPIC_CONTROL);

      // Subscribe to the main topic
      client.subscribe(topic);      
    }
    else
    {
      #ifdef DEBUG_ENABLED
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      
      // Wait 5 seconds before retrying
      digitalWrite(LED1, !digitalRead(LED1));
      delay(2500);
      digitalWrite(LED1, !digitalRead(LED1));
      delay(2500);
    }    
  }
  digitalWrite(LED1, LOW);
}

/**
 * mqttHandle
 * 
 * Handle MQTT client connectivity
 */
void mqttHandle(void)
{
  if (!client.connected())
    mqttReconnect();

  uint32_t currentTime = millis();
  if (currentTime - hBeatTime > hBeatPeriod)
  {
    client.publish(TOPIC_GATEWAY, "Running");
    hBeatTime = currentTime;
  }
    
  client.loop();
}

/**
 * mqttSend
 * 
 * Send MQTT message
 * 
 * @param Raw buffer received via RF
 */
void mqttSend(char *buffer)
{
  int i;
  char addr[13];
  char topic[MAX_MQTT_TOPIC_LENGTH];

  // Device address 
  for(i=0 ; i<24 ; i++)
    addr[i] = buffer[i+6];
  addr[i] = 0;

  // Message topic
  sprintf(topic, "%s/%s", TOPIC_NETWORK, addr);

  // Send MQTT message
  client.publish(topic, buffer);
}

/**
 * mqttRgbReceive
 * 
 * Function called whenever a MQTT packet is received
 * 
 * @param topic MQTT topic
 * @param payload message
 * @param len message length
 */
void mqttReceive(char* topic, byte* payload, unsigned int len)
{  
  //char *ptr1 = topic + sizeof(TOPIC_CONTROL);

  modem.write((char*)payload, len/2);
}

/**
 * strToHex
 *
 * Convert char array to hexadecimal 8-bit value
 *
 * @param buf Hex byte as a char array
 *
 * @return Hex byte
 */
uint8_t charToHex(char *buf)
{
  uint8_t res = 0;
  
  for (int i=0 ; i<2 ; i++)
  {
    res <<= 4;
    if (buf[i] >= 'A' && buf[i] <= 'F')
      res |= buf[i] - 55;
    else if (buf[i] >= 'a' && buf[i] <= 'f')
      res |= buf[i] - 87;
    else if (buf[i] >= '0' && buf[i] <= '9')
      res |= buf[i] - 48;
    else
      return 0;
  }

  return res;
}

