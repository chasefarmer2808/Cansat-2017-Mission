/*
 Name:		Container.ino
 Created:	3/29/2017 8:45:56 PM
 Author:	chase
*/

// the setup function runs once when you press reset or power the board
#include <Container.h>

Container c = Container();
float temp;

void setup() {
	Serial.begin(9600);
	if (!c.bmp.begin()) {
		Serial.print("here");
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	c.setBMP180Data();
	Serial.println(c.temperature);
	Serial.println(c.pressure);
	Serial.println(c.altitude);
	Serial.println();
	delay(1000);
}
