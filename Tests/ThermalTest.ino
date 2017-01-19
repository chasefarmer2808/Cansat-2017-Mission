//Thermal test for the CanSat 2017 Mission
//Turn off heat at 60C
//Turn on heat at 55C
//Nominal temperature is within this range.
//side sensor status LEDs: Red  Green  Blue
//Pin numbers on nano:      12     11   10

//Bottom sensor status LEDs: Red  Green  Blue
//Pin numbers on nano:        9     8     7

#include <Wire.h>

int tmp1 = 0x48; //side sensor
int tmp2 = 0x49; //bottom sensor

int sideR = 12;
int sideG = 11;
int sideB = 10;

int botR = 9;
int botG = 8;
int botB = 7;

float minTemp = 55.0;  //celcius
float maxTemp = 60.0;  //celcius

void setup() {
  // put your setup code here, to run once:
  pinMode(sideR, OUTPUT);
  pinMode(sideG, OUTPUT);
  pinMode(sideB, OUTPUT);

  pinMode(botR, OUTPUT);
  pinMode(botG, OUTPUT);
  pinMode(botB, OUTPUT);
  
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float sideTemp = getCelcius(tmp1);
  float bottomTemp = getCelcius(tmp2);

  setSideLEDS(sideTemp);
  setBottomLEDS(bottemTemp);  
}

float getCelcius(int sensorAddr) {
  Wire.requestFrom(sensorAddr, 2);  //Tells sensor to load two bytes of data

  //Temperature register is 16 bits long.  Temperature data in 12 MS bits.  Last four bits are 0 padded.
  byte MSB = Wire.read();  //Get the first byte
  byte LSB = Wire.read();  //Get the second byte

  int temperature = ((MSB << 8) | LSB) >> 4;

  float celsius = temperature*0.0625;
  return celsius;
}

void setSideLEDS(float temp) {
  if (temp > maxTemp) {  //Too hot
    digitalWrite(sideR, HIGH);
    digitalWrite(sideG, LOW);
    digitalWrite(sideB, LOW);
  } else if (temp < minTemp) {  //Too cold
    digitalWrite(sideR, LOW);
    digitalWrite(sideG, LOW);
    digitalWrite(sideB, HIGH);
  } else if (temp >= minTemp && temp <= maxTemp) { //just right
    digitalWrite(sideR, LOW);
    digitalWrite(sideG, HIGH);
    digitalWrite(sideB, LOW);
  } else {
    digitalWrite(sideR, LOW);
    digitalWrite(sideG, LOW);
    digitalWrite(sideB, LOW);
  }
}

void setBottomLEDS(float temp) {
  if (temp > maxTemp) { //too hot
    digitalWrite(botR, HIGH);
    digitalWrite(botG, LOW);
    digitalWrite(botB, LOW);
  } else if (temp < minTemp) { //too cold
    digitalWrite(botR, LOW);
    digitalWrite(botG, LOW);
    digitalWrite(botB, HIGH);
  } else if (temp >= minTemp && temp <= maxTemp) { //just right
    digitalWrite(botR, LOW);
    digitalWrite(botG, HIGH);
    digitalWrite(botB, LOW);
  } else {
    digitalWrite(botR, LOW);
    digitalWrite(botG, LOW);
    digitalWrite(botB, LOW);
  }
}

