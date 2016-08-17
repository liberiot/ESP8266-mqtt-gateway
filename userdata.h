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
 * Creation date: 03/31/2016
 */

#ifndef _USERDATA_H
#define _USERDATA_H

#include <EEPROM.h>

/**
 * MQTT connection
 */
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define MAX_MQTT_TOPIC_LENGTH  128   // Max topic length

/**
 * LIBERIOT constant definition
 */
#define LIBERIOT_KEY_LENGTH    32
#define GPS_CCORDINATE_LENGTH  24
const uint32_t hBeatPeriod = 60000; // Transmit heartbeat every 60 sec

/**
 * Pseudo-EEPROM addresses
 */
#define EEPROM_MAX_PARAM_LENGTH  64
#define EEPROM_WIFI_SSID  0
#define EEPROM_WIFI_PWD      EEPROM_WIFI_SSID + EEPROM_MAX_PARAM_LENGTH
#define EEPROM_USER_KEY      EEPROM_WIFI_PWD + EEPROM_MAX_PARAM_LENGTH
#define EEPROM_LATITUDE      EEPROM_USER_KEY + LIBERIOT_KEY_LENGTH
#define EEPROM_LONGITUDE     EEPROM_LATITUDE + GPS_CCORDINATE_LENGTH

/**
 * LIBERiot MQTT broker information
 */
const char LIBERIOT_MQTT_BROKER[] = "mqtt.liberiot.org";
const int LIBERIOT_MQTT_PORT = 3001;
const char LIBERIOT_TOPIC_MAIN[] = "liberiot";  // Main MQTT topic

/**
 * Default WiFi Access Point settings
 */
const char apPassword[] = "liberiot";
const char apName[] = "LIBERiot WiFi gateway";

/**
 * Class: USERDATA
 * 
 * Description:
 * USER config
 */
class USERDATA
{
  public:

    /**
     * Wifi settings
     */
    char ssid[EEPROM_MAX_PARAM_LENGTH];     // WiFi network SSID
    char password[EEPROM_MAX_PARAM_LENGTH]; // WiFi password

    /**
     * User and gateway identification
     */
    char userKey[LIBERIOT_KEY_LENGTH];      // Liberiot User key
    char gatewayKey[LIBERIOT_KEY_LENGTH];   // Liberiot gateway key

    /**
     * GPS coordinates of the gateway
     */
    char latitude[GPS_CCORDINATE_LENGTH];   // GPS latitude
    char longitude[GPS_CCORDINATE_LENGTH];  // GPS longitude

    /**
     * begin
     * 
     * Initialize EEPROM
     */
    inline void begin(void)
    {
      // Config pseudo-EEPROM space
      EEPROM.begin(512);
    }
    
    /**
     * readConfig
     * 
     * Read complete configuration settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    inline bool readConfig(void)
    {
      if (readWifiConfig())
      {
        if (readUserConfig())
        {
          if (readGpsConfig())
            return true;
        }
      }

      return false;
    }
    
    /**
     * readWifiCondif
     *
     * Read Wifi settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readWifiConfig(void);

    /**
     * readUserConfig
     *
     * Read user/gateway settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readUserConfig(void);

    /**
     * readGpsConfig
     *
     * Read GPS coordinates from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readGpsConfig(void);

    /**
     * saveWifiConfig
     *
     * Save Wifi config in EEPROM
     */
    void saveWifiConfig(void);

    /**
     * saveUserConfig
     *
     * Save user and gateway keys in EEPROM
     */
    void saveUserConfig(void);

    /**
     * saveGpsConfig
     *
     * Save GPS coordinates of the gateway in EEPROM
     */
    void saveGpsConfig(void);
};

#endif

