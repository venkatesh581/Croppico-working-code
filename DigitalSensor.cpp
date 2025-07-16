#include "Arduino.h"
#include "DigitalSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS PB5

bool conversionRequested = false;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


#define ph_up_level PC_2
#define ph_down_level PC_3
#define ec_A_level PC_8
#define ec_B_level PA_15
#define water_level1 PA_2

float prevwater_Temp,WTemp;
int pHIncLev = 0, pHDecLev = 0, EcALev = 0, EcBLev = 0, wtemperror = 0;

/**
   @brief Water temperature function (ONE WIRE)

   @return float water temperature
*/
float DigitalSensor::waterTemp() {

   sensors.begin();
  sensors.requestTemperatures();
  if (sensors.isConversionComplete()) {
     WTemp = sensors.getTempCByIndex(0);
    
    }
  

  if (WTemp == DEVICE_DISCONNECTED_C) {
   
    wtemperror++;
    if (wtemperror == 100) {
      wtemperror = 0;
      prevwater_Temp = WTemp;
    }
    //Serial1.println("enter");
  
    return prevwater_Temp;
  }

  wtemperror = 0;
  prevwater_Temp = WTemp;
  return WTemp;
}

/**
   @brief Initializing the Level sensor

*/
void DigitalSensor::TrigSensor() {
  pinMode(ph_up_level, INPUT);
  pinMode(ph_down_level, INPUT);
  pinMode(ec_A_level, INPUT);
  pinMode(ec_B_level, INPUT);
  pinMode(water_level1, INPUT);

  //digitalWrite(water_level1, HIGH);

  //  digitalWrite(ph_down_level,HIGH);
  //  digitalWrite(ph_up_level,HIGH);
  //  digitalWrite(ec_A_level,HIGH);
  //  digitalWrite(ec_B_level,HIGH);
}

/**
   @brief pH Up solution level function

   @return int container Level
*/
int DigitalSensor::phUpLevel() {
  int pH_UP[10], countones = 0, countzeros = 0;


  for (int i = 0; i < 10; i++) {
    pH_UP[i] = digitalRead(ph_up_level);

    if (pH_UP[i] == 1) {
      countones++;
    } else {
      countzeros++;
    }
  }
  if (countones >= countzeros) {
    return 1;
  } else {
    return 0;
  }
}

/**
   @brief pH Down solution function

   @return int container Level
*/
int DigitalSensor::phDownLevel() {

  int pH_down[10], countones = 0, countzeros = 0;


  for (int i = 0; i < 10; i++) {
    pH_down[i] = digitalRead(ph_down_level);

    if (pH_down[i] == 1) {
      countones++;
    } else {
      countzeros++;
    }
  }
  if (countones >= countzeros) {
    return 1;
  } else {
    return 0;
  }
}

/**
   @brief EC supplement A level function

   @return int container Level
*/
int DigitalSensor::ec_1Level() {
  int EC_A[10], countones = 0, countzeros = 0;


  for (int i = 0; i < 10; i++) {
    EC_A[i] = digitalRead(ec_A_level);

    if (EC_A[i] == 1) {
      countones++;

    } else {
      countzeros++;
    }
  }

  if (countones >= countzeros) {
    return 1;
  } else {
    return 0;
  }
}


/**
   @brief EC supplement B level function

   @return int container Level
*/
int DigitalSensor::ec_2Level() {
  int EC_B[10], countones = 0, countzeros = 0;


  for (int i = 0; i < 10; i++) {
    EC_B[i] = digitalRead(ec_B_level);

    if (EC_B[i] == 1) {
      countones++;
    } else {
      countzeros++;
    }
  }
  if (countones >= countzeros) {
    return 1;
  } else {
    return 0;
  }
}

/**
   @brief Water level 1 in the system

   @return int water level
*/
int DigitalSensor::waterLevel() {
  int waterlvl[10], countones = 0, countzeros = 0;


  for (int i = 0; i < 10; i++) {
    waterlvl[i] = digitalRead(water_level1);

    if (waterlvl[i] == 1) {
      countones++;
    } else {
      countzeros++;
    }
  }
  if (countones >= countzeros) {
    return 1;
  } else {
    return 0;
  }
}