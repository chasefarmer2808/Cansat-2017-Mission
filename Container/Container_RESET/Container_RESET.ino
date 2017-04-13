void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i <= 3; i++){
    EEPROM.write(i,0);
  }

}
