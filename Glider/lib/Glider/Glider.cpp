#include "Glider.h"

Glider::Glider(Serial* device, PinName sda, PinName scl) {
    dev = device;
    dev->printf("constructing glider...");  
    heading = 0; 
    telemetry = "hello";
    hmc = new HMC5883L(sda, scl);
    bmp = new BMP180(sda, scl);
    bmp->Initialize();
}

void Glider::setHeading() {
    this->heading = this->hmc->getHeadingXYDeg();
}

void Glider::setTempPress() {
    bmp->ReadData(&this->temp, &this->pressure);
}

void Glider::saveTelem() {
     FILE *fp = fopen("/telem/telem.txt", "w");
     fprintf(fp, "hello,");
     fclose(fp);  
}