#ifndef Glider_H
#define Glider_H

#include "mbed.h"
#include <string>
#include "HMC5883L.h"
#include "BMP180.h"

const string telemFile = "/telem/telem.txt";

class Glider {
public:
    Serial* dev;
    Serial* xbee;
    double heading;
    float pressure;
    float temp;
    float alt;
    time_t startTime;
    time_t missionTime;
    HMC5883L* hmc;
    BMP180* bmp;
    Glider(Serial* device, PinName sda, PinName scl, PinName tx, PinName rx);
    void setHeading();
    void setTempPress();
    void setMissionTime();
    void saveTelem();
    void transmitPacket();
};

#endif
