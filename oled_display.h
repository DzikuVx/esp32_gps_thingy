#ifndef OLED_DISPLAY
#define OLED_DISPLAY

#include "types.h"
#include "SSD1306.h"
#include <TinyGPS++.h>

enum txOledPages {
    OLED_PAGE_NONE,
    OLED_PAGE_STATS,
    OLED_PAGE_SPEED,
    OLED_PAGE_ALTITUDE,
    OLED_PAGE_RELATIVE_ALTITUDE
};

#define OLED_COL_COUNT 64
#define OLED_DISPLAY_PAGE_COUNT 4

extern TinyGPSPlus gps;
extern GpsDataState_t gpsState;

const uint8_t pageSequence[OLED_DISPLAY_PAGE_COUNT] = {
    OLED_PAGE_STATS,
    OLED_PAGE_SPEED,
    OLED_PAGE_ALTITUDE,
    OLED_PAGE_RELATIVE_ALTITUDE
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
        void renderPageRelativeAltitude();
        void renderHeader(String title);
        uint8_t _page = OLED_PAGE_NONE;
        uint8_t _mainPageSequenceIndex = 0;
        bool _forceDisplay = false;
};


#endif