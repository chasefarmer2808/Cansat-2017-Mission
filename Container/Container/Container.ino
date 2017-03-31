/*
 Name:		Container.ino
 Created:	3/29/2017 8:45:56 PM
 Author:	chase
*/

// the setup function runs once when you press reset or power the board

#include <Container.h>

#define BAUD 9600

Container c = Container();

volatile char command;  //global command variable

SoftwareSerial xbee(2, 3);

void setup() {
	Serial.begin(9600);
	xbee.begin(9600);
	
	analogReference(DEFAULT);  //reference range 0V - 5V
	Serial.println("init");
	if (!c.bmp.begin()) {
		Serial.print("here");
	}

	if (!c.rtc.begin()) {
		Serial.println("RTC failed");
		while (1);
	}

	if (c.rtc.lostPower()) {
		c.rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}

	pinMode(c.battPin, INPUT);
	pinMode(c.releasePin, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(RX), processCommand, RISING);
	
}

// the loop function runs over and over again until power down or reset
void loop() { 
	c.setBMP180Data(); 
	c.setLux();
	c.setMissionTime();
	c.setVoltage();
	Serial.println(c.temperature);
	Serial.println(c.pressure);
	Serial.println(c.lux);
	Serial.println(c.missionTime);
	Serial.println(c.battVoltage);
	Serial.println();

	if (command == 'r' && c.state == 0) {
		c.release();
	}

	delay(1000);
}

void processCommand() {
	if (xbee.available()) {
		command = xbee.read();
	}
}
