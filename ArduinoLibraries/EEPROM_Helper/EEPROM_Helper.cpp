//EEPROM_Helper.cpp - Library for dynamic read/writing of any sized variable to EEPROM Memory

/*
#include "EEPROM_Helper.h"
#include "Arduino.h"

EEPROM_Helper::EEPROM_Helper()
{
}

EEPROM_Helper::template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

EEPROM_Helper::template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}
*/