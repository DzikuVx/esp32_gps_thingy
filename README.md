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

* [Arduino ESP32 core](https://github.com/espressif/arduino-esp32)
* [TinyGPS++ library](http://arduiniana.org/libraries/tinygpsplus/) or [GitHub](https://github.com/mikalhart/TinyGPSPlus)
* [QmuTactile library](https://github.com/DzikuVx/QmuTactile)
