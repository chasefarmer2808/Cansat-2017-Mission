/*
 Name:		ContainerFunctionalityTest.ino
 Created:	6/3/2017 11:54:18 AM
 Author:	chase
*/
#include <ContainerFunctionalityTest.h>

#define XBEE_SS_RX 2
#define XBEE_SS_TX 3
#define SD_PIN 10
#define BAUD 9600

//global objects to be used during test
ContainerFunctionalityTest test = ContainerFunctionalityTest();
SoftwareSerial xbee(XBEE_SS_RX, XBEE_SS_RX);

// the setup function runs once when you press reset or power the board
void setup() {
	xbee.begin(BAUD);
	xbee.println("Setting up test environment...");
	analogReference(DEFAULT);

	xbee.println("Finding SD card module...");
	pinMode(SD_PIN, OUTPUT);

	if (!SD.begin(SD_PIN)) {
		xbee.println("SD failed");
		return;
	}

	SD.mkdir("/ContainerTest");

	if (SD.exists(TEST_OUTPUT_FILENAME)) {
		SD.remove(TEST_OUTPUT_FILENAME);
	}

	test.log = SD.open(TEST_OUTPUT_FILENAME, FILE_WRITE);
	 
	log("Initializing RTC...");
	if (!test.initRTC()) {
		xbee.println("RTC init FAILED");
		return;
	}
	delay(2000);
	logln("DONE.");

	log("Initializing BMP...");
	if (!test.initBMP180()) {
		logln("BMP init FAILED");
		return;
	}
	delay(2000);
	logln("DONE.");

	xbee.println("Clearing EEPROM...");
	if (!test.clearEEPROM()) {
		logln("EEPROM ERROR");
		return;
	}
	delay(2000);
	logln("DONE.");

	logln("");

	logln("Environment ready.");

	logln("Send any char acters to start tests...");
	while (!xbee.available());  //wait for char to be recieved.
	logln((String("Recieved ") + String(xbee.read()) + String(".  Starting test...")));

	test.startTime = test.rtc.now().unixtime();
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}

void log(String s) {
	xbee.print(s);
	test.log.print(s);
}

void logln(String s) {
	xbee.println(s);
	test.log.println(s);
}
