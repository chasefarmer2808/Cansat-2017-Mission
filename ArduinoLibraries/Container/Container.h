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

	MOSFET Wiring
	mosfet		Nano
	G			D4
	S			GND
	D			NiChrome
*/


#ifndef Container_h
#define Container_h

#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <RTClib.h>
#include <TimerOne.h>
#include "Arduino.h"

#define RANGE5V 1024  //max range of the ADC
#define LOGRANGE 5.0

#define R1 100000.0  //voltage divider R1 value in ohms
#define R2 10000.0   //R2 value in ohms

#define LAUNCH 0  //first state is 255 b/c all values in EEPROM are stored as 255
#define RELEASE 1
#define LAND 2

#define STATE_ADDR 0
#define PACKET_ADDR 1
#define MISSIONTIME_ADDR 3

#define CMD_RELEASE 'r'
#define CMD_RESET 'z'

const byte RX = 2;  //Software serial RX pin for xbee (digital pin 2)
const byte TX = 3;  //Software serial TX pin for xbee (digital pin 3)

class Container {
public:
	Container();  //constructor
	float temperature;  //C
	float altitude;  //meters
	float pressure;  //hPa
	float lux;
	float battVoltage;  //volts
	int missionTime;  //seconds elapsed
	DateTime initialTime;
	bool timeSet;
	bool transmitFlag;
	bool cmdFlag;
	uint8_t state;  //launching(0), released(1), landed(2)
	int lightPin = A0;  //analog input pin for the light sensor
	int battPin = A1;  //analog pin for voltage divider input
	int releasePin = 4;  //digital pin for NiChrome release
	int packetCount;
	String packet;
	char command;
	void setBMP180Data();  //sets temp and pressure attributes
	void setLux();  //sets the lux attribute
	void setMissionTime();  //set the elapsed time in seconds
	void setVoltage();  //set the battVoltage attribute
	void processCommand(SoftwareSerial* xbee);
	void saveState(uint8_t val);
	void release();
	void createPacket();
	void resetSaveData();
	Adafruit_BMP085_Unified bmp;
	RTC_DS3231 rtc;
};

#endif
