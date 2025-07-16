#ifndef Callibration_h
#define Callibration_h

#include "Arduino.h"

#define RES2 (7500.0/0.66)
#define ECREF 200.0

class Callibration {
  public:
    int phcallifour(int PHrawValue);
     int phcalliseven(int PHrawValue);

    float ECcallibration();
    
};

#endif
