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
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char data = char(Serial.read());
    Serial.print(data);
    Serial.println();
  }
}
