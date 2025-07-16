#ifndef RelayInit_h
#define RelayInit_h

#include "Arduino.h"

class RelayInit {
public:
  void TrigOutput();
  void WaterPump(int state);
  void Chiller(int state);
  void pHDown(int state);
  void pHUp(int state);
  void ecPump_A(int state);
  void ecPump_B(int state);
  void Light1(int state);
  void Light2(int state);
  void Light3(int state);
  void Light4(int state);
  void Light5(int state);
  void pHPower(int state);
    void pHADCPower(int state);

  void EcPower(int state);
  void EcADCPower(int state);
  void solenoidIN(int state);
  void solenoidOut(int state);
    void sensor3v(int state);
      void sensor5v(int state);
      

  
};

#endif