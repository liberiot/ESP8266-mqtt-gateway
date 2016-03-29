# ESP8266-mqtt-gateway
LIBERiot MQTT gateway for the ESP8266 WiFi module. This application compiles on Arduino 1.6.5 or newer.

## RF interface

This application relies on a panSTamp NRG module for the low-power wireless communications. This
module runs gswapmodem and transmits any wireless packet received to the ESP8266. After
receiving any wireless packet, the ESP8266 module transmits this packet in ASCII format to LIBERiot's
MQTT broker and server.

GWAP is the protocol used between low-power wireless motes and gateways. This protocol is open
and uses unique 12-byte addresses for each device. This makes each devide uniquely identifiable around
the globe.

## WiFi

Change WiFi SSID and password from wifi.h according to your router settings.

LIBWERiot's MQTT broker address is mqtt.liberiot.org. Packets transmitted to this broker have to comply
with the packet structure defined by the GWAP protocol.

## Links

* [panStamp NRG - Wiki page](https://github.com/panStamp/panstamp/wiki/panStamp%20NRG%202.-Technical%20details)
* [Global Wireless Abstract Protocol (GWAP)](https://github.com/panStamp/gwap)

