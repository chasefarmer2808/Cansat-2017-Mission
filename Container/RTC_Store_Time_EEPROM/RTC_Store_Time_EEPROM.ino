#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <EEPROM_Helper.h>
#include <Arduino.h>

/* WIRING INSTRUCTIONS
 *  RTC       ARDUINO
 *  ____      _______
 *  Vin    -  5v
 *  GND    -  
 *  
 *  GND
 *  SCL    -  SCL
 *  SDA    -  SDA
 */

bool timeSet=true;
long initialUnixTime, currentUnixTime;


RTC_DS1307 rtc;

void setup() {

#ifndef ESP8266
  while (!Serial);
#endif

  Serial.begin(9600);

  delay(3000);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  EEPROM_read(1,initialUnixTime);
  if(initialUnixTime==NULL){
    timeSet=false;
  }


  //bool timeSet=false;

}

void loop() {
  if (!timeSet){
      initialUnixTime=rtc.now().unixtime();
      EEPROM_write(1, initialUnixTime);
      timeSet=true;
  }
  EEPROM_read(1, initialUnixTime);
  currentUnixTime=rtc.now().unixtime();
  int missionTime=currentUnixTime-initialUnixTime;
  Serial.print("Initial Time: ");
  Serial.println(initialUnixTime);
  Serial.print("Mission Time: ");
  Serial.println(missionTime);

  if (Serial.available() > 0) {
      if(Serial.read()=='r'){
        initialUnixTime=NULL;
        EEPROM_write(1, initialUnixTime);
      }
  }

  delay(1000);

}

