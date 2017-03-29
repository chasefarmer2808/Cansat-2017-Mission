// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

/* WIRING INSTRUCTIONS
 *  RTC       ARDUINO
 *  ____      _______
 *  Vin    -  5v
 *  GND    -  GND
 *  SCL    -  SCL
 *  SDA    -  SDA
 */

DateTime initialTime;
bool timeSet;
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

#ifndef ESP8266
  while (!Serial);
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  bool timeSet=false;
}

void loop () {
    if (!timeSet){
      initialTime=rtc.now();
      timeSet=true;
    }
    DateTime currentTime = rtc.now();
    TimeSpan missionDateTime=(currentTime-initialTime);
    int missionTime=currentTime.unixtime()-initialTime.unixtime();
    
    Serial.println(missionTime);

    delay(1000);
}
