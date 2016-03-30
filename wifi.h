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

#ifndef _WIFI_H
#define _WIFI_H

/**
 * Pseudo-EEPROM addresses
 */
#define EEPROM_MAX_PARAM_LENGTH  64
#define EEPROM_WIFI_SSID  0
#define EEPROM_WIFI_PWD   EEPROM_MAX_PARAM_LENGTH

/**
 * Wifi settings
 */
char ssid[EEPROM_MAX_PARAM_LENGTH];
char password[EEPROM_MAX_PARAM_LENGTH];

/**
 * Default Access Point settings
 */
const char apPassword[] = "liberiot";
const char apName[] = "LIBERiot WiFi gateway";
#endif

