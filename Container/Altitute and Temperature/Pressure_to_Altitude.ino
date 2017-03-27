#include <Wire.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_Sensor.h>

/*   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
*/

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Pressure Sensor Test"); Serial.println("");

}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure)
  {
    Serial.print("Pressure:   ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature:  ");
    Serial.print(temperature);
    Serial.println(" C");

    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:  ");
    Serial.print(bmp.pressureToAltitude(seaLevelPressure, event.pressure));
    Serial.println(" m");
    Serial.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);
 }
