/*
	Using 5V AREF for all analog input.


	Light Sensor Wiring
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
*/


#ifndef Container_h
#define Container_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "Arduino.h"

#define RANGE5V 1024;  //max range of the ADC
#define LOGRANGE 5.0


class Container {
public:
	Container();  //constructor
	float temperature;  
	float pressure;
	float lux;
	int lightPin = A0;  //analog input pin for the light sensor
	void setBMP180Data();  //sets temp and pressure attributes
	void setLux();  //sets the lux attribute
	Adafruit_BMP085_Unified bmp;
};

#endif
