/**
 * Copyright (c) 2016 LIBERiot/panStamp <dberenguer@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 01/31/2016
 */

#include "userdata.h"
   
/**
 * readWifiConfig
 *
 * Read Wifi settings from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readWifiConfig(void)
{
  bool found = false;
  
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
 * readUserConfig
 *
 * Read user/gateway settings from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readUserConfig(void)
{
  bool found = false;
  
  // User key
  for(int i=0 ; i<LIBERIOT_KEY_LENGTH ; i++)
  {
    userKey[i] = EEPROM.read(EEPROM_USER_KEY + i);
    if (userKey[i] == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    found = false;
    // Gateway key
    for(int i=0 ; i<LIBERIOT_KEY_LENGTH ; i++)
    {
      gatewayKey[i] = EEPROM.read(EEPROM_GATEWAY_KEY + i);
      if (gatewayKey[i] == 0)
      {
        found = true;
        break;
      }
    }
  }
  
  return found;
}

/**
 * readGpsConfig
 *
 * Read GPS coordinates from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readGpsConfig(void)
{
  bool found = false;
  
  // WiFi network SSID
  for(int i=0 ; i<GPS_CCORDINATE_LENGTH ; i++)
  {
    latitude[i] = EEPROM.read(EEPROM_LATITUDE + i);
    if (latitude[i] == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    found = false;
    // WiFi password
    for(int i=0 ; i<GPS_CCORDINATE_LENGTH ; i++)
    {
      longitude[i] = EEPROM.read(EEPROM_LONGITUDE + i);
      if (longitude[i] == 0)
      {
        found = true;
        break;
      }
    }
  }
  
  return found;
}

/**
 * saveWifiConfig
 *
 * Save Wifi config in EEPROM
 */
void USERDATA::saveWifiConfig(void)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(ssid) ; i++)
    EEPROM.write(EEPROM_WIFI_SSID + i, ssid[i]);
  EEPROM.write(EEPROM_WIFI_SSID + i, 0);

  for(i=0 ; i<strlen(password) ; i++)
    EEPROM.write(EEPROM_WIFI_PWD + i, password[i]);
  EEPROM.write(EEPROM_WIFI_PWD + i, 0);

  EEPROM.commit();
}

/**
 * saveUserConfig
 *
 * Save user and gateway keys in EEPROM
 */
void USERDATA::saveUserConfig(void)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(userKey) ; i++)
    EEPROM.write(EEPROM_USER_KEY + i, userKey[i]);
  EEPROM.write(EEPROM_USER_KEY + i, 0);

  for(i=0 ; i<strlen(gatewayKey) ; i++)
    EEPROM.write(EEPROM_GATEWAY_KEY + i, gatewayKey[i]);
  EEPROM.write(EEPROM_GATEWAY_KEY + i, 0);

  EEPROM.commit();
}

/**
 * saveGpsConfig
 *
 * Save GPS coordinates of the gateway in EEPROM
 */
void USERDATA::saveGpsConfig(void)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(latitude) ; i++)
    EEPROM.write(EEPROM_LATITUDE + i, latitude[i]);
  EEPROM.write(EEPROM_LATITUDE + i, 0);

  for(i=0 ; i<strlen(longitude) ; i++)
    EEPROM.write(EEPROM_LONGITUDE + i, longitude[i]);
  EEPROM.write(EEPROM_LONGITUDE + i, 0);

  EEPROM.commit();
}

