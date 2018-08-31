# ESP32 GPS Thingy

Quite simple ESP32 based device that allows to display basic GPS data on OLED screen and measure distance in a straight line. 

More about this device is available [here](https://www.youtube.com/watch?v=ebBtEi8srBc). If you want to know more about ESP32, you might want to [visit my website too](https://quadmeup.com/tag/esp32/).

# Hardware

* [ESP32 development board](http://bit.ly/2w98mvf)
* [Serial GPS module with NMEA support](http://bit.ly/2OxW5by)
* [OLED display connected to ESP32](http://bit.ly/2PTBaAX)

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
