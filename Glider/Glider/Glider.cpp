#include "Glider.h"

Glider::Glider(Serial* device, PinName sda, PinName scl, PinName tx, PinName rx) : xbeeRxInt(rx), pitot(p15) {
    dev = device;
    dev->printf("constructing glider...");
    initSaveData();
    xbee = new Serial(tx, rx);
    heading = 0.0;
    pressure = 0.0;
    temp = 0.0;
    alt = 0.0;
    cmdFlag = false;
    transmitFlag = false;
    hmc = new HMC5883L(sda, scl);
    bmp = new BMP180(sda, scl);
    bmp->Initialize(1013.25, BMP180_OSS_ULTRA_LOW_POWER);
    xbeeRxInt.rise(this, &Glider::setCommandFlag);
}

void Glider::setHeading() {
    this->heading = this->hmc->getHeading();
}

void Glider::setTempPressAlt() {
    bmp->ReadData(&this->temp, &this->pressure, &this->alt);
}

void Glider::setAirSpeed() {
    float vInput =  this->pitot.read_u16() * PITO_ADC_RATIO;
    vInput = vInput / PITO_V_DIVIDER;
    
    float diffPressure = (vInput - 0.5) / (0.2 * 5.0);  //kPa
    this->speed = sqrt((2*diffPressure) / AIR_DENSITY);
}

void Glider::setMissionTime() {
    this->missionTime = time(NULL) - this->startTime;  //duration = current tome - start time
}

void Glider::setTransmitFlag() {
    this->transmitFlag = true;   
}

void Glider::saveTelem() {
     FILE *fp = fopen("/telem/telem.txt", "w");
     fprintf(fp, "hello,");
     fclose(fp);
}

void Glider::initSaveData() {
     this->dev->printf("getting the time...\r\n");
     FILE *fp = fopen("/telem/data.txt", "r");   //attempt to read the saved data
     
     if (fp == NULL) {  //file does not exist
        this->dev->printf("starting the time...\r\n");
        this->startTime = time(NULL); //initialize the start time to now
        this->packetCount = 0;  //start the packet count at 0
        this->state = CRUZE;
        FILE *fp1 = fopen("/telem/data.txt", "w");  //create the data file
        fprintf(fp1, SAVE_DATA_FORMAT, this->startTime, this->packetCount, this->state);  //save the start time
        fclose(fp1);
        return;   
     }
     
     //else, the file exists.  Read the data
     fscanf(fp, SAVE_DATA_FORMAT, &this->startTime, &this->packetCount, &this->state);  //set the start time, packet count, and state
     rewind(fp);  //move cursor back to beginning of file
     fclose(fp);
}

void Glider::saveData() { 
     FILE *fp = fopen("/telem/data.txt", "w");
     fprintf(fp, SAVE_DATA_FORMAT, this->startTime, this->packetCount, this->state);  //save the start time, packet count, and state to flash
     fclose(fp);   
}

void Glider::transmitPacket() {
    //TODO: transmit 
            // voltage
    this->packetCount++;
    
    this->xbee->printf("3387, GLIDER, %d, %d, %f, %f, %f, %f, %f, %d\r\n", 
                        this->missionTime,  //duration in seconds 
                        this->packetCount,  
                        this->alt, //meters
                        this->pressure, //
                        this->temp,  //C
                        this->speed,  //m per s
                        this->heading, //degrees from North
                        this->state);
}

void Glider::setCommandFlag() {
    this->cmdFlag = true;   
}

void Glider::processCommand() {
    if (this->cmdFlag) {  //command recieved
        char command = this->xbee->getc();  //get the command char
        
        switch (command) {
            case CMD_BUZZER:  //'b' was recieved.  sound the buzzer
                  this->dev->printf("buzzing...\r\n");
                  this->state = LAND;
                  while(1);
                  break; 
            case CMD_RESET:
                  this->dev->printf("resetting save file...\r\n");
                  remove("/telem/data.txt");
                  this->initSaveData();
        }
        
        this->cmdFlag = false;  //reset the flag so a new command can be recieved
    }  
}
