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
 * Creation date: 03/16/2016
 */

#ifndef _RFMODEM_H
#define _RFMODEM_H

#include "Arduino.h"

#define SERIAL_BUFFER_LEN  132

/**
 * Class: RFMODEM
 * 
 * Description:
 * SWAP serial interface
 */
class RFMODEM
{
  private:
    /**
     * Serial Rx buffer
     */
    uint8_t rxBuffer[SERIAL_BUFFER_LEN];

    /**
     * Current length of serial packet
     */
    uint8_t rxLength;

    /**
     * strToHex
     *
     * @param buf Hex byte as a char array
     *
     * @return Hex byte
     */
    uint8_t charToHex(char *buf);

  public:
    /**
     * Class constructor
     */
    inline RFMODEM(void)
    {
      clearRxBuffer();
    }

    /**
     * Start modem
     *
     * @param speed Serial speed
     */
    inline void begin(uint16_t speed=38400)
    {
      Serial.begin(speed);
      Serial.setDebugOutput(0);
      Serial.println("");
      clearRxBuffer();
      delay(2000);
    }

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
    bool config(uint8_t channel, uint8_t netidH, uint8_t netidL, uint8_t address);

    /**
     * clearRxBuffer
     *
     * Clear Rx buffer
     */
    inline void clearRxBuffer(void)
    {
      memset(rxBuffer, 0, sizeof(rxBuffer));
      rxLength = 0;
    }

    /**
     * available
     *
     * Return true if a SWAP packet is waiting to be read from the buffer
     *
     * @return true if a paquet is waiting to be read. Return false otherwise
     */
    bool available(void);

    /**
     * check
     * 
     * Check integrity of packet received
     * 
     * @return True if check is OK
     */
    bool check(void);

    /**
     * read
     *
     * Return pointer to buffer containing the last packet received
     *
     * @param pointer to the buffer
     */
    inline void read(char *buf)
    {
      int i;
      
      for(i=0 ; i<rxLength ; i++)
        buf[i] = rxBuffer[i];
      buf[i] = 0;

      clearRxBuffer();
    }
    
    /**
     * write
     *
     * Send packet to serial modem
     *
     * @param buffer Buffer to be transmitted
     * @param len Buffer length
     *
     * @return amount of bytes transmitted
     */
    uint8_t write(char *buffer, uint8_t len);
};

#endif

