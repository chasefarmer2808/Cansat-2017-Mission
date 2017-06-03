#include "ContainerFunctionalityTest.h"
#include "Arduino.h"

ContainerFunctionalityTest::ContainerFunctionalityTest() {}

bool ContainerFunctionalityTest::initRTC() {
	bool res = true;

	if (!this->rtc.begin()) {
		res = false;
	}

	if (this->rtc.lostPower()) {
		this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //initialize the time
	}

	return res;
}

bool ContainerFunctionalityTest::initBMP180() {
	bool res = true;

	this->bmp = Adafruit_BMP085_Unified(10085);

	if (!this->bmp.begin()) {
		res = false;
	}

	return res;
}

bool ContainerFunctionalityTest::clearEEPROM() {
	for (int i = 0; i < 10; i++) {
		EEPROM[i] = 0;
	}
	return true;
}