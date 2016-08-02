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
 
#include <ESP8266WebServer.h>
#include "userdata.h"

// Web server
ESP8266WebServer server(80);
String webString="";

void handle_root()
{
  server.send(200, "text/plain", "Visit https://github.com/liberiot/ESP8266-mqtt-gateway for details");
  delay(100);
}

void initWebServer(void)
{
  server.on("/", handle_root);

  server.on("/setnet", []()
  {
    setNetwork();
  });

  server.on("/setusr", []()
  {
    setUserData();
  });
  
  server.on("/setgps", []()
  {
    setCoordinates();
  });
  
  server.begin();
}

/**
 * Handle HTTP requests
 */
void httpHandle(void)
{
  server.handleClient();
}

/**
 * setNetwork
 * 
 * Set WiFi settings
 */
void setNetwork(void)
{
  uint8_t i;
  webString = "Request not supported";
  
  if (server.args() == 2)
  {
    char value[64], buf[7];
  
    if (server.argName(0) == "ssid")
    {
      if (server.arg(0).length() > 0)
      {
        if (server.argName(1) == "pwd")
        {
          if (server.arg(1).length() > 0)
          {
            // Update global variables
            server.arg(0).toCharArray(config.ssid, sizeof(config.ssid));
            server.arg(1).toCharArray(config.password, sizeof(config.password));
            // Save config in EEPROM
            config.saveWifiConfig();            
            String str = "OK";
            server.send(200, "text/plain", str);
            return;
          }
        }
      }
    }
  }    

  server.send(200, "text/plain", webString);
}

/**
 * setUserData
 * 
 * Set User and gateways keys
 */
void setUserData(void)
{
  uint8_t i;
  webString = "Request not supported";
  
  if (server.args() == 1)
  {
    char value[64], buf[7];
  
    if (server.argName(0) == "userkey")
    {
      if (server.arg(0).length() > 0)
      {
        // Update global variables
        server.arg(0).toCharArray(config.userKey, sizeof(config.userKey));
        // Save config in EEPROM
        config.saveUserConfig();            
        String str = "OK";
        server.send(200, "text/plain", str);
        return;
      }
    }
  }    

  server.send(200, "text/plain", webString);
}


/**
 * setCoordinates
 * 
 * Set GPS position
 */
void setCoordinates(void)
{
  uint8_t i;
  webString = "Request not supported";
  
  if (server.args() == 2)
  {
    char value[64], buf[7];
  
    if (server.argName(0) == "lat")
    {
      if (server.arg(0).length() > 0)
      {
        if (server.argName(1) == "lon")
        {
          if (server.arg(1).length() > 0)
          {
            // Update global variables
            server.arg(0).toCharArray(config.latitude, sizeof(config.latitude));
            server.arg(1).toCharArray(config.longitude, sizeof(config.longitude));
            // Save config in EEPROM
            config.saveGpsConfig();            
            String str = "OK";
            server.send(200, "text/plain", str);
            return;
          }
        }
      }
    }
  }    

  server.send(200, "text/plain", webString);
}

