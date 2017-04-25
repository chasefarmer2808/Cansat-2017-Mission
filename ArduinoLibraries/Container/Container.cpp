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
	releasing = false;
	releaseCount = 0;

	EEPROM_read(STATE_ADDR, state);
	EEPROM_read(PACKET_ADDR, packetCount);
	EEPROM_read(INITIALTIME_ADDR, initialTime);

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
		this->buzz(3000, false);  //sound buzzer for specified amount of time
	}
	else if (this->command == CMD_LAND) {
		this->endMission();  //set state for last time, stop timer interrupt, and sound buzzer
	}

	this->command = NULL;  //band-aid for rx every second bug
}

void Container::release() {
	this->releasing = true;  //set the counter flag to true

	digitalWrite(this->releasePin, 1);  //turn on the Nichrome

	while (digitalRead(this->magnetPin)) {  //while the magnet is closed
		if (this->releaseCount > RELEASE_TIME_LIMIT) {  //hold the nichrome on until the magnet opens 
			break;										//or until a count limit is reached (seconds)
		}
	}

	digitalWrite(this->releasePin, 0);  //turn off the Nichrome
	this->releasing = false;  //stop counting
	this->releaseCount = 0;  //reset counter for in-the-loop retesting (no MCU reset required)
	this->state = 1;  //set state to released
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

void Container::saveTelem() {
	this->flightData = SD.open("Flight_Data.txt", FILE_WRITE);

	if (this->flightData) {
		Serial.println("saving");
		this->flightData.println(this->packet);
		this->flightData.close();
	}
	
}

void Container::resetSaveData() {
	this->state = 0;
	this->packetCount = 0;
	this->initialTime = 0;

	EEPROM_write(STATE_ADDR, this->state);
	EEPROM_write(PACKET_ADDR, this->packetCount);
	EEPROM_write(INITIALTIME_ADDR, this->initialTime);
}

void Container::buzz(int dur, bool infinate) {  //dur in millis
	if (infinate) {
		tone(buzzPin, BUZZ_FREQ);
		while (1);
	}

	tone(buzzPin, BUZZ_FREQ, dur);
}

void Container::endMission() {
	this->setState(LAND);
	Timer1.detachInterrupt();
	this->buzz(0, true);
	while (1);
}
