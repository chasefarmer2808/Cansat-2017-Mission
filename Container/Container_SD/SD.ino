#include <SD.h>
 
File myFile;
 
void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("Container.txt", FILE_WRITE);

  x = 1;
 
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to Container.txt...");
    myFile.print("Test");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
 
void loop() {
  String packet = String("3387,Container," + String(missionTime) + "," + String(packetCount) + "," + String(altitude) + "," + String(temperature) + "," + String(battVoltage) + "," + String(state);
  if (myFile) {
    Serial.print("Writing to Container.txt...");
    myFile.print(packet);
  // close the file:
    myFile.close();
    Serial.println("done.");
  // nothing happens after setup
}
}
