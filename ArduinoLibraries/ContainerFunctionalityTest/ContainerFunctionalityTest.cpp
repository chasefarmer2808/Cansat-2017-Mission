#include "ContainerFunctionalityTest.h"
#include "Arduino.h"

ContainerFunctionalityTest::ContainerFunctionalityTest() {
	releasing = false;
	releaseCount = 0;
}

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
	for (int i = 0; i < 10; i++) {  //reset first 10 values in EEPROM to 0
		EEPROM[i] = 0;
	}
	return true;
}

bool ContainerFunctionalityTest::testDate(int day) {
	this->rtcDay = this->rtc.now().day();  //get today's date

	if (this->rtcDay == day) {
		return true;
	}

	return false;
}

bool ContainerFunctionalityTest::testBMPTempPressAlt() {
	sensors_event_t event;  //initialize an event
	bmp.getEvent(&event);  //do a read

	if (event.pressure) {
		this->pressure = event.pressure;  //save the pressure
		bmp.getTemperature(&this->temperature);  //save the temperature
		this->altitude = bmp.pressureToAltitude(1009, event.pressure);
	}

	if (this->pressure > 0 && this->altitude > 0 && this->temperature > 0) {  //chech is everything is positive
		return true;
	}

	return false;
}

bool ContainerFunctionalityTest::testLux(int analogPin) {
	int rawLux = analogRead(analogPin);  //get raw voltage input
	float logLux = rawLux * LOGRANGE / RANGE5V;
	this->lux = pow(10, logLux);

	if (this->lux > 0) {
		return true;
	}

	return false;
}

bool ContainerFunctionalityTest::testBatteryVoltage(int analogPin, float ratio) {
	int input = analogRead(analogPin);
	float rawVoltage = (input * LOGRANGE) / RANGE5V;
	this->battVoltage = rawVoltage / ratio;

	if (this->battVoltage > 2) {
		return true;
	}

	return false;
}

bool ContainerFunctionalityTest::testRelease(int nicrPin, int magPin, uint8_t seconds) {
	this->releasing = true;  //set the counter flag to true

	digitalWrite(nicrPin, 1);  //turn on the Nichrome

	while (digitalRead(magPin)) {  //while the magnet is closed
		if (this->releaseCount > seconds) {  //hold the nichrome on until the magnet opens
			break;										//or until a count limit is reached (seconds)
		}
	}

	digitalWrite(nicrPin, 0);  //turn off the Nichrome
	this->releasing = false;  //stop counting
	this->releaseCount = 0;  //reset counter for in-the-loop retesting (no MCU reset required)
	return true;
}

bool ContainerFunctionalityTest::testBuzzer(int buzzPin, int freq) {
	tone(buzzPin, freq, 3000);  //sound buzzer for specified duration
	return true;
}

void ContainerFunctionalityTest::generateTestResults() {
	this->testResults = String("Test Duration: " + String(this->duration) + "\r" +
		"RTC day: " + String(this->rtcDay) + "\r" +
		"Temperature (C): " + String(this->temperature) + "\r" +
		"Pressure (hPa): " + String(this->pressure) + "\r" +
		"Altitude (meters): " + String(this->altitude) + "\r" +
		"Light reading (lux): " + String(this->lux) + "\r" +
		"Battery voltage (Volts): " + String(this->battVoltage) + "\r"
	);
}

