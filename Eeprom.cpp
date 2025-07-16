
#include "Arduino.h"

#include "Eeprom.h"
#include <stm32g0xx_hal.h>  // Adjust the include according to your STM32 series



extern int pumpOn, pumpOff, MinTemp, checkTime, phcalibration_seven, phcalibration_four, model, screentime, screenstatus;
extern float phLow, phHigh, Ph_PUMP, ph_sensor, EC_solution, ph_solution, ec_calibration;
extern int SetpointEc, EcDelta, ECSERROR, pumpEc,wtdreset,borreset;

volatile uint32_t pumpOnTime, pumpOffTime, pumpOffTime1,PumpOntime,PumpOfftime;
float thresholdMaxpH, thresholdMinpH, pH_Delta, check_errorPh, pumperror_ph, Ecconcentrationvalue, phconcentrationvalue, Kvaluelow, systemmodel;
int Ec_Delta, Setpoint_Ec, check_errorEc, pumperror_Ec, Set_Temp, doseCheckTime, Acidvoltage, Neutralvoltage, savertime, saverstatus,watchdogreset,BORreset;






void Eeprom::writeeeprom(void) {

  HAL_FLASH_Unlock();
  FLASH_EraseInitTypeDef eraseInitStruct;
  uint32_t pageError = 0;

  // Set up the erase parameters
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseInitStruct.Page = 62;
  //(FLASH_USER_START_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE;  // Adjust for your STM32 series
  eraseInitStruct.NbPages = 1;

  HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);  // Erase the Flash Memory


  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F000, pumpOn);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F008, pumpOff);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F010, phLow * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F018, phHigh * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F020, ph_sensor * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F028, Ph_PUMP * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F030, SetpointEc);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F038, EcDelta);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F040, ECSERROR);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F048, pumpEc);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F050, MinTemp);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F068, checkTime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F058, EC_solution * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F060, ph_solution * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F070, phcalibration_four);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F078, phcalibration_seven);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F080, ec_calibration * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F088, model);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F090, screentime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F098, screenstatus);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F0A0, wtdreset);
   HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F0A8, borreset);








  HAL_FLASH_Lock();
}

void Eeprom::readeeprom(void) {

  PumpOntime = *(__IO uint32_t*)0x801F000;
  pumpOnTime = PumpOntime * 60;


  PumpOfftime = *(__IO uint32_t*)0x801F008;
  pumpOffTime = PumpOfftime * 60;
   
  uint32_t MinpH = *(__IO uint32_t*)0x801F010;
  thresholdMinpH = ((float)MinpH / 100);
  uint32_t MaxpH = *(__IO uint32_t*)0x801F018;
  thresholdMaxpH = MaxpH / 100.0;
  uint32_t errorPh = *(__IO uint32_t*)0x801F020;
  check_errorPh = errorPh / 100.0;

  uint32_t pumperror = *(__IO uint32_t*)0x801F028;
  pumperror_ph = pumperror / 100.0;

  Setpoint_Ec = *(__IO uint32_t*)0x801F030;
  Ec_Delta = *(__IO uint32_t*)0x801F038;
  check_errorEc = *(__IO uint32_t*)0x801F040;
  pumperror_Ec = *(__IO uint32_t*)0x801F048;

  Set_Temp = *(__IO uint32_t*)0x801F050;
 
  uint32_t Ecconcentration = *(__IO uint32_t*)0x801F058;
  Ecconcentrationvalue = Ecconcentration / 100.0;
  uint32_t phconcentration = *(__IO uint32_t*)0x801F060;
  phconcentrationvalue = phconcentration / 100.0;
  doseCheckTime = *(__IO uint32_t*)0x801F068;

  Acidvoltage = *(__IO uint32_t*)0x801F070;
  Neutralvoltage = *(__IO uint32_t*)0x801F078;

  uint32_t K_valuelow = *(__IO uint32_t*)0x801F080;
  Kvaluelow = K_valuelow / 100.0;
  systemmodel = *(__IO uint32_t*)0x801F088;
  savertime = *(__IO uint32_t*)0x801F090;

  saverstatus = *(__IO uint32_t*)0x801F098;
  watchdogreset = *(__IO uint32_t*)0x801F0A0;
  BORreset = *(__IO uint32_t*)0x801F0A8;

}


void Eeprom::backupwriteeeprom(void) {


  HAL_FLASH_Unlock();
  FLASH_EraseInitTypeDef eraseInitStruct;
  uint32_t pageError = 0;

  // Set up the erase parameters
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseInitStruct.Page = 63;
  //(FLASH_USER_START_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE;  // Adjust for your STM32 series
  eraseInitStruct.NbPages = 1;

  HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);  // Erase the Flash Memory


  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F800, PumpOntime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F808, PumpOfftime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F810, thresholdMinpH * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F818, thresholdMaxpH * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F820, check_errorPh * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F828, pumperror_ph * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F830, Setpoint_Ec);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F838, Ec_Delta);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F840, check_errorEc);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F848, pumperror_Ec);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F850, Set_Temp);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F868, doseCheckTime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F858, Ecconcentrationvalue * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F860, phconcentrationvalue * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F870, Acidvoltage);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F878, Neutralvoltage);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F880, Kvaluelow * 100);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F888, systemmodel);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F890, savertime);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F898, saverstatus);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F8A0, watchdogreset);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 0x801F8A8, BORreset);








  HAL_FLASH_Lock();
}


void Eeprom::backupreadeeprom(void) {

  pumpOn = *(__IO uint32_t*)0x801F800;


  pumpOff = *(__IO uint32_t*)0x801F808;
     //Serial1.print((String) "" +" pumpOn" +pumpOn+"pumpOff" +pumpOff+"\n");

  uint32_t MinpH = *(__IO uint32_t*)0x801F810;
  phLow = ((float)MinpH / 100);
  uint32_t MaxpH = *(__IO uint32_t*)0x801F818;
  phHigh = MaxpH / 100.0;
  uint32_t errorPh = *(__IO uint32_t*)0x801F820;
  ph_sensor = errorPh / 100.0;

  uint32_t pumperror = *(__IO uint32_t*)0x801F828;
  Ph_PUMP = pumperror / 100.0;

  SetpointEc = *(__IO uint32_t*)0x801F830;
  EcDelta = *(__IO uint32_t*)0x801F838;
  ECSERROR = *(__IO uint32_t*)0x801F840;
  pumpEc = *(__IO uint32_t*)0x801F848;

  MinTemp = *(__IO uint32_t*)0x801F850;

  uint32_t Ecconcentration = *(__IO uint32_t*)0x801F858;
  EC_solution = Ecconcentration / 100.0;
  uint32_t phconcentration = *(__IO uint32_t*)0x801F860;
  ph_solution = phconcentration / 100.0;
  checkTime = *(__IO uint32_t*)0x801F868;

  phcalibration_four = *(__IO uint32_t*)0x801F870;
  phcalibration_seven = *(__IO uint32_t*)0x801F878;

  uint32_t K_valuelow = *(__IO uint32_t*)0x801F880;
  ec_calibration = K_valuelow / 100.0;
  model = *(__IO uint32_t*)0x801F888;
  screentime = *(__IO uint32_t*)0x801F890;

  screenstatus = *(__IO uint32_t*)0x801F898;

 wtdreset = *(__IO uint32_t*)0x801F8A0;
 borreset = *(__IO uint32_t*)0x801F8A8;



}