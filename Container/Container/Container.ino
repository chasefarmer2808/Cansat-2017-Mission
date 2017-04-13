/*
 Name:		Container.ino
 Created:	3/29/2017 8:45:56 PM
 Author:	chase
*/

// the setup function runs once when you press reset or power the board

#include <Container.h>
#include <TimerOne.h>

#define BAUD 9600

Container c = Container();

bool packetFlag=false;

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

  Timer1.initialize();
  Timer1.attachInterrupt(sendPacket); //Interrupt using a timer to send a packet every second
  
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

	if (command == 'r' && c.state == LAUNCH) {
		c.release();
		c.saveState(RELEASE);
	}

	if (command == 'l') {
		c.saveState(LAUNCH);
	}

	if (c.state != LAND) {
		c.createPacket();
		//xbee.println(c.packet);
	}


  if(packetFlag==true){
    xbee.println(c.packet);
    packetFlag=false;
  }

}

//ISR for RX
void processCommand() {
	if (xbee.available()) {
		command = xbee.read();
	}
}

void sendPacket(){
  packetFlag=true;
}



