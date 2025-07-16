#ifndef DigitalSensor_h
#define DigitalSensor_h

#include "Arduino.h"

class DigitalSensor {
    public:
        void TrigSensor();
        int phUpLevel();
        int phDownLevel();
        int ec_1Level();
        int ec_2Level();
        int waterLevel();
        
        float waterTemp();
        
};

#endif
