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
 * Creation date: 01/09/2016
 */

#include "rfmodem.h"

#define OK_RECEIVED() (rxBuffer[0] == 'O' && rxBuffer[1] == 'K')

/**
 * config
 *
 * Configure modem
 *
 * @param channel RF chanel
 * @param netidH  Network ID, high byte
 * @param netidL  Network ID, low byte
 * @param address modem's device address
 */
bool RFMODEM::config(uint8_t channel, uint8_t netidH, uint8_t netidL, uint8_t address)
{
  bool res = false;

  Serial.flush();
  clearRxBuffer();

  // Enter command mode
  Serial.print("+++");
  while (!available());
  if (OK_RECEIVED())
  {
    delay(1000);
    clearRxBuffer();

    // Set RF channel
    Serial.print("ATCH=");
    Serial.println(channel, HEX);
    while (!available());
    if (OK_RECEIVED())
    {
      delay(1000);
      clearRxBuffer();

      // Set RF channel
      Serial.print("ATSW=");
      Serial.print(netidH, HEX);
      Serial.println(netidL, HEX);
      while (!available());
      if (OK_RECEIVED())
      {
        delay(1000);
        clearRxBuffer();

        // Set RF channel
        Serial.print("ATDA=");
        Serial.println(address, HEX);
        while (!available());
        if (OK_RECEIVED())
          res = true;
      }
    }
  }

  delay(1000);
  clearRxBuffer();

  // Leave command mode
  Serial.println("ATO");
  while (!available());
  if (!OK_RECEIVED())
    return false;

  return res;
}

/**
 * available
 *
 * Return true if a SWAP packet is waiting to be read from the buffer
 *
 * @return true if a paquet is waiting to be read. Return false otherwise
 */
bool RFMODEM::available(void)
{
  if (Serial.available() > 0)
  {
    uint8_t ch = Serial.read();

    if (rxLength >= sizeof(rxBuffer) - 1)
      clearRxBuffer();
    else if (ch == 0x0D) 
    {
      rxBuffer[rxLength] = 0;      
      return true;
    }
    else if (ch != 0x0A)
    {
      rxBuffer[rxLength] = ch;
      rxLength++;
    }
  }

  return false;
}

/**
 * check
 * 
 * Check integrity of packet received
 * 
 * @return True if check is OK
 */
bool RFMODEM::check(void)
{
  if (rxBuffer[0] == '(' && rxBuffer[5] == ')')
  {
    return true;
  }

  // Check NOK
  clearRxBuffer();
  return false;
}

