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

#define LAUNCHING 0  //first state is 255 b/c all values in EEPROM are stored as 255
#define FALLING 1
#define RELEASED 2
#define LANDED 3

#define STATE_ADDR 0
#define PACKET_ADDR 1
#define INITIALTIME_ADDR 3

#define CMD_RELEASE 'r'
#define CMD_BUZZER 'b'
#define CMD_RESET 'z'
#define CMD_LAND 'l'
#define CMD_NEXT_STATE 'n'
#define CMD_PREV_STATE 'p'

#define BUZZ_FREQ 262 //Hz
#define BUZZ_DUR 5000 //millis

#define TELEM_FILE "Flight_Data.txt"

#define RELEASE_TIME_LIMIT 10  //seconds

#define LIGHT_THRESH 1000 //lux
#define RELEASE_ALT 420 //meters

const byte RX = 2;  //Software serial RX pin for xbee (digital pin 2)
const byte TX = 3;  //Software serial TX pin for xbee (digital pin 3)

//SoftwareSerial xbee(2, 3);

class Container {
public:
	Container(SoftwareSerial* radio);  //constructor
	void init();
	float temperature;  //C
	float altitude;  //meters
	float pressure;  //hPa
	float lux;  //light level
	float battVoltage;  //volts
	uint16_t missionTime;  //seconds elapsed
	long initialTime;  //start time of the mission in epoch unix time
	bool timeSet;  //flag for checking if the initial time has been set
	bool transmitFlag;  //used for 1Hz transmit interrupt
	bool cmdFlag;  //used for RX external interrupt
	bool forceRelease;
	bool releasing;  //used for controlling the nichrome burn timer
	bool lastTwo;  //flag for sending the last two seconds of data
	uint8_t state;  //launching(0), released(1), landed(2)
	uint8_t releaseCount;  //holds the seconds the nichrome has been on
	uint8_t lastTwoCount;  //holds the seconds for the last two data packets
	int lightPin = A0;  //analog input pin for the light sensor
	int battPin = A1;  //analog pin for voltage divider input
	int magnetPin = 6;  //digital pin 6
	int buzzPin = 5;  //digital pin 5
	int releasePin = 4;  //digital pin for NiChrome release
	uint16_t packetCount;  //number of packets sent
	String packet;  //holds the entire packet to be sent to GS
	char command;  //holds command char recieved from GS
	File flightData;  
	void updateTelem();  //gather latest sensor readings
	void setBMP180Data();  //sets temp and pressure attributes
	void setLux();  //sets the lux attribute
	void setMissionTime();  //set the elapsed time in seconds
	void setVoltage();  //set the battVoltage attribute
	void processCommand();
	void setState(uint8_t val);
	void saveEEPROMData();
	void saveTelem();
	void release();
	void createPacket();
	void transmitTelem();
	void resetSaveData();
	void buzz(int dur, bool infinate);
	void checkFallingCondition();
	void checkReleaseCondition();
	void endMission();
	Adafruit_BMP085_Unified bmp;
	RTC_DS3231 rtc;
	SoftwareSerial* xbee;
};

#endif
