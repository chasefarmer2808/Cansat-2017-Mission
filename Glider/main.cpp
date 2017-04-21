#include "Glider.h"

Serial pc(USBTX, USBRX);
LocalFileSystem local("telem");
Ticker transmitTimer;

int main() {
    Glider g(&pc, p28, p27, p9, p10);
    transmitTimer.attach(&g, &Glider::setTransmitFlag, FREQ); 

    while (1) {
        
        g.setHeading();
        g.setTempPressAlt();
        g.setAirSpeed();
        g.setMissionTime();
        g.saveData();
        
        if (g.transmitFlag) {
            g.transmitPacket();
            g.transmitFlag = false;   
        }
        //pc.printf("heading: %f\r\n", g.heading);
        g.processCommand();
        //g.saveTelem();
        
        //pc.printf("%d\r\n", g.missionTime);
        /*
        pc.printf("heading: %f\r\n", g.heading);
        pc.printf("pressure: %f\r\n", g.pressure);
        pc.printf("temp: %f\r\n", g.temp);
        pc.printf("alt: %f\r\n", g.alt);
        g.xbee->printf("hello\r\n");
        */
        //wait(1);
    }

}

