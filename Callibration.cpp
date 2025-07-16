#include "Arduino.h"
#include "Callibration.h"

int acidvoltage, neutralvoltage;
extern float wTemp, voltage_ec, ECvalueRaw;


/**
 * @brief PH callibration function
 * 
 * @param PHrawValue PH ADC value
 * @return float Callibration value
 */

int Callibration::phcallifour(int PHrawValue) {

  float Voltage = PHrawValue / 4096.0 * 3300;
   Serial3.println((String)"acidvoltage =" + Voltage);


  if ((Voltage >= 1175) && (Voltage <= 1225)) {  //buffer solution:4.0

    acidvoltage = (int)Voltage;



    return acidvoltage;
  } else {
     Serial3.println((String)"acidvoltage =" + Voltage);
    return 1;
  }
}


int Callibration::phcalliseven(int PHrawValue) {

  float Voltage = PHrawValue / 4096.0 * 3300;
  Serial3.println((String)"neutralvoltage =" + Voltage);


  if ((Voltage >= 1000) && (Voltage <= 1050)) {


    neutralvoltage = (int)Voltage;



    return neutralvoltage;
  } else {
     Serial3.println((String)"neutralvoltage =" + Voltage);

    return 1;
  }
}


/**
 * @brief EC callibration
 * 
 * @return float EC callibration value
 */


float Callibration::ECcallibration() {
  static float rawECsolution;
  float KValueTemp = 0, kvalue = 0;




  if ((ECvalueRaw >= 0.3) && (ECvalueRaw <= 1.9)) {
    rawECsolution = 1.413 * (1.0 + 0.0185 * (wTemp - 25.0));
  } else {
     Serial3.println((String)"ECvalueRaw =" + ECvalueRaw);

    return 1;
  }

  KValueTemp = RES2 * ECREF * rawECsolution / 1000.0 / voltage_ec / 10.0;
   Serial3.println((String)"KValueTemp =" + KValueTemp);


  if ((KValueTemp >= 0.3) && (KValueTemp <= 10))

  {
    kvalue = KValueTemp;
    return kvalue;
  }
   else {


    return 1;
  }
}


