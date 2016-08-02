# ESP8266-mqtt-gateway

LIBERiot MQTT gateway for the ESP8266 WiFi module. This application compiles on Arduino 1.6.5 or newer.

## RF interface

This application relies on a panStamp NRG module for the low-power wireless communications. This module runs gswapmodem and transmits any wireless packet received to the ESP8266. After receiving any wireless packet, the ESP8266 module transmits this packet in ASCII format to LIBERiot's MQTT broker and server.

GWAP is the protocol used between low-power wireless motes and gateways. This protocol is open and uses unique 12-byte addresses for each device. This makes each device uniquely identifiable around the globe.

## WiFi

Press the GPIO0 button to put the gateway in Access Point mode. After this connect to the new "LIBERiot WiFi gateway" WiFi network from a computer and send a HTTP GET request with the following format:

http://192.168.4.1/setnet?ssid=your_ssid&pwd=your_password

192.168.4.1 is the default IP address of the gateway when working in access point mode. Simply change "your_ssid" by your router's WiFi SSID and "your_password" by your router's WiFi password. Once done, restart the gateway.

## LIBERiot register

In order to register gateways and devices against LIBERiot each user needs a user key. User keys are automatically generated when creating an account from red.liberiot.org. After getting the user key, it needs to be entered into the gateway by means of the following HTTP GET command:

http://192.168.4.1/setusr?userkey=your_user_key

Users also need to set the location of the gateway as follows:

http://192.168.4.1/setgps?lat=your_latitude&lon=your_longitude

## MQTT

LIBERiot's MQTT broker address is mqtt.liberiot.org. Packets transmitted to this broker have to comply with the packet structure defined by the GWAP protocol.

## Links

* [panStamp NRG - Wiki page](https://github.com/panStamp/panstamp/wiki/panStamp%20NRG%202.-Technical%20details)
* [Global Wireless Abstract Protocol (GWAP)](https://github.com/liberiot/gwap)
* [GWAP modem application](https://github.com/panStamp/panstamp_sketches/tree/master/gwapmodem)

