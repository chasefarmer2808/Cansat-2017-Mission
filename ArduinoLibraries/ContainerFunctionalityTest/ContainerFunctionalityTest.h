#ifndef ContainerFunctionalityTest_h
#define ContainerFunctionalityTest_h

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <RTClib.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include "Arduino.h"

#define TEST_OUTPUT_FILENAME "ContainerTest.txt"
#define RANGE5V 1023  //max range of the ADC
#define LOGRANGE 5.0  //max voltage input for default reference

class ContainerFunctionalityTest {
public:
	ContainerFunctionalityTest();

	//test duration = endTime - startTime
	long startTime;
	long endTime;
	long duration;
	int rtcDay;
	uint8_t releaseCount;
	bool releasing;
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
	bool testDate(int day);  //makes sure clock is ticking
	bool testBMPTempPressAlt();  //makes sure data can be read
	bool testLux(int analogPin);  //gets a light reading in lux
	bool testBatteryVoltage(int analogPin, float ratio);  //gets the voltage of the battery
	bool testRelease(int nicrPin, int magPin, uint8_t seconds);  //turns on nichrome release for specified amount of time
	bool testBuzzer(int buzzPin, int freq);
	
	void generateTestResults();  //probs gonna need to know some info
};


#endif