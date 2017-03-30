#include "Container.h"
#include "Arduino.h"

Container::Container() {  //constructor implementation
	bmp = Adafruit_BMP085_Unified(10085);  //initialize the bmp library object

	//init attribute values
	temperature = 0.0;
	pressure = 0.0;
	lux = 0.0;
}

void Container::setBMP180Data() {
	sensors_event_t event;  //initialize an event
	bmp.getEvent(&event);  //do an read

	if (event.pressure) {
		this->pressure = event.pressure;  //save the pressure
		bmp.getTemperature(&this->temperature);  //save the temperature
	}
}

void Container::setLux() {
	int rawLux = analogRead(this->lightPin);  //get raw voltage input
	float logLux = rawLux * LOGRANGE / RANGE5V;
	this->lux = pow(10, logLux);
}
