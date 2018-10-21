#include "oled_display.h"
#include "Arduino.h"

OledDisplay::OledDisplay(SSD1306 *display) {
    _display = display;
}

void OledDisplay::init() {
    _display->init();
}

void OledDisplay::loop() {
    page(pageSequence[_mainPageSequenceIndex]);
}

void OledDisplay::nextPage() {
    _mainPageSequenceIndex++;
    if (_mainPageSequenceIndex == OLED_DISPLAY_PAGE_COUNT) {
        _mainPageSequenceIndex = 0;
    }
}

void OledDisplay::page(uint8_t page) {

    static uint32_t lastUpdate = 0;

    //Do not allow for OLED to be updated too often
    if (lastUpdate > 0 && millis() - lastUpdate < 200 && _forceDisplay == false) {
        return;
    }

    _forceDisplay = false;

    switch (page) {
        
        case OLED_PAGE_STATS:
            renderPageStats();
            break;
        case OLED_PAGE_SPEED:
            renderPageSpeed();
            break;
        case OLED_PAGE_ALTITUDE:
            renderPageAltitude();
        break;
    }
    _page = page;

    lastUpdate = millis();
}

void OledDisplay::renderPageStats() {
    _display->clear();
    _display->setFont(ArialMT_Plain_10);

    _display->drawString(0, 0, "Sat:");
    _display->drawString(26, 0, String(gps.satellites.value()));

    _display->drawString(0, 12, "Alt:");      
    _display->drawString(26, 12, String(gps.altitude.meters(), 1));
    if (altMax > -999999) {
        _display->drawString(64, 12, String(altMax, 1));        
    } else {
        _display->drawString(64, 12, "-");
    }

    _display->drawString(0, 24, "Spd:");
    _display->drawString(26, 24, String(gps.speed.mps(), 1));
    _display->drawString(64, 24, String(spdMax, 1)); 

    _display->drawString(0, 36, "Dst:");

    if (dist < 10000) {
        _display->drawString(26, 36, String(dist, 1));
    } else {
        _display->drawString(26, 36, String(dist / 1000, 1)); //in km
    }
    
    if (distMax < 10000) {
        _display->drawString(64, 36, String(distMax, 1));
    } else {
        _display->drawString(64, 36, String(distMax / 1000, 1)); //in km
    }

    _display->display();
}

void OledDisplay::renderHeader(String title) {
    _display->setFont(ArialMT_Plain_10);
    _display->drawString(0, 0, title);

    _display->drawString(90, 0, String(gps.satellites.value()) + " sats");
}

void OledDisplay::renderPageSpeed() {
    _display->clear();

    renderHeader("Speed");

    _display->setFont(ArialMT_Plain_24);

    _display->drawString(0, 20, String(gps.speed.kmph(), 1) + " km/h");

    _display->setFont(ArialMT_Plain_10);
    _display->drawString(0, 54, "Max speed " + String(spdMax, 1) + " km/h");

    _display->display();
}

void OledDisplay::renderPageAltitude() {
    _display->clear();

    renderHeader("Abs. Altitude");

    _display->setFont(ArialMT_Plain_24);
    _display->drawString(0, 20, String(gps.altitude.meters(), 1) + " m");

    _display->setFont(ArialMT_Plain_10);

    if (altMin < 999999) {
        _display->drawString(0, 54, "Min " + String(altMin, 1) + " m");    
    } else {
        _display->drawString(0, 54, "Min -");
    }

    if (altMax > -999999) {
        _display->drawString(64, 54, "Max " + String(altMax, 1) + " m");
    } else {
        _display->drawString(64, 54, "Max -");
    }

    _display->display();
}