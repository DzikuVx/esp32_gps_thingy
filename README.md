# ESP32 GPS Thingy

# Hardware

* ESP32 development board
* Serial GPS module with NMEA support
* OLED display connected to ESP32 

# Connections

## GPS:

* TX pin on GPS goes to ESP32 board pin 16 (Serial pin RX)
* RX pin on GPS goes to ESP32 board pin 17 (Serial pin TX)

## OLED:

* OLED SDA to pin 4
* OLED SCL to pin 15

# Dependencies

* https://github.com/espressif/arduino-esp32 at version 706bf48708e6668077187ba3f5543d0f7b49e132 . Version of 23cf233ecad1121d56a3a93adbb9de2e5889e665 is not working for unknown reason, will investigate
* TinyGPS++ library: http://arduiniana.org/libraries/tinygpsplus/ https://github.com/mikalhart/TinyGPSPlus
