#include "Glider.h"

Glider::Glider(Serial* device, PinName sda, PinName scl, PinName tx, PinName rx) {
    startTime = time(NULL);
    dev = device;
    xbee = new Serial(tx, rx);
    dev->printf("constructing glider...");
    heading = 0.0;
    pressure = 0.0;
    temp = 0.0;
    alt = 0.0;
    hmc = new HMC5883L(sda, scl);
    bmp = new BMP180(sda, scl);
    bmp->Initialize(1013.25, BMP180_OSS_ULTRA_LOW_POWER);
}

void Glider::setHeading() {
    this->heading = this->hmc->getHeadingXYDeg();
}

void Glider::setTempPress() {
    bmp->ReadData(&this->temp, &this->pressure, &this->alt);
}

void Glider::setMissionTime() {
    this->missionTime = time(NULL) - this->startTime;
}

void Glider::saveTelem() {
     FILE *fp = fopen("/telem/telem.txt", "w");
     fprintf(fp, "hello,");
     fclose(fp);
}

void Glider::transmitPacket() {
    //TODO: transmit mission time, packet count, alt, pressure, speed
            //temp, voltage, state
    xbee->printf("3387, GLIDER, %f, %d\r\n", this->heading, this->missionTime);
}
