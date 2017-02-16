int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin=4;
float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux
 
void setup() 
{
  analogReference(DEFAULT); //
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}
 
void loop() 
{
  // read the raw value from the sensor:
  int rawValue = analogRead(sensorPin);    
  

  Serial.println(RawToLux(rawValue)); 
  delay(50);
  float lux=RawToLux(rawValue);
  if (lux<40){
    digitalWrite(ledPin, LOW);
  }else{
    digitalWrite(ledPin, HIGH);
  }
}
 
float RawToLux(int raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}
