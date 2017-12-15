#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "EEPROM.h"

#include <Wire.h>
#include "SSD1306.h"

#define PIN_BUTTON 0
#define EEPROM_SIZE 8

#define OLED_ADDR 0x3C
#define OLED_SDA 4
#define OLED_SCL 15

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
SSD1306  display(OLED_ADDR, OLED_SDA, OLED_SCL);

double originLat = 0;
double originLon = 0; 
double distMax = 0;
double altMax = 0;
double spdMax = 0;

double prevDist = 0;

#define TASK_OLED_RATE 200
#define TASK_SERIAL_RATE 500

uint32_t nextSerialTaskTs = 0;
uint32_t nextOledTaskTs = 0;

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

    display.init();

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

    // Store new origin point
    if (button == LOW && buttonPrevious == HIGH) {
        originLat = gps.location.lat();
        originLon = gps.location.lng();

        long writeValue;
        writeValue = originLat * 1000000;
        EEPROM_writeAnything(0, writeValue);
        writeValue = originLon * 1000000;
        EEPROM_writeAnything(4, writeValue);
        EEPROM.commit();

        distMax = 0;
        altMax = 0;
        spdMax = 0;
    }

    buttonPrevious = button;

    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }

    double dist = 0;

    if (gps.satellites.value() > 4) {
        dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), originLat, originLon);

        if (dist > distMax && abs(prevDist - dist) < 50) {
            distMax = dist;
        }

        prevDist = dist;

        if (gps.altitude.meters() > altMax) {
            altMax = gps.altitude.meters();
        }

        if (gps.speed.mps() > spdMax) {
            spdMax = gps.speed.mps();
        }
    }

    if (nextSerialTaskTs < millis()) {

        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
        Serial.print("ALT=");  Serial.println(gps.altitude.meters());
        Serial.print("Sats=");  Serial.println(gps.satellites.value());
        Serial.print("DST: ");
        Serial.println(dist,1);

        nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
    }

    if (nextOledTaskTs < millis()) {

        display.clear();
        display.setFont(ArialMT_Plain_10);

        display.drawString(0, 0, "Sat:");
        display.drawString(26, 0, String(gps.satellites.value()));

        display.drawString(0, 12, "Alt:");      
        display.drawString(26, 12, String(gps.altitude.meters(), 1));
        display.drawString(64, 12, String(altMax, 1));        


        display.drawString(0, 24, "Spd:");
        display.drawString(26, 24, String(gps.speed.mps(), 1));
        display.drawString(64, 24, String(spdMax, 1)); 

        display.drawString(0, 36, "Dst:");

        if (dist < 10000) {
            display.drawString(26, 36, String(dist, 1));
        } else {
            display.drawString(26, 36, String(dist / 1000, 1)); //in km
        }
        
        if (distMax < 10000) {
            display.drawString(64, 36, String(distMax, 1));
        } else {
            display.drawString(64, 36, String(distMax / 1000, 1)); //in km
        }

        display.display();

        nextOledTaskTs = millis() + TASK_OLED_RATE;
    }

}