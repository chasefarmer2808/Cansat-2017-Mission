#include "Glider.h"

Serial pc(USBTX, USBRX);

LocalFileSystem local("telem");

int main() {
    Glider g(&pc, p28, p27, p9, p10);

    while (1) {
        g.setHeading();
        g.setTempPress();
        g.setMissionTime();
        //g.saveTelem();
        /*
        pc.printf("heading: %f\r\n", g.heading);
        pc.printf("pressure: %f\r\n", g.pressure);
        pc.printf("temp: %f\r\n", g.temp);
        pc.printf("alt: %f\r\n", g.alt);
        */
        //pc.printf("start time: %d\r\n", g.startTime);
        //pc.printf("curr time: %d\r\n", time(NULL));
        g.transmitPacket();
        wait(1);
    }

}
