#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "EEPROM.h"

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

#define PIN_BUTTON 0
#define EEPROM_SIZE 8

double originLat = 0;
double originLon = 0; 

void setup() {
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    while (!EEPROM.begin(EEPROM_SIZE)) {
        true;
    }

    long readValue; 
    EEPROM_readAnything(0, readValue);
    originLat = (double)readValue / 1000000;

    EEPROM_readAnything(4, readValue);
    originLon = (double)readValue / 1000000;
}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return i;
}

void loop() {
    uint8_t button = digitalRead(PIN_BUTTON);
    static uint8_t buttonPrevious;

    Serial.println(originLat, 6);
    Serial.println(originLon, 6);

    if (button == LOW && buttonPrevious == HIGH) {
        originLat = gps.location.lat();
        originLon = gps.location.lng();

        long writeValue;
        writeValue = originLat * 1000000;
        EEPROM_writeAnything(0, writeValue);
        writeValue = originLon * 1000000;
        EEPROM_writeAnything(4, writeValue);
        EEPROM.commit();
    }

    buttonPrevious = button;

    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }

    Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
    Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");  Serial.println(gps.altitude.meters());
    Serial.print("Sats=");  Serial.println(gps.satellites.value());
    Serial.print("DST: ");

    double dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), originLat, originLon);
    Serial.println(dist,1);

    delay(300);
}