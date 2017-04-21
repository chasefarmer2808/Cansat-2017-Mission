#ifndef Glider_H
#define Glider_H

#include "mbed.h"
#include <string>
#include "math.h"
#include "HMC5883L.h"
#include "BMP180.h"

#define SAVE_DATA_FORMAT "%d %d %d"
#define CMD_BUZZER 'b'
#define CMD_RESET 'r'

#define FREQ 1.0
#define CRUZE 0
#define LAND 1

#define PITO_V_DIVIDER 0.6666
#define PITO_ADC_RATIO 3.3 / 0xffff

#define AIR_DENSITY 1.225 //kg per m3

//AnalogIn pitot(p15);

class Glider {
public:
    Serial* dev;
    Serial* xbee;
    double heading;
    float pressure;
    float temp;
    float alt;
    float speed;
    bool cmdFlag;
    bool transmitFlag;
    time_t startTime;  //saved data
    time_t missionTime;  
    uint8_t packetCount;  //saved data
    uint8_t state;  //saved data
    HMC5883L* hmc;
    BMP180* bmp;
    Glider(Serial* device, PinName sda, PinName scl, PinName tx, PinName rx);
    InterruptIn xbeeRxInt;
    AnalogIn pitot;
    void setHeading();
    void setTempPressAlt();
    void setAirSpeed();
    void setMissionTime();
    void setTransmitFlag();
    void initSaveData();
    void saveTelem();
    void saveData();
    void transmitPacket();
    void setCommandFlag();
    void processCommand();
};

#endif
