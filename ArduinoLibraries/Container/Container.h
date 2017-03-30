#ifndef Container_h
#define Container_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "Arduino.h"

class Container {
public:
	Container();  //constructor
	float temperature;
	float pressure;
	float altitude;
	void setBMP180Data();
	Adafruit_BMP085_Unified bmp;
};

#endif
