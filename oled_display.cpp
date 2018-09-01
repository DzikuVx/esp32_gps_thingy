#include "oled_display.h"

OledDisplay::OledDisplay(SSD1306 *display) {
    _display = display;
}

void OledDisplay::init() {
    _display->init();
}

void OledDisplay::loop() {
    page(pageSequence[_mainPageSequenceIndex]);
}

void OledDisplay::page(uint8_t page) {

    static uint32_t lastUpdate = 0;

    //Do not allow for OLED to be updated too often
    if (lastUpdate > 0 && millis() - lastUpdate < 200) {
        return;
    }

    switch (page) {
        
        case OLED_PAGE_STATS:
            renderPageStats();
            break;
        case OLED_PAGE_SPEED:
            renderPageSpeed();
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
    _display->drawString(64, 12, String(altMax, 1));        


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

void OledDisplay::renderPageSpeed() {

}