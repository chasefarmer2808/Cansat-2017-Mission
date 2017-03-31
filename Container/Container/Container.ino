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

SoftwareSerial xbee(2, 3);  //software serial port for the xbee

void setup() {
	Serial.begin(9600);
	xbee.begin(9600);  //start the software serial port
	
	analogReference(DEFAULT);  //reference range 0V - 5V

	if (!c.bmp.begin()) {
		Serial.print("BMP failed");
		while (1);
	}

	if (!c.rtc.begin()) {
		Serial.println("RTC failed");
		while (1);
	}

	if (c.rtc.lostPower()) {
		c.rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //initialize the time
	}

	pinMode(c.battPin, INPUT);  //set the voltage input
	pinMode(c.releasePin, OUTPUT);  //set the digital output of the release pin
	attachInterrupt(digitalPinToInterrupt(RX), processCommand, RISING);  //initialize an interrupt for D2
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
	//xbee.write(test);

	if (command == 'r' && c.state == 0) {
		c.release();
	}

	delay(1000);
}

//ISR for RX
void processCommand() {
	if (xbee.available()) {
		command = xbee.read();
	}
}
