#include "types.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "EEPROM.h"
#include "QmuTactile.h"

#include <Wire.h>
#include "SSD1306.h"
#include "oled_display.h"

#define PIN_BUTTON 0

QmuTactile button0(PIN_BUTTON);

#define EEPROM_SIZE 128

#define OLED_ADDR 0x3C
#define OLED_SDA 4
#define OLED_SCL 15

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
SSD1306 display(OLED_ADDR, OLED_SDA, OLED_SCL);
OledDisplay oledDisplay(&display);

GpsDataState_t gpsState = {};

#define TASK_OLED_RATE 200
#define TASK_SERIAL_RATE 100

uint32_t nextSerialTaskTs = 0;
uint32_t nextOledTaskTs = 0;

void setup() {

    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
    button0.start();

    while (!EEPROM.begin(EEPROM_SIZE)) {
        true;
    }

    long readValue; 
    EEPROM_readAnything(0, readValue);
    gpsState.originLat = (double)readValue / 1000000;

    EEPROM_readAnything(4, readValue);
    gpsState.originLon = (double)readValue / 1000000;

    EEPROM_readAnything(8, readValue);
    gpsState.originAlt = (double)readValue / 1000000;

    oledDisplay.init();
    oledDisplay.page(OLED_PAGE_STATS);
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
    button0.loop();

    static int p0 = 0;

    // Store new origin point
    if (button0.getState() == TACTILE_STATE_LONG_PRESS) {
        gpsState.originLat = gps.location.lat();
        gpsState.originLon = gps.location.lng();
        gpsState.originAlt = gps.altitude.meters();

        long writeValue;
        writeValue = gpsState.originLat * 1000000;
        EEPROM_writeAnything(0, writeValue);
        writeValue = gpsState.originLon * 1000000;
        EEPROM_writeAnything(4, writeValue);
        writeValue = gpsState.originAlt * 1000000;
        EEPROM_writeAnything(8, writeValue);
        EEPROM.commit();

        gpsState.distMax = 0;
        gpsState.altMax = -999999;
        gpsState.spdMax = 0;
        gpsState.altMin = 999999;
    } else if (button0.getState() == TACTILE_STATE_SHORT_PRESS) {
        oledDisplay.nextPage();
    } 

    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }

    if (gps.satellites.value() > 4) {
        gpsState.dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), gpsState.originLat, gpsState.originLon);

        if (gpsState.dist > gpsState.distMax && abs(gpsState.prevDist - gpsState.dist) < 50) {
            gpsState.distMax = gpsState.dist;
        }

        gpsState.prevDist = gpsState.dist;

        if (gps.altitude.meters() > gpsState.altMax) {
            gpsState.altMax = gps.altitude.meters();
        }

        if (gps.speed.mps() > gpsState.spdMax) {
            gpsState.spdMax = gps.speed.mps();
        }

        if (gps.altitude.meters() < gpsState.altMin) {
            gpsState.altMin = gps.altitude.meters();
        }
    }

    if (nextSerialTaskTs < millis()) {

        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
        Serial.print("ALT=");  Serial.println(gps.altitude.meters());
        Serial.print("Sats=");  Serial.println(gps.satellites.value());
        Serial.print("DST: ");
        Serial.println(gpsState.dist,1);

        nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
    }

    oledDisplay.loop();

}