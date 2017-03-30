#include "Container.h"
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Container::Container() {  //constructor implementation
	bmp = Adafruit_BMP085_Unified(10085);
	temperature = 0.0;
}

void Container::setBMP180Data() {
	sensors_event_t event;
	bmp.getEvent(&event);

	if (event.pressure) {
		this->pressure = event.pressure;
		bmp.getTemperature(&this->temperature);
		this->altitude = bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA, event.pressure);
	}
}
