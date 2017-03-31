/*
	Using 5V AREF for all analog input.


	GA1A Light Sensor Wiring
	Sensor		Nano
	Output		A0
	VCC			5V
	GND			GND

	BMP180 Wiring
	Sensor		Nano
	SDA			A4
	SCL			A5
	VIN			3V3
	GND			GND

	Sparkfun RCT Wiring
	RTC			Nano
	SDA			A4
	SCL			A5
	VIN			5V
	GND			GND

	XBEE Wiring
	xbee		Nano
	VSS			GND
	VDD			5V
	DOUT		D2
	DIN			D3
*/


#ifndef Container_h
#define Container_h

#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <RTClib.h>
#include "Arduino.h"

#define RANGE5V 1024  //max range of the ADC
#define LOGRANGE 5.0

#define R1 100000.0
#define R2 10000.0



const byte RX = 2;  //Software serial RX pin for xbee (digital pin 2)
const byte TX = 3;  //Software serial TX pin for xbee (digital pin 3)

class Container {
public:
	Container();  //constructor
	float temperature;  
	float pressure;
	float lux;
	float battVoltage;
	int missionTime;
	DateTime initialTime;
	bool timeSet;
	int state;
	int lightPin = A0;  //analog input pin for the light sensor
	int battPin = A1;
	int releasePin = 4;
	void setBMP180Data();  //sets temp and pressure attributes
	void setLux();  //sets the lux attribute
	void setMissionTime();
	void setVoltage();
	void release();
	Adafruit_BMP085_Unified bmp;
	RTC_DS3231 rtc;
};

#endif
