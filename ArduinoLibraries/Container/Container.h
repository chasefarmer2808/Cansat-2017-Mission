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

#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <RTClib.h>
#include <TimerOne.h>
#include <EEPROM_Helper.h>
#include "Arduino.h"

#define RANGE5V 1023  //max range of the ADC
#define LOGRANGE 5.0  //max voltage input for default reference

#define R1 1000.0  //voltage divider R1 value in ohms
#define R2 1330.0   //R2 value in ohms
#define VOLT_DIV_RATIO (R2 / (R1 + R2))

#define LAUNCH 0  //first state is 255 b/c all values in EEPROM are stored as 255
#define RELEASE 1
#define LAND 2

#define STATE_ADDR 0
#define PACKET_ADDR 1
#define INITIALTIME_ADDR 3

#define CMD_RELEASE 'r'
#define CMD_BUZZER 'b'
#define CMD_RESET 'z'
#define CMD_LAND 'l'

#define BUZZ_FREQ 262

#define TELEM_FILE "Flight_Data.txt"

#define RELEASE_TIME_LIMIT 10

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
	uint16_t missionTime;  //seconds elapsed
	long initialTime;
	bool timeSet;
	bool transmitFlag;
	bool cmdFlag;
	bool releasing;
	uint8_t state;  //launching(0), released(1), landed(2)
	uint8_t releaseCount;
	int lightPin = A0;  //analog input pin for the light sensor
	int battPin = A1;  //analog pin for voltage divider input
	int magnetPin = 6;
	int buzzPin = 5;
	int releasePin = 4;  //digital pin for NiChrome release
	uint16_t packetCount;
	String packet;
	char command;
	File flightData;
	void setBMP180Data();  //sets temp and pressure attributes
	void setLux();  //sets the lux attribute
	void setMissionTime();  //set the elapsed time in seconds
	void setVoltage();  //set the battVoltage attribute
	void processCommand(SoftwareSerial* xbee);
	void setState(uint8_t val);
	void saveEEPROMData();
	void saveTelem();
	void release();
	void createPacket();
	void resetSaveData();
	void buzz(int dur, bool infinate);
	void endMission();
	Adafruit_BMP085_Unified bmp;
	RTC_DS3231 rtc;
};

#endif
