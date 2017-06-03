#ifndef ContainerFunctionalityTest_h
#define ContainerFunctionalityTest_h

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <RTClib.h>
#include <EEPROM.h>
#include "Arduino.h"

#define TEST_OUTPUT_FILENAME "ContainerTest.txt"

class ContainerFunctionalityTest {
public:
	ContainerFunctionalityTest();

	//test duration = endTime - startTime
	long startTime;
	long endTime;
	float temperature;  //C
	float altitude;  //meters
	float pressure;  //hPa
	float lux;  //light level
	float battVoltage;  //volts
	File log;
	String testResults;
	Adafruit_BMP085_Unified bmp;
	RTC_DS3231 rtc;

	bool initBMP180();
	bool initRTC();
	bool clearEEPROM();  //zeros out EEPROM
	bool saveTestDataToSD();  //saves test to SD card
	bool testEEPROM();  //makes sure values are all zero
	bool testRTC();  //makes sure clock is ticking
	bool getBMPTempPressAlt();  //makes sure data can be read
	bool getLux();  //gets a light reading in lux
	bool getBatteryVoltage();  //gets the voltage of the battery
	bool testNiChrome(uint8_t seconds);  //turns on nichrome release for specified amount of time
	
	void generateTestResults();  //probs gonna need to know some info
};


#endif