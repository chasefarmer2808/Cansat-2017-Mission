#include "Glider.h"

Glider::Glider(Serial* device, PinName sda, PinName scl) {
    dev = device;
    dev->printf("constructing glider...");
    heading = 0;
    telemetry = "hello";
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

void Glider::saveTelem() {
     FILE *fp = fopen("/telem/telem.txt", "w");
     fprintf(fp, "hello,");
     fclose(fp);
}
