#include "Container.h"
#include "Arduino.h"

Container::Container() {  //constructor implementation
	bmp = Adafruit_BMP085_Unified(10085);  //initialize the bmp library object

	//init attribute values
	temperature = 0.0;
	pressure = 0.0;
	lux = 0.0;
	battVoltage = 0.0;
	transmitFlag = false;
	cmdFlag = false;

	EEPROM_read(STATE_ADDR, state);
	EEPROM_read(PACKET_ADDR, packetCount);
	EEPROM_read(INITIALTIME_ADDR, initialTime);

	//Serial.println(initialTime);

	//if (initialTime == 0) {
		//timeSet = false;
	//}
}

void Container::setBMP180Data() {
	sensors_event_t event;  //initialize an event
	bmp.getEvent(&event);  //do an read

	if (event.pressure) {
		this->pressure = event.pressure;  //save the pressure
		bmp.getTemperature(&this->temperature);  //save the temperature
		this->altitude = bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA, event.pressure);
	}
}

void Container::setLux() {
	int rawLux = analogRead(this->lightPin);  //get raw voltage input
	float logLux = rawLux * LOGRANGE / RANGE5V;
	this->lux = pow(10, logLux);
}

void Container::setMissionTime() {
	if (this->initialTime == 0) {//!this->timeSet) {
		this->initialTime = rtc.now().unixtime();
		//EEPROM_write(INITIALTIME_ADDR, this->initialTime);
		//this->timeSet = true;
	}

	DateTime currentTime = rtc.now();
	this->missionTime = currentTime.unixtime() - this->initialTime;
}

void Container::setVoltage() {
	int input = analogRead(this->battPin);
	float rawVoltage = (input * LOGRANGE) / RANGE5V;
	this->battVoltage = rawVoltage / VOLT_DIV_RATIO;
}

void Container::processCommand(SoftwareSerial* xbee) {
	if (xbee->available()) {
		this->command = xbee->read();
	}

	if (this->command == CMD_RELEASE && this->state == LAUNCH) {
		this->release();
		this->setState(RELEASE);
	}
	else if (this->command == CMD_RESET) {
		xbee->println("resetting data...");
		this->resetSaveData();
	}
	else if (this->command == CMD_BUZZER) {
		tone(this->buzzPin, BUZZ_FREQ, 3000);
	}

	this->command = NULL;  //band-aid for rx every second bug
}

void Container::release() {
	Serial.println("releasing...");
	digitalWrite(this->releasePin, 1);  //turn on the Nichrome
	delay(3000);
	digitalWrite(this->releasePin, 0);  //turn off the Nichrome
	this->state = 1;  //set state to released
	Serial.println("Glider should be released now");
}

void Container::createPacket() {
	
	this->packet = String("3387,CONTAINER," +
						   String(this->missionTime) +
						    "," +
						   String(this->packetCount) +
							"," +
						   String(this->altitude) +
						    "," +
						   String(this->temperature) +
							"," +
						   String(this->battVoltage) +
						    "," +
						   String(this->state));
}

void Container::setState(uint8_t val) {
	this->state = val;
}

void Container::saveEEPROMData() {
	EEPROM_write(STATE_ADDR, this->state);
	EEPROM_write(PACKET_ADDR, this->packetCount);
	EEPROM_write(INITIALTIME_ADDR, this->initialTime);
}

void Container::resetSaveData() {
	this->state = 0;
	this->packetCount = 0;
	this->initialTime = 0;

	EEPROM_write(STATE_ADDR, this->state);
	EEPROM_write(PACKET_ADDR, this->packetCount);
	EEPROM_write(INITIALTIME_ADDR, this->initialTime);
}
