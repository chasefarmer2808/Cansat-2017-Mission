/*
 Name:		ContainerFunctionalityTest.ino
 Created:	6/3/2017 11:54:18 AM
 Author:	chase
*/
#include <ContainerFunctionalityTest.h>

#define XBEE_SS_RX 2  //software serial pin
#define XBEE_SS_TX 3  //software serial pin
#define SD_PIN 10  //pin for sd card module
#define BAUD 9600  //baud rate
#define TODAY 4  //current date
#define LIGHT_PIN A0  //analog input for light sensor
#define BATT_PIN A1  //analog input for voltage input
#define MAG_PIN 6  //digital pin for magnetic switch
#define BUZZ_PIN 5  //pwm output pin for the buzzer
#define RELEASE_PIN 4  //digital pin for the mosfet
#define HOT_TIME 10  //time in seconds to turn on the nichrome
#define BUZZ_FREQ 262  //frequency the buzzer will buzz at
#define R1 1000.0  //voltage divider R1 value in ohms
#define R2 1330.0   //R2 value in ohms
#define VOLT_DIV_RATIO (R2 / (R1 + R2))  //voltage divider result for battery voltage input

//global objects to be used during test
ContainerFunctionalityTest test = ContainerFunctionalityTest();
SoftwareSerial xbee(XBEE_SS_RX, XBEE_SS_RX);

// the setup function runs once when you press reset or power the board
void setup() {
	xbee.begin(BAUD);
	xbee.println("Setting up test environment...");
	analogReference(DEFAULT);
	Timer1.initialize();  //set up 1 Hz timer to count seconds
	Timer1.attachInterrupt(secondCount);

	xbee.println("Finding SD card module...");
	pinMode(SD_PIN, OUTPUT);
	pinMode(LIGHT_PIN, INPUT);
	pinMode(BATT_PIN, INPUT);

	if (!SD.begin(SD_PIN)) {
		xbee.println("SD failed");
		return;
	}

	SD.mkdir("/ContainerTest");

	if (SD.exists(TEST_OUTPUT_FILENAME)) {  //make a new test result file
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

	log("Testing RTC Module...");
	if (!test.testDate(TODAY)) {
		logln("FAIL.");
	}
	else {
		logln("PASS.");
	}
	delay(2000);

	log("Testing BMP180 Module...");
	if (!test.testBMPTempPressAlt()) {
		logln("FAIL.");
	}
	else {
		logln("PASS.");
	}
	delay(2000);

	log("Testing Light Sensor...");
	if (!test.testLux(LIGHT_PIN)) {
		logln("FAIL.");
	}
	else {
		logln("PASS.");
	}
	delay(2000);

	log("Testing battery voltage..."); 
	if (!test.testBatteryVoltage(BATT_PIN, VOLT_DIV_RATIO)) {
		logln("FAIL.");
	}
	else {
		logln("PASS.");
	}
	delay(2000);

	log("Testing release mechanism...");
	if (!test.testRelease(RELEASE_PIN, MAG_PIN, HOT_TIME)) {
		logln("ERROR.");
	}
	else {
		logln("DONE.");
	}
	delay(2000);

	log("Testing buzzer...");
	if (!test.testBuzzer(BUZZ_PIN, BUZZ_FREQ)) {
		logln("ERROR.");
	}
	else {
		logln("DONE.");
	}
	delay(2000);

	test.endTime = test.rtc.now().unixtime();
	test.duration = test.endTime - test.startTime;

	logln("All tests complete.");
	logln("Results:");
	logln(test.testResults);
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

void secondCount() {
	if (test.releasing) {
		test.releaseCount++;
	}
}
