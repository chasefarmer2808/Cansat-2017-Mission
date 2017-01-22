//Code for Adafruit SD Card reader/writer breakout.

//Wiring (SPI):
/*
 * Arduino Nano     Breakout
 * 3V3              3V
 * GND              GND
 * SCK              CLK
 * MISO             DO
 * MOSI             DI
 * SS (low true     CS
 */



 #include <SPI.h>
 #include <SD.h>

 Sd2Card card;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
