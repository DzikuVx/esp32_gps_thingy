#ifndef OLED_DISPLAY
#define OLED_DISPLAY

#include "SSD1306.h"
#include <TinyGPS++.h>

enum txOledPages {
    OLED_PAGE_NONE,
    OLED_PAGE_STATS,
    OLED_PAGE_SPEED,
    OLED_PAGE_ALTITUDE
};

#define OLED_COL_COUNT 64
#define OLED_DISPLAY_PAGE_COUNT 3

extern TinyGPSPlus gps;
extern double originLat;
extern double originLon; 
extern double distMax;
extern double dist;
extern double altMax;
extern double spdMax;
extern double altMin;

const uint8_t pageSequence[OLED_DISPLAY_PAGE_COUNT] = {
    OLED_PAGE_STATS,
    OLED_PAGE_SPEED,
    OLED_PAGE_ALTITUDE
};

class OledDisplay {
    public:
        OledDisplay(SSD1306 *display);
        void init();
        void loop();
        void page(uint8_t page);
        void nextPage();
    private:
        SSD1306 *_display;
        void renderPageStats();
        void renderPageSpeed();
        void renderPageAltitude();
        void renderHeader(String title);
        uint8_t _page = OLED_PAGE_NONE;
        uint8_t _mainPageSequenceIndex = 0;
        bool _forceDisplay = false;
};


#endif