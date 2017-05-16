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
#define VOLT_DIV_RATIO (R2 / (R1 + R2))  //voltage divider result for battery voltage input

#define LAUNCHING 0  //flying up in the rocket
#define FALLING 1    //released from the rocket
#define RELEASED 2   //glider has been released
#define LANDED 3     //container on the ground

#define STATE_ADDR 0  //index in EEPROM of state value
#define PACKET_ADDR 1  //index in EEPROM of packet count value
#define INITIALTIME_ADDR 3  //index in EEPROM of initial time value

#define CMD_RELEASE 'r'  //when in falling state, forces a call to the release() function
#define CMD_BUZZER 'b'  //sounds buzzer for 3 seconds
#define CMD_RESET 'z'  //zeros out mission time, packet count, and state
#define CMD_LAND 'l'  //forces a call to endMission() function
#define CMD_NEXT_STATE 'n'  //increments the state
#define CMD_PREV_STATE 'p'  //decrements the state

#define BUZZ_FREQ 262 //Hz
#define BUZZ_DUR 5000 //millis

#define RELEASE_TIME_LIMIT 10  //seconds
#define EMERGENCY_BUZZER_TIME_LIMIT 250//seconds

#define LIGHT_THRESH 1000 //lux
#define RELEASE_ALT 420 //meters

#define TELEM_FILE "Telem.csv"  //name of file on sd card to save telemetry to

#define DECIMAL_PLACES 2  //number of digits after decimal for floating point numbers

const byte RX = 2;  //Software serial RX pin for xbee (digital pin 2)
const byte TX = 3;  //Software serial TX pin for xbee (digital pin 3)

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
	uint16_t emergencyCount;
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
