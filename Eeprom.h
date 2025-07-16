#ifndef Eeprom_h
#define Eeprom_h

#include "Arduino.h"




class Eeprom {
  public:
    void writeeeprom();
    void readeeprom();
     void backupwriteeeprom();
    void backupreadeeprom();
  
};

#endif