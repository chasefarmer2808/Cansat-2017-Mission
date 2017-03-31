#include "Container.h"
#include "Arduino.h"

Container::Container() {  //constructor implementation
	bmp = Adafruit_BMP085_Unified(10085);  //initialize the bmp library object

	//init attribute values
	temperature = 0.0;
	pressure = 0.0;
	lux = 0.0;
	missionTime = 0;
	battVoltage = 0.0;
	state = 0;
	timeSet = false;
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

void Container::setMissionTime() {
	if (!this->timeSet) {
		this->initialTime = rtc.now();
		this->timeSet = true;
	}

	DateTime currentTime = rtc.now();
	this->missionTime = currentTime.unixtime() - this->initialTime.unixtime();
}

void Container::setVoltage() {
	int input = analogRead(this->battPin);
	float rawVoltage = (input * LOGRANGE) / RANGE5V;
	this->battVoltage = rawVoltage / (R2 / (R1 + R2));
}

void Container::release() {
	Serial.println("releasing...");
	digitalWrite(this->releasePin, HIGH);
	delay(3000);
	digitalWrite(this->releasePin, LOW);
	//this->state = 1;
	Serial.println("Glider should be released now");
}
