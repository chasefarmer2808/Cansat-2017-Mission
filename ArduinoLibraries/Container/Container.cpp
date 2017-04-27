#include "Container.h"
#include "Arduino.h"

Container::Container(SoftwareSerial* radio) {  //constructor implementation
	xbee = radio;
	xbee->begin(9600);

	bmp = Adafruit_BMP085_Unified(10085);  //initialize the bmp library object

	init();
}

void Container::init() {

	//init attribute values
	this->setState(LAUNCHING);
	this->temperature = 0.0;
	this->pressure = 0.0;
	this->lux = 0.0;
	this->battVoltage = 0.0;
	this->transmitFlag = false;
	this->cmdFlag = false;
	this->forceRelease = false;
	this->releasing = false;
	this->lastTwo = false;
	this->releaseCount = 0;
	this->lastTwoCount = 0;
	this->emergencyCount = 0;

	EEPROM_read(STATE_ADDR, state);
	EEPROM_read(PACKET_ADDR, packetCount);
	EEPROM_read(INITIALTIME_ADDR, initialTime);
}

void Container::updateTelem() {
	//gather latest sensor readings
	this->setBMP180Data();
	this->setLux();
	this->setMissionTime();
	this->setVoltage();
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
	if (this->initialTime == 0) {
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

void Container::processCommand() {
	if (this->xbee->available()) {
		this->command = this->xbee->read();
	}

	if (this->command == CMD_RELEASE && this->state == FALLING) {
		this->forceRelease = true;
	}
	else if (this->command == CMD_RESET) {
		this->xbee->println("resetting data...");
		this->emergencyCount = 0;
		this->resetSaveData();
	}
	else if (this->command == CMD_BUZZER) {
		this->buzz(BUZZ_DUR, false);  //sound buzzer for specified amount of time
	}
	else if (this->command == CMD_LAND) {
		this->endMission();  //set state for last time, stop timer interrupt, and sound buzzer
	}
	else if (this->command == CMD_NEXT_STATE) {
		this->state++;
	}
	else if (this->command == CMD_PREV_STATE) {
		if (this->state >= 1) {  //only if not on first state
			this->state--;
		}
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

void Container::transmitTelem() {
	this->packetCount++;  //increment the packet count
	this->xbee->println(this->packet);  //send the packet to GS
	this->saveEEPROMData();  //save data to eeprom
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
	this->flightData = SD.open(TELEM_FILE, FILE_WRITE);
	if (this->flightData) {
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

	this->init();
}

void Container::buzz(int dur, bool infinate) {  //dur in millis
	if (infinate) {  //sound buzzer forever
		tone(buzzPin, BUZZ_FREQ);
		while (1);
	}

	tone(buzzPin, BUZZ_FREQ, dur);  //sound buzzer for specified duration
}

void Container::checkFallingCondition() {
	if (this->lux >= LIGHT_THRESH) {  //container ejected from rocket
		this->setState(FALLING);  //container now falling by parachute
	}
}

void Container::checkReleaseCondition() {
	if (this->altitude <= RELEASE_ALT || this->forceRelease) {  //container has fallen to nominal release altitude
		this->release();  //burn the nichrome to release the glider
		this->setState(RELEASED);  //glider should be released and falling at this point
		this->lastTwo = true;   //set this flag so only two more packets can get send
		this->forceRelease = false;
	}
}

void Container::endMission() {
	this->saveEEPROMData();
	Timer1.stop();  //stop timer so no more timer interrupts occur
	this->buzz(0, true);  //sound buzzer forever
	while (1);  //this line will never get reached, but just in case
}
