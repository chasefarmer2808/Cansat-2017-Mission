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
    double heading;
    float pressure;
    float temp;
    float alt;
    string telemetry;
    HMC5883L* hmc;
    BMP180* bmp;
    Glider(Serial* device, PinName sda, PinName scl);
    void setHeading();
    void setTempPress();
    void saveTelem();
};

#endif
