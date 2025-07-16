
#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Arduino.h"
#include "ErrorHandling.h"


#define RES2 (7500.0/0.66)
#define ECREF 200.0


class AnalogSensor {
  public:
    float getPHAnalogInput(float &Phvalue, byte &confidence,byte &threshold);
    int getEc(float &wTemp, byte &ecconfidence,byte &threshold);
    float GrowEnvironmentHum(float &Humvalue,byte &htdconfidence);
    float GrowEnvironmentTemp(float &growTempvalue,byte &tempconfidence);

  
};

#endif
