#ifndef TYPES_H_
#define TYPES_H_

struct GpsDataState_t {
    double originLat = 0;
    double originLon = 0; 
    double originAlt = 0;
    double distMax = 0;
    double dist = 0;
    double altMax = -999999;
    double altMin = 999999;
    double spdMax = 0;
    double prevDist = 0;
};

#endif