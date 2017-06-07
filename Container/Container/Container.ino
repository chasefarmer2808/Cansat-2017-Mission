/*
 Name:		Container.ino
 Created:	3/29/2017 8:45:56 PM
 Author:	chase
*/

// the setup function runs once when you press reset or power the board

#include <Container.h>

SoftwareSerial xbee(2, 3);  //software serial port for the xbee

Container c = Container(&xbee);

void setup() { 
	Serial.begin(9600);
	analogReference(DEFAULT);  //reference range 0V - 5V
	 
	pinMode(10, OUTPUT);
	
	if (!SD.begin(10)) {
		Serial.println("SD failed");
		return;
	}
	
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

	Timer1.initialize();
	Timer1.attachInterrupt(sendPacket); //Interrupt using a timer to send a packet every second
	pinMode(c.battPin, INPUT);  //set the voltage input
	pinMode(c.releasePin, OUTPUT);  //set the digital output of the release pin
	pinMode(c.magnetPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(RX), processCommand, RISING);  //initialize an interrupt for D2

	SD.mkdir("/Conatiner");
}

// the loop function runs over and over again until power down or reset
void loop() { 

	switch (c.state) {
	case LAUNCHING:
		//c.checkFallingCondition();
		c.updateTelem();
		break;

	case FALLING:
		c.checkReleaseCondition();
		c.updateTelem();
		break;

	case RELEASED:

		while (c.lastTwoCount < 2) {
			Serial.println(c.lastTwoCount);
		}
		//wait for last two packets to send
		c.setState(LANDED);
		break;
	case LANDED:
		c.endMission();
		break;
	}

	if (c.cmdFlag) {
		c.processCommand();
		c.cmdFlag = false;
	}


	if (c.transmitFlag){
		c.xbee->print("LUX: ");
		c.xbee->println(c.lux);
		c.createPacket();
		c.saveTelem();
		c.transmitTelem();
	    c.transmitFlag = false;
	}

	if (c.emergencyCount > EMERGENCY_BUZZER_TIME_LIMIT) {
		c.setState(LANDED);
	}

}

//ISR for RX
void processCommand() {
	c.cmdFlag = true;

	if (c.state == LANDED) {
		c.processCommand();
		c.cmdFlag = false;
	}
}

//ISR for transmitting telemetry
void sendPacket() {
	c.emergencyCount++;
	c.transmitFlag = true;

	if (c.releasing) {
		c.releaseCount++;
	}

	if (c.lastTwo && c.lastTwoCount < 2) {
		c.lastTwoCount++;
		c.transmitTelem();
	}
}



