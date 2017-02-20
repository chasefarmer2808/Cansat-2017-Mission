/*
 * Pin configuration for xbee FTDI breakout to audiono uno
 * 
 * Arduino Uno    Xbee FTDI
 * GND            VSS
 * 5V             VDD
 * Tx             DIN
 * Rx             DOUT
 */


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReference(DEFAULT); //
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // read the raw value from the sensor:
  int rawValue = analogRead(sensorPin);   

  Serial.println(RawToLux(rawValue)); 
  delay(50);
  float lux=RawToLux(rawValue);
  
  
  if (Serial.available()) {
    char data = char(Serial.read());
    Serial.print(data);
    Serial.println();
  }
}

float RawToLux(int raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}
