/** * @file main.ino
   @author Mohammed.K.Afridi (afridi.denvik@gmail.com)
   @brief Cropicco Firmware (Hydrophonics)
   @version 2.0
   @date 2022-03-16

   @copyright Copyright (c) 2022

*/
//Flash eeprom and PH neglect filter
// test

#include <EveryTimer.h>
#include <IWatchdog.h>

#include "AnalogSensor.h"
#include "PWM.h"                  //not used
#include "Eeprom.h"
#include "DigitalSensor.h"
#include "RelayInit.h"
#include "ErrorHandling.h"
#include "DoseTime.h"

#include "Callibration.h"
#include "HardwareSerial.h"
#include <SoftwareSerial.h>


Callibration callibration;
AnalogSensor analogSensor;
DigitalSensor digitalSensor;
Dosetime dosetime;
Eeprom EEprom;
RelayInit relayInit;
ErrorHandling errorHandling;

SoftwareSerial Serial1P{ PC9, PC10 };

EveryTimer timer;
PWM Pwm;
HardwareSerial Serial1{ PA10, PA9 };
HardwareSerial Serial3{ PD6, PD5 };




#define PERIOD_MS 5000
#define flow_sensor PA3
#define Dose_start 180000   //3 minutes
#define Calibration 30



int confTrigger = 0;
unsigned long startTime, endtime, timeOne, time2, endtime2;

unsigned char data[4] = {};
const int numReadings = 5;                                          // Number of readings to collect
float readings[numReadings], confidence_sum, confidenceavg, alpha;  // Array to store readings
int readIndex = 0, avgcnt, Phcount = 0;
;
float distance, prev_ultravalue, Wdistance, prevWdistance, autoprevWdistance, prev_avg, growTempvalue, Humvalue;
int ultrasonicerror, PHdowntrig, ecdosetrig, PHuptrig, ultrasonicnt;
byte confidence, ECconfidence, htdconfidence, tempconfidence;

extern bool PHdebugflag, ECdebugflag;

bool Ledinitflag = false, manualacklgeflag = false, autoackflag = false, initOffflag = true, flushstartflag = false, flushstopflag = false, loopholdflag = true, watertopupflag = false, waterfillOFFflag = false, intruptwaterfillflag = false, watertopuperrorflag = false, autowatertopuperrorflag = false, waterflusherrorflag = false;
;
bool OnFlag = true, OffFlag = false, pumprstart = false, solenoidOFFflag = false, solenoidONflag = false, Phvalueflag = true, ECvalueflag = true;
bool phDcycleFlag = true, phUPcycleFlag = true, ecFlag = true, testFlag1 = false, testFlag = false, testFlag2 = false, testFlag3 = false, testFlag4 = false, testFlag5 = false, dayDownFlag = true, dayUPFlag = true, Phcountstartflag = true, PhTfourstart = false;
bool loopHold = false;
bool writeFlag = true, startFlag = true, isEcDosing = false, ectrig1 = true;
bool phPwrFlag = false, ecPwrFlag = false, PhRLYflag = false, ECRLYflag = true, maintflag = false, ledONflag = false, sensor_3vflag = true, sensor_5vflag = false;
bool ecDosage = false, pHupDosage = false, pHdownDosage = false, PHdownflag3 = true, PHdownflag2 = false, PHdownflag1 = false, PHupflag3 = true, PHupflag2 = false, PHupflag1 = false;
bool errorFlag = false;
bool maintWaterFlag = false, maintChillerFlag = false, maintECFlag = false, maintpHUpFlag = false, maintpHDownFlag = false;
bool ecPumpBDosageFlag = false, phPumpDOWNDosageFlag = false, phPumpUPDosageFlag = false, intrflag = true, confidenceerror = false;
;
;
bool ecBDosageOffFlag = false, ovrflowflag = true;
bool ChillerOffFlag = false, ECDosageTrig = true, waterclogflag = false;

extern bool neutralvoltage_wflag, acidvoltage_wflag, Eclow_flag;

long initCheck = 0;
long int lightStat;

byte threshold = 5;




float minFlow, checkSum = 0, CRCcheck = 0, l_hour = 0;
float growTemp, growHum, wTemp, pH, ec, prevPH;
float ECAX, ECBJ;
float tempPH, pHTempData, tempEC, EcTempData, CalibData;


volatile unsigned long long i = 0, phTemp = 0, phTemp1 = 0, phTemp2 = 0, phTemp3 = 0, ecTemp = 0, ecTemp1 = 0, PhHrstemp = 0, Downdosewaittime = 0, updosewaittime;
volatile unsigned long long pumpEOnTemp, pumpEOffTemp, solenoidOnTemp, waterfillTemp, waterflushTemp;


extern volatile uint32_t pumpOnTime, pumpOffTime, pumpOffTime1;  //////////
extern float thresholdMaxpH, thresholdMinpH, pH_Delta, check_errorPh, pumperror_ph, Ecconcentrationvalue, phconcentrationvalue, systemmodel;
extern int Ec_Delta, Setpoint_Ec, check_errorEc, pumperror_Ec, Set_Temp, doseCheckTime, saverstatus, savertime, watchdogreset, BORreset;

unsigned long long phInit = 0, ecInit = 0, phOffTimer = 0, ecOffTimer = 0, Ledinit = 0, autoackinit = 0, Timer_5v = 0, Timer_3v = 0, maintpHDowntime, maintectime, maintpHuptime;
unsigned long long currentTime, watertopupcheck, autowatertopupcheck, waterflushcheck;
unsigned long long cloopTime;
unsigned long long ecBDosageOnTime = 0, CurrentECDosageTime = 0, ChillerOffTime = 0, CurrentPHDosageTime = 0, CurrentPHUPDosageTime = 0;
// Number of pH samples to average
extern uint32_t Total_PH, Total_EC;
uint32_t ECvalue;
int error, len, arr[20], msgType, delimiter = 0, wltrs, highFlowCount = 0, wtdreset, borreset;
int maint_mode, light_mode, setting_mode;
int light_temp, maint_temp,maint_ecseconds,maint_pHUpseconds,maint_pHDownseconds;
int ecMin = 0;
int Phinccnt = 0, Phdeccnt = 0, Eccnt = 0, pHTError = 0, ECTError = 0, HumErrorcnt, temperrorcnt;

int stat1 = 1, stat2 = 1, stat3 = 1, stat4 = 1, stat5 = 1, waterflushstat = 0, watertopupstat = 0;
int chillerStat = 0;

int WaterPumpError = 0, FlowSensorError = 0, flowSensorError = 0, waterChillerError = 0, waterTempError = 0, envTempError = 0, pHError = 0, ECError = 0, pHIncPumpError = 0, pHDecPumpError = 0, ECPumpError = 0, transpirationError = 0, DropECError = 0, DropPHError = 0, autosolenoiderror = 0, pherror = 0;
int wLevel, ecLevel_1, ecLevel_2, pHUp, pHDown, wLevel1;
int frequency = 100;
int dutyCycle = 100;



int calibration_mode, calibresult, pump_mode;
volatile int irrigationStatus = 0;
int ChillerTrig = 0;
int phdownDosageFlag = 0, ecADosageFlag = 0, ecBDosageFlag = 0, phupDosageFlag = 0;
int ECDosageProcessTrig = 0;
int pHDosageProcessTrig = 0;

volatile int analogLed;
volatile int flow_frequency;
float totalraw = 0, Ledraw, AvgLedvalue, Ledvoltage;
extern int errorData, prevStage, PHrawValue;
int pumpOn, pumpOff, prevOn, prevOff, MinTemp, checkTime, phcalibration_four, phcalibration_seven, model, screentime, screenstatus;  /////
int SetpointEc, EcDelta, ECSERROR, pumpEc;


float wFlow, previouswflowValue = 1, waterflowvalue;

float EC_A_Pumpcal, EC_B_Pumpcal, EC_B_pumpON, EC_A_pumpON;
float phLow, phHigh, PHdelta, Ph_PUMP, ph_sensor, EC_solution, ph_solution, ec_calibration;



String inString = "";
extern String debugPHString, debugECString;

String SuccessMessage = "88,98,1", FailureMessage = "99,109,0", debuging = "";



struct result {
  int calibrationflag = 0;
  int flushresult = 0;
  int waterfillresult = 0;
};
result Result;

struct Versions {
  int versionId = 3;
  int versionSubId = 0;
  int board_ver=229;
  int board_subver=68; //Ascii value D
};
Versions Version;

struct constant {
  int key = 10;
  int PayloadMessage = 11;
  int DebugingPHMessage = 12;
  int DebugingECMessage = 13;
  int SettingMessage = 55;
  int ErrorMessage = 99;
  int RestartCMD = 77;
  int versionCMD = 33;
  int ECDosageTime = 240;  //nedd to change 300
  int PHDosageTime = 240;
  int SensorPowerTime = 5000;
  int DataFetchTime = 15000;
  int callibrationresult = 66;
  int calibstatus = 88;
  int errorreset = 44;
  int useacknowledgement = 46;
  int logPH = 89;
  int solenoidOnTime = 1200;
  int solenoidOFFTime = 660;  ////need to change 11 min
  int flush = 45;
  int Water_Topup = 46;
};
constant Constant;


/**
 * @brief Serial interrupt  from Pi to controller
 * 
 */


void serialEventRun() {
  while (Serial1.available()) {

    int input = Serial1.read();
    inString += (char)input;

    if (input == '\n') {

      len = inString.length();

      for (int i = 0; i < len; i++) {
        if (inString[i] == ',')

        {
          delimiter++;
        }
      }

      for (int i = 0; i < delimiter; i++) {

        String check_byte = getValue(inString, ',', i);
        float j = check_byte.toFloat();
        checkSum = checkSum + j;
      }

      String check_byte1 = getValue(inString, ',', delimiter);
      float CRC_Rx = check_byte1.toFloat();


      if (CRC_Rx == (checkSum + Constant.key)) {

        String part02 = getValue(inString, ',', 1);
        msgType = part02.toInt();

        if (msgType == Constant.PayloadMessage) {

          CRCcheck = CrcMessage();
          MessagePayload();
        }

        if (msgType == Constant.versionCMD) {


          CRCcheck = CrcVersion();
          versionPayload();
        }

        if (msgType == Constant.SettingMessage) {

          CRCcheck = CrcSetting();
          SettingPayload();
        }

        // if (msgType == Constant.RestartCMD) {
        //   resetFunc();
        // }

        if (msgType == Constant.errorreset) {

          Errorresetfunc();
          Serial1.print((String) "" + SuccessMessage + "\n");
        }

        // if (msgType == Constant.useacknowledgement) {
        //   manualacklgeflag = true;
        //   useacknowledgementfunc();
        //   Serial1.print((String) "" + SuccessMessage + "\n");
        // }

        if ((msgType == Constant.flush) /* && (systemmodel == 0)*/) {
          String part = getValue(inString, ',', 2);
          int flushstop = part.toInt();

          if (flushstop == 1) {
            waterflusherrorflag = true;
            flushstart();


          } else if (flushstop == 0) {
            forcestop();
          } else if (flushstop == 3) {
            if ((Result.flushresult == 1) || (Result.flushresult == 2)) {

              CRCcheck = Crcflushresult();
              flushpayload();
            } else {
              CRCcheck = Crcflushresult();
              flushpayload();
            }
          }

          else if (flushstop == 4) {
            if ((Result.flushresult == 1) || (Result.flushresult == 2)) {
              Result.waterfillresult = 0;
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }
        }

        // Serial1.print((String) "" + SuccessMessage + "\n");



        if ((msgType == Constant.Water_Topup) /* && (systemmodel == 0)*/) {
          String part = getValue(inString, ',', 2);
          int water_topup = part.toInt();

          if (water_topup == 1) {

            if ((!wLevel) || (waterclogflag)) {  //no water like system off
              watertopuperrorflag = true;
              watertopup();
              waterclogflag = false;

            }


            else if ((intruptwaterfillflag == true) && (systemmodel == 1)) {  // manual system water topup
              watertopuperrorflag = true;
              watertopup();

              // Result.waterfillresult = 0;
              // intruptwaterfillflag = false;
            }

            else if ((intruptwaterfillflag == false) && (systemmodel == 1)) {  //
              Result.waterfillresult = 2;
            }

          } else if (water_topup == 0) {
            waterfillforcestop();

          } else if (water_topup == 3) {
            if ((Result.waterfillresult == 1) || (Result.waterfillresult == 2) || (Result.waterfillresult == 3)) {

              CRCcheck = Crcwaterfillresult();
              waterfillpayload();
            }

            else {
              CRCcheck = Crcwaterfillresult();
              waterfillpayload();
            }
          } else if (water_topup == 4) {
            if ((Result.waterfillresult == 1) || (Result.waterfillresult == 2) || (Result.waterfillresult == 3)) {
              Result.waterfillresult = 0;
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }
        }




        if (msgType == Constant.ErrorMessage) {

          CRCcheck = CrcError();
          ErrorPayload();
        }
        if (msgType == Constant.DebugingPHMessage) {

          if (PHdebugflag) {

            CRCcheck = CrcPHDebug();
            DebugPHPayload();
            PHdebugflag = false;
          }
        }


        if (msgType == Constant.DebugingECMessage) {

          if (ECdebugflag) {

            CRCcheck = CrcECDebug();
            DebugECPayload();
            ECdebugflag = false;
          }
        }


        if (msgType == Constant.logPH) {

          String part = getValue(inString, ',', 2);
          int part1 = part.toInt();

          if (part1 == 1) {

            phupDosageFlag = 0;
            Serial1.print((String) "" + SuccessMessage + "\n");
          }

          if (part1 == 2) {

            phdownDosageFlag = 0;
            Serial1.print((String) "" + SuccessMessage + "\n");
          }
        }



        if (msgType == Constant.callibrationresult) {
          if ((Result.calibrationflag == 1) || (Result.calibrationflag == 2)) {
            //Serial1.print("sucess");
            CRCcheck = Crccallibration();
            callibrationpayload();
          }
        }


        if (msgType == Constant.calibstatus) {
          Serial1.print((String) "" + SuccessMessage + "\n");
          String part02 = getValue(inString, ',', 2);

          calibresult = part02.toInt();

          if (calibresult == 1) {
            Result.calibrationflag = 0;
          }
        }


        if (msgType == 0) {
          String code = getValue(inString, ',', 2);
          String data = getValue(inString, ',', 3);
          String seconds = getValue(inString, ',', 4);
          maint_mode = code.toInt();
          maint_temp = data.toInt();

          if (maint_mode == 0) {
            if (maint_temp == 0) {
              // Serial1.println("Off");
              maintflag = false;
              // loopHold = false;
              // phPwrFlag = true;
              // ecPwrFlag = false;
              // phInit = millis();
              // phOffTimer = phInit;
              // ecInit = phInit;
              // ecOffTimer = phInit;
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              // Serial1.println("On");
              maintflag = true;
              // loopHold = true;
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }

          if (maint_mode == 1) {
            if (maint_temp == 0) {
              relayInit.WaterPump(0);
              irrigationStatus = 0;
              maintWaterFlag = false;
              // Serial1.println("Water pump Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              relayInit.WaterPump(1);
              irrigationStatus = 1;

              maintWaterFlag = true;
              // Serial1.println("Water pump On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }

          if (maint_mode == 2) {
            if (maint_temp == 0) {
              relayInit.Chiller(0);

              chillerStat = 0;
              maintChillerFlag = false;
              // Serial1.println("Water Chiller Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              relayInit.Chiller(1);
              chillerStat = 1;
              // Serial1.println("maint");
              //Serial1.print((String)"" + "maint on" );
              maintChillerFlag = true;
              // Serial1.println("Water Chiller On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }


          if (maint_mode == 3) {
            if (maint_temp == 0) {
              relayInit.ecPump_A(0);
              relayInit.ecPump_B(0);
              ecADosageFlag = 0;
               ecBDosageFlag=0;
              maintECFlag = false;
              // Serial1.println("Supplement EC - A&B Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              relayInit.ecPump_A(1);
              relayInit.ecPump_B(1);
              ecADosageFlag = 1;
               ecBDosageFlag=1;
              maint_ecseconds=seconds.toInt();

              maintECFlag = true;
              maintectime = millis();
              // Serial1.println("Supplement EC - A&B On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }


          if (maint_mode == 4) {
            if (maint_temp == 0) {
              relayInit.pHUp(0);
              phupDosageFlag = 0;
              maintpHUpFlag = false;
              // Serial1.println("pH increase Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              relayInit.pHUp(1);
              phupDosageFlag = 1;
              maint_pHUpseconds=seconds.toInt();

              maintpHUpFlag = true;
              maintpHuptime = millis();
              // Serial1.println("pH increase On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }


          if (maint_mode == 5) {
            if (maint_temp == 0) {
              relayInit.pHDown(0);
               phdownDosageFlag = 0;
              maintpHDownFlag = false;
              // Serial1.println("pH decrease Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              maint_pHDownseconds=seconds.toInt();
              phdownDosageFlag = 1;
              relayInit.pHDown(1);
              maintpHDownFlag = true;
              maintpHDowntime = millis();
              // Serial1.println("pH decrease On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }


          if (maint_mode == 7) {
            if (maint_temp == 0) {
              relayInit.solenoidIN(0);

              //   maintECFlag = false;
              // Serial1.println("Supplement EC - A&B Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {
              relayInit.solenoidIN(1);

              //maintECFlag = true;
              // Serial1.println("Supplement EC - A&B On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }


          if (maint_mode == 6) {
            if (maint_temp == 0) {

              relayInit.solenoidOut(0);
              // maintECFlag = false;
              // Serial1.println("Supplement EC - A&B Off");
              Serial1.print((String) "" + SuccessMessage + "\n");
            } else {

              relayInit.solenoidOut(1);
              //  maintECFlag = true;
              // Serial1.println("Supplement EC - A&B On");
              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }
        }


        if (msgType == 1) {
          //      Serial1.println("Lights mode");
          String code = getValue(inString, ',', 2);
          String data = getValue(inString, ',', 3);
          light_mode = code.toInt();
          light_temp = data.toInt();
          if (light_mode == 0) {
            dutyCycle = light_temp;
            Pwm.TrigPwm(frequency, dutyCycle);
            Serial1.print((String) "" + SuccessMessage + "\n");
          }
          if (light_mode == 255) {
            if (light_temp == 0) {
              relayInit.Light1(0);
              relayInit.Light2(0);
              relayInit.Light3(0);
              relayInit.Light4(0);
              relayInit.Light5(0);
              stat1 = 0;
              stat2 = 0;
              stat3 = 0;
              stat4 = 0;
              stat5 = 0;
              lightStat = 99999;
              Serial1.print((String) "" + SuccessMessage + "\n");



            } else {

              ledONflag = true;

              Serial1.print((String) "" + SuccessMessage + "\n");
            }
          }
          if (light_mode == 1) {
            if (light_temp == 0) {
              if (stat1 == 1) {
                relayInit.Light1(0);
                stat1 = 0;
                lightStat = lightStat + 80000;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            } else {
              if (stat1 == 0) {
                relayInit.Light1(1);
                stat1 = 1;

                lightStat = lightStat - 80000;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }
          }
          if (light_mode == 2) {
            if (light_temp == 0) {
              if (stat2 == 1) {
                relayInit.Light2(0);
                stat2 = 0;

                lightStat = lightStat + 8000;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            } else {
              if (stat2 == 0) {
                relayInit.Light2(1);
                stat2 = 1;
                lightStat = lightStat - 8000;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }
          }
          if (light_mode == 3) {
            if (light_temp == 0) {
              if (stat3 == 1) {
                relayInit.Light3(0);
                stat3 = 0;
                lightStat = lightStat + 800;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            } else {
              if (stat3 == 0) {
                relayInit.Light3(1);
                stat3 = 1;
                lightStat = lightStat - 800;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }
          }
          if (light_mode == 4) {
            if (light_temp == 0) {
              if (stat4 == 1) {
                relayInit.Light4(0);
                stat4 = 0;
                lightStat = lightStat + 80;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            } else {
              if (stat4 == 0) {
                relayInit.Light4(1);
                stat4 = 1;
                lightStat = lightStat - 80;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }
          }
          if (light_mode == 5) {
            if (light_temp == 0) {
              if (stat5 == 1) {
                relayInit.Light5(0);
                stat5 = 0;
                lightStat = lightStat + 8;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            } else {
              if (stat5 == 0) {
                relayInit.Light5(1);
                stat5 = 1;
                lightStat = lightStat - 8;
                Serial1.print((String) "" + SuccessMessage + "\n");
              } else {
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }
          }
        }
        if (msgType == 2) {
          //      Serial1.println("Setting mode");
          String code = getValue(inString, ',', 2);
          setting_mode = code.toInt();

          if (setting_mode == 0) {
            EEprom.writeeeprom();
            delay(10);
            EEprom.readeeprom();
            delay(10);
            EEprom.backupwriteeeprom();
            delay(10);
            EEprom.backupreadeeprom();
            delay(10);
          }


          if (setting_mode == 1) {
            String data1 = getValue(inString, ',', 3);
            String data2 = getValue(inString, ',', 4);
            pumpOn = data1.toInt();
            pumpOff = data2.toInt();
            //Serial1.print((String) "" + " pumpOn" + pumpOn + "\n");


            EEprom.writeeeprom();
            delay(10);
            EEprom.readeeprom();
            delay(10);
            EEprom.backupwriteeeprom();
            delay(10);
            EEprom.backupreadeeprom();
            delay(10);


            Serial1.print((String) "" + SuccessMessage + "\n");
          }


          if (setting_mode == 2) {

            String data1 = getValue(inString, ',', 3);
            String data2 = getValue(inString, ',', 4);
            String data3 = getValue(inString, ',', 5);
            String data4 = getValue(inString, ',', 6);

            phLow = data1.toFloat();
            phHigh = data2.toFloat();
            ph_sensor = data3.toFloat();
            Ph_PUMP = data4.toFloat();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();


            Serial1.print((String) "" + SuccessMessage + "\n");
          }


          if (setting_mode == 3) {
            String data1 = getValue(inString, ',', 3);
            String data2 = getValue(inString, ',', 4);
            String data3 = getValue(inString, ',', 5);
            String data4 = getValue(inString, ',', 6);

            SetpointEc = data1.toInt();
            EcDelta = data2.toInt();
            ECSERROR = data3.toInt();
            pumpEc = data4.toInt();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();




            Serial1.print((String) "" + SuccessMessage + "\n");
          }


          if (setting_mode == 4) {
            String data1 = getValue(inString, ',', 3);


            MinTemp = data1.toInt();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();


            Serial1.print((String) "" + SuccessMessage + "\n");
          }


          if (setting_mode == 5) {
            String data1 = getValue(inString, ',', 3);
            String data2 = getValue(inString, ',', 4);
            String data3 = getValue(inString, ',', 5);
            EC_solution = data1.toFloat();
            ph_solution = data2.toFloat();
            checkTime = data3.toInt();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();

            Serial1.print((String) "" + SuccessMessage + "\n");
          }


          if (setting_mode == 6) {
            String code = getValue(inString, ',', 3);
            calibration_mode = code.toInt();

            String data1 = getValue(inString, ',', 4);
            float CalibData = data1.toFloat();

            if (calibration_mode == 1) {
              if (CalibData == 4) {

                phcalibration_four = callibration.phcallifour(PHrawValue);
                // Serial1.println((String)"" + " phcalibration_four:" + phcalibration_four);
                Phvalueflag = true;


                if (phcalibration_four == 1.0) {
                  phcalibration_four = 1200;
                  Result.calibrationflag = 2;
                  EEprom.writeeeprom();
                  EEprom.readeeprom();
                  EEprom.backupwriteeeprom();
                  EEprom.backupreadeeprom();


                } else {
                  EEprom.writeeeprom();
                  EEprom.readeeprom();
                  EEprom.backupwriteeeprom();
                  EEprom.backupreadeeprom();


                  Result.calibrationflag = 1;
                  Serial1.print((String) "" + SuccessMessage + "\n");
                }
              } else if (CalibData == 7) {

                phcalibration_seven = callibration.phcalliseven(PHrawValue);
                //Serial1.println((String)"" + " phcalibration_seven:" + phcalibration_seven);
                Phvalueflag = true;


                if (phcalibration_seven == 1) {


                  phcalibration_seven = 1024;
                  Result.calibrationflag = 2;
                  EEprom.writeeeprom();
                  EEprom.readeeprom();

                } else {
                  EEprom.writeeeprom();
                  EEprom.readeeprom();
                  EEprom.backupwriteeeprom();
                  EEprom.backupreadeeprom();



                  Result.calibrationflag = 1;
                  Serial1.print((String) "" + SuccessMessage + "\n");
                }
              }
            }
            if (calibration_mode == 2) {
              ec_calibration = callibration.ECcallibration();
              ECvalueflag = true;
              //Serial1.println((String)"" + " ec_calibration:" + ec_calibration);



              if (ec_calibration == 1) {

                EEprom.writeeeprom();
                EEprom.readeeprom();
                EEprom.backupwriteeeprom();
                EEprom.backupreadeeprom();


                Result.calibrationflag = 2;


              } else {
                EEprom.writeeeprom();
                EEprom.readeeprom();
                EEprom.backupwriteeeprom();
                EEprom.backupreadeeprom();

                Result.calibrationflag = 1;
                Serial1.print((String) "" + SuccessMessage + "\n");
              }
            }

            Serial1.print((String) "" + SuccessMessage + "\n");
          }

          if (setting_mode == 8) {
            String code = getValue(inString, ',', 3);
            model = code.toInt();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();

            Serial1.print((String) "" + SuccessMessage + "\n");
          }

          if (setting_mode == 9) {

            String code = getValue(inString, ',', 3);
            screentime = code.toInt();
            String code1 = getValue(inString, ',', 4);
            screenstatus = code1.toInt();
            EEprom.writeeeprom();
            EEprom.readeeprom();
            EEprom.backupwriteeeprom();
            EEprom.backupreadeeprom();

            Serial1.print((String) "" + SuccessMessage + "\n");
          }
        }

        inString = "";
        //      Serial1.println("88,98");
        //      resetFunc();
      } else {
        inString = "";
        Serial1.print((String) "" + FailureMessage + "\n");
      }
      checkSum = 0;
      delimiter = 0;
    }
  }
}


void setup() {


  pinMode(flow_sensor, INPUT);
  analogWriteFrequency(200);
  digitalSensor.TrigSensor();
  relayInit.TrigOutput();
  HAL_Init();
  error = 0;
  EEprom.readeeprom();
  EEprom.backupwriteeeprom();
  EEprom.backupreadeeprom();

  analogReadResolution(12);
  analogReference(AR_DEFAULT);
  relayInit.solenoidOut(0);
  relayInit.solenoidIN(0);



  if (IWatchdog.isReset(true)) {
    //EEprom.backupreadeeprom();
    wtdreset += 1;

    EEprom.writeeeprom();
    EEprom.readeeprom();
    EEprom.backupwriteeeprom();
    EEprom.backupreadeeprom();
  }



  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST) == 1) {
    //EEprom.backupreadeeprom();
    borreset += 1;

    EEprom.writeeeprom();
    EEprom.readeeprom();
    EEprom.backupwriteeeprom();
    EEprom.backupreadeeprom();



    __HAL_RCC_CLEAR_RESET_FLAGS();
}

  Serial1.print("borreset");
  Serial1.println(borreset);

  Serial1.print("wtdreset");
  Serial.println(wtdreset);

  IWatchdog.begin(5000000);  //   
  attachInterrupt(flow_sensor, flow, RISING);  // Setup Interrupt
  currentTime = millis();
  cloopTime = currentTime;
  Serial1.println();
  relayInit.sensor3v(1);
  delay(50);
  wLevel1 = digitalSensor.waterLevel();
  if (wLevel1) {
    relayInit.WaterPump(1);
    irrigationStatus = 1;
  }

  LEDON();
  Serial1.begin(115200);
  Serial1P.begin(9600);
  Serial3.begin(9600);



  // lightStat = 11111;
  timer.Every(PERIOD_MS, action);
}



void LEDON() {
  lightStat = 99999;
  dutyCycle = 100;
  Pwm.TrigPwm(frequency, dutyCycle);

  relayInit.Light1(1);
  delay(300);
  lightStat = lightStat - 80000;


  relayInit.Light2(1);
  lightStat = lightStat - 8000;
  delay(300);






  relayInit.Light3(1);
  lightStat = lightStat - 800;
  delay(300);




  relayInit.Light4(1);
  lightStat = lightStat - 80;
  delay(300);




  relayInit.Light5(1);
  lightStat = lightStat - 8;
  delay(300);

  stat1 = 1;
  stat2 = 1;
  stat3 = 1;
  stat4 = 1;
  stat5 = 1;
}



/**
   @brief Software restart function

*/
void resetFunc() {
  void (*resetFunc)(void) = 0;
}



/**
   @brief It's a interrupt function for water flow sensor

*/
void flow()  // Interrupt function
{
  if (irrigationStatus) {
    if (flow_frequency <= 250) {
      flow_frequency++;
    }
  }
}

void processHumValue() {
  if (htdconfidence >= 60) {
    growHum = Humvalue;
    HumErrorcnt = 0;
  } else {

    HumErrorcnt++;
  }
}

void processtemp() {
  if (tempconfidence >= 60) {
    temperrorcnt = 0;
    growTemp = growTempvalue;

  } else {
    temperrorcnt++;
  }
}

void waterlvlfunc() {
  if ((wLevel1) && (Wdistance <= 11.9)) {
    wLevel = 2;
  } else if ((wLevel1) && (Wdistance >= 12)) {
    wLevel = 1;

  } else if (!wLevel1) {
    wLevel = 0;
  }
}

void read3vSensors() {
  if (sensor_3vflag) {
    relayInit.sensor3v(1);
    if (millis() >= Timer_3v + 5000) {
      wTemp = digitalSensor.waterTemp();
      Wdistance = Ultrasonic();
      wltrs = dosetime.waterltrs(Wdistance);
      wLevel1 = digitalSensor.waterLevel();
      waterlvlfunc();

      Humvalue = analogSensor.GrowEnvironmentHum(Humvalue, htdconfidence);
      processHumValue();
      growTempvalue = analogSensor.GrowEnvironmentTemp(growTempvalue, tempconfidence);
      processtemp();
      envTempError = errorHandling.humitidyErrorCheck(growHum, growTemp, HumErrorcnt, temperrorcnt);
      waterTempError = errorHandling.waterTempCheck(wTemp);
      Serial3.println((String) ": Wdistance = " + Wdistance );
      relayInit.sensor3v(0);
      sensor_3vflag = false;
      sensor_5vflag = true;
      Timer_5v = millis();
    }
  }
}

void read5vSensors() {
  if (sensor_5vflag) {
    relayInit.sensor5v(1);
    if (millis() >= Timer_5v + 5000) {
      wFlow = waterFlowRate();
      pHUp = digitalSensor.phUpLevel();
      pHDown = digitalSensor.phDownLevel();
      ecLevel_1 = digitalSensor.ec_1Level();
      ecLevel_2 = digitalSensor.ec_2Level();
      relayInit.sensor5v(0);
      sensor_5vflag = false;
      ecPwrFlag = true;
      ecOffTimer = millis();
    }
  }
}
void maintenance() {
  if ((maintECFlag) &&(millis() >= maintectime + (maint_ecseconds*1000) )) {

      relayInit.ecPump_A(0);
      relayInit.ecPump_B(0);
       ecADosageFlag = 0;
        ecBDosageFlag=0;
      maintECFlag = false;
    }
  if ((maintpHDownFlag) &&(millis() >= maintpHDowntime + (maint_pHDownseconds*1000))) {
      relayInit.pHDown(0);
         phdownDosageFlag = 0;
      maintpHDownFlag = false;
    }

  if ((maintpHUpFlag) &&(millis() >= maintpHuptime + (maint_pHUpseconds*1000))) {
      relayInit.pHUp(0);
      phupDosageFlag = 0;
      maintpHUpFlag = false; 
     
    }
}



void loop() {
  IWatchdog.reload();
  timereset();
  if (ledONflag) {
    LEDON();
    ledONflag = false;
  }

  if (loopholdflag) {
    maintenance();
    read3vSensors();
    read5vSensors();
    getECvalue();
    getPHvalue();

    if ((initOffflag) || (OffFlag) && (!ultrasonicerror)) {
      ecdosetrig = dosetime.ecDoseTime(wltrs, ec, Setpoint_Ec, Ec_Delta);
      PHuptrig = dosetime.phUpDoseTime(wltrs, pH, thresholdMinpH, thresholdMaxpH);
      PHdowntrig = dosetime.phDownDoseTime(wltrs, pH, thresholdMinpH, thresholdMaxpH);
    }
    transpirationError = errorHandling.TranspirationCheck(ec, Setpoint_Ec, wLevel);
    nutrientdosage();
    timer.Update();
    //Pwm.TrigPwm(frequency, dutyCycle);

  } else if (flushstartflag) {

    flushstop();

  }

  else if (watertopupflag) {
    waterfillstop();
  }
}

void processECValue(float ECvalue, byte confidence) {

  if (ECvalueflag) {
    ec = ECvalue;
    ECvalueflag = false;
  }

  if (confidence < 50) {
    ECTError++;
    return;

  } else if (confidence < 80) {
    alpha = 0.1;

  } else {
    alpha = 0.25;
  }
  ECTError = 0;
  ec = (int)(alpha * ECvalue + (1 - alpha) * ec);
  //   Serial3.println((String)"finalec =" + ec);

}






/**
 * @brief EC value function
 * 
 */

void getECvalue() {

  if (ecPwrFlag) {

    relayInit.EcPower(1);
    relayInit.EcADCPower(1);

    if ((millis() >= ecOffTimer + Constant.DataFetchTime)) {


      if (waterTempError) {
        float errortempvalue = 25;
        ECvalue = analogSensor.getEc(errortempvalue, ECconfidence, threshold);
        //Serial1.println("EC");
      } else {
        ECvalue = analogSensor.getEc(wTemp, ECconfidence, threshold);
      }
     // Serial3.println((String)"ECvalue =" + ECvalue);



      processECValue(ECvalue, ECconfidence);
      phPwrFlag = true;

      ecPwrFlag = false;

      ECError = errorHandling.ECErrorCheck(ec, wLevel, ECTError,check_errorEc);
      phOffTimer = millis();
      relayInit.EcPower(0);
      relayInit.EcADCPower(0);
    }
  }
}



void processPHValue(float phVal, byte confidence) {

  if (Phvalueflag) {
    pH = phVal;
    Phvalueflag = false;
  }

  if (confidence < 50) {
    pHTError++;
    return;

  } else if (confidence <= 80) {

    alpha = 0.1;


  } else {
    alpha = 0.25;
  }
  pHTError = 0;
  pH = alpha * phVal + (1 - alpha) * pH;
  Serial3.println((String) "  final pH =" + pH);
}



/**
 * @brief PH value function 
 * 
 */

void getPHvalue() {
  if (phPwrFlag)  
  {

    relayInit.pHPower(1);
    relayInit.pHADCPower(1);

    if ((millis() > phOffTimer + Constant.DataFetchTime)) {


      float Phvalue;
      Phvalue = analogSensor.getPHAnalogInput(Phvalue, confidence, threshold);
      Serial3.println((String) "Phvalue =" + Phvalue);
      processPHValue(Phvalue, confidence);
      pHError = errorHandling.pHErrorCheck(pH, pHTError,check_errorPh);


      // Serial2.print("PH :");
      // Serial2.println(pH);

      phPwrFlag = false;
      sensor_3vflag = true;
      Timer_3v = millis();



      relayInit.pHPower(0);
      relayInit.pHADCPower(0);
    }
  }
}


void timereset() {
  if ((millis() >= 4294967280) && (ovrflowflag)) {
    phTemp = 0;
    phTemp1 = 0;
    phTemp2 = 0;
    phTemp3 = 0;
    ecTemp = 0;
    ecBDosageOnTime = 0;
    ecTemp1 = 0;
    PhHrstemp = 0;
    ecOffTimer = 0;
    phOffTimer = 0;
    Timer_5v = 0;
    Timer_3v = 0;

    Serial1.println("check");
    ovrflowflag = false;
  }
}


void flushstart() {
  Wdistance = Ultrasonic();
  if (!ultrasonicerror) {
    Result.flushresult = 0;


    relayInit.solenoidOut(1);
    waterflushstat = 1;
    if (waterflusherrorflag) {
      //  Serial1.print("enter");
      prevWdistance = Wdistance;
      waterflushcheck = millis();
      waterflusherrorflag = false;
    }
    relayInit.Chiller(0);
    chillerStat = 0;
    loopholdflag = false;
    //ECDosageTrig = false;
    watertopupflag = false;

    flushstartflag = true;
    waterflushTemp = millis();
    relayInit.solenoidIN(0);
    watertopupstat = 0;
    relayInit.WaterPump(0);
    irrigationStatus = 0;
    relayInit.ecPump_A(0);
    ecBDosageFlag = 0;
    ecADosageFlag = 0;
    relayInit.pHDown(0);
    phupDosageFlag = 0;
    relayInit.pHUp(0);
    phdownDosageFlag = 0;
    relayInit.ecPump_B(0);
    relayInit.EcPower(0);
    relayInit.EcADCPower(0);
    relayInit.pHPower(0);
    relayInit.pHADCPower(0);
    relayInit.sensor5v(0);
  } else {
    Result.flushresult = 2;
    flushstartflag = false;
    waterflusherrorflag = true;
    relayInit.solenoidOut(0);
    waterflushstat = 0;
  }
}


void forcestop() {
  relayInit.solenoidOut(0);
  waterflushstat = 0;
  flushstartflag = false;
  waterflusherrorflag = true;
  //relayInit.Chiller(0);

  Serial1.print((String) "" + SuccessMessage + "\n");
}



void flushstop() {

  relayInit.sensor3v(1);
  delay(100);
  Wdistance = Ultrasonic();

  wltrs = dosetime.waterltrs(Wdistance);
  wLevel1 = digitalSensor.waterLevel();
  if ((wLevel1) && (Wdistance <= 11.9)) {
    wLevel = 2;
  } else if ((wLevel1) && (Wdistance >= 12)) {
    wLevel = 1;

  } else if (!wLevel1) {
    wLevel = 0;
  }
  //Serial3.println((String) "" + "wltrs:" + wltrs+"Wdistance:"+Wdistance+"wLevel1=" +wLevel1);


  if (ultrasonicerror) {
    Result.flushresult = 2;
    flushstartflag = false;
    waterflusherrorflag = true;
    relayInit.solenoidOut(0);
    waterflushstat = 0;
  }

  if (Wdistance > prevWdistance + 0.1) {
    prevWdistance = Wdistance;
    waterflushcheck = millis();
  }

  if ((millis() > waterflushcheck + 180000) && (!loopholdflag) && (!waterflusherrorflag)) {  //3min
    relayInit.solenoidOut(0);
    watertopupstat = 0;
    flushstartflag = false;
    waterflusherrorflag = true;

    Result.flushresult = 2;
  }

  if (millis() > waterflushTemp + 660000) {
    relayInit.solenoidOut(0);
    watertopupstat = 0;
    flushstartflag = false;
    waterflusherrorflag = true;

    Result.flushresult = 2;
  }



  if ((Wdistance >= 21.5) && (!wLevel)) {  ///
    relayInit.solenoidOut(0);
    waterflushstat = 0;
    Result.flushresult = 1;

    flushstartflag = false;
  }
}



void watertopup() {

  if ((!ultrasonicerror) && (!irrigationStatus)) {
    relayInit.sensor3v(1);
    delay(100);
    Wdistance = Ultrasonic();

    wltrs = dosetime.waterltrs(Wdistance);

    wLevel1 = digitalSensor.waterLevel();
    Result.waterfillresult = 0;
    relayInit.solenoidIN(1);
    watertopupstat = 1;

    loopholdflag = false;
    flushstartflag = false;
    watertopupflag = true;  ///this for water fill in loop

    waterfillOFFflag = true;
    relayInit.solenoidOut(0);
    waterflushstat = 0;
    waterfillTemp = millis();
    relayInit.WaterPump(0);
    irrigationStatus = 0;
    ecBDosageFlag = 0;
    ecADosageFlag = 0;
    relayInit.ecPump_A(0);
    relayInit.pHDown(0);
    phupDosageFlag = 0;

    relayInit.pHUp(0);
    phdownDosageFlag = 0;
    relayInit.ecPump_B(0);
    relayInit.EcPower(0);
    relayInit.EcADCPower(0);
    relayInit.pHPower(0);
    relayInit.pHADCPower(0);
    relayInit.sensor5v(0);
    relayInit.Chiller(0);
    chillerStat = 0;

  } else {

    relayInit.solenoidIN(0);
    watertopupstat = 0;
    Result.waterfillresult = 3;
    watertopupflag = false;
    waterclogflag = true;
  }
}



void waterfillstop() {

  Wdistance = Ultrasonic();

  wltrs = dosetime.waterltrs(Wdistance);
  wLevel1 = digitalSensor.waterLevel();
  if ((wLevel1) && (Wdistance <= 11.9)) {
    wLevel = 2;
  } else if ((wLevel1) && (Wdistance >= 12)) {
    wLevel = 1;

  } else if (!wLevel1) {
    wLevel = 0;
  }
  // Serial3.println((String) "" + "wltrs:" + wltrs+"Wdistance:"+Wdistance+"wLevel1=" +wLevel1);


  if (ultrasonicerror) {
    relayInit.solenoidIN(0);
    watertopupstat = 0;
    Result.waterfillresult = 3;
    watertopupflag = false;
    waterclogflag = true;
  }
  if (watertopuperrorflag) {
    prevWdistance = Wdistance;
    watertopupcheck = millis();
    watertopuperrorflag = false;
  }

  if (Wdistance < prevWdistance - 0.1) {
    prevWdistance = Wdistance;
    watertopupcheck = millis();
  }

  if ((millis() > watertopupcheck + 180000) && (!loopholdflag) && (!watertopuperrorflag)) {  //3min

    relayInit.solenoidIN(0);
    watertopupstat = 0;

    watertopupflag = false;
    Result.waterfillresult = 3;
    waterclogflag = true;
  }
  if (millis() > waterfillTemp + 720000) {
    waterfillTemp = millis();
    relayInit.solenoidIN(0);
    watertopupstat = 0;

    watertopupflag = false;
    Result.waterfillresult = 3;
    waterclogflag = true;
  }

  if (((Wdistance > 1) && (Wdistance <= 3.8)) && (!loopholdflag) && (wLevel) && (waterfillOFFflag)) {


    i = 0;
    solenoidOnTemp = 0;
    pumpEOnTemp = 0;
    pumpEOffTemp = 0;
    phTemp = millis();
    phTemp1 = millis();
    phTemp2 = millis();
    phTemp3 = millis();
    ecTemp = millis();
    CurrentECDosageTime = 0;
    ecBDosageOnTime = millis();
    ChillerOffTime = 0;
    ecTemp1 = millis();
    ECPumpError = 0;
    CurrentPHDosageTime = 0;
    CurrentPHUPDosageTime = 0;
    pHIncPumpError = 0;
    pHDecPumpError = 0;
    //PhHrstemp = 0;

    Phcountstartflag = true;
    phDcycleFlag = true;
    dayDownFlag = true;
    phUPcycleFlag = true;
    dayUPFlag = true;
    ecFlag = true;

    intruptwaterfillflag = false;
    relayInit.solenoidIN(0);
    watertopupstat = 0;

    pumpEOffTemp = i;
    pumpEOnTemp = i;
    OffFlag = false;
    OnFlag = true;
    relayInit.WaterPump(1);
    irrigationStatus = 1;
    intruptwaterfillflag = false;
    flushstartflag = false;
    watertopupflag = false;
    waterfillOFFflag = false;
    initCheck = millis();

    sensor_3vflag = true;
    sensor_5vflag = false;
    ecPwrFlag = false;
    phPwrFlag = false;
    Timer_3v = millis();
    Timer_5v = millis();
    ecOffTimer = millis();
    phOffTimer = millis();


    loopholdflag = true;
    Result.waterfillresult = 1;
  }
}

void waterfillforcestop() {
  relayInit.solenoidIN(0);
  watertopupstat = 0;
  watertopupflag = false;
  waterclogflag = true;
  Serial1.print((String) "" + SuccessMessage + "\n");
}


float Ultrasonic() {


  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Collect 10 readings
  for (int j = 0; j < numReadings; j++) {
    for (int i = 0; i < 4; i++) {
      data[i] = Serial1P.read();
    }

    Serial1P.flush();

    if (data[0] == 0xff) {
      int sum = (data[0] + data[1] + data[2]) & 0x00FF;

      if (sum == data[3]) {
        distance = (data[1] << 8) + data[2];
        if (distance > 0) {
          distance = (distance / 10);
          ultrasonicnt = 0;
          ultrasonicerror = 0;
        } else {
          distance = 0;
          ultrasonicnt++;
        }
      } else {
        distance = 0;
        ultrasonicnt++;
      }
    }

    if (ultrasonicnt == 10) {
      // ultrasonicnt++;
      ecdosetrig = 40;
      PHuptrig = 10;
      PHdowntrig = 10;
      ultrasonicnt = 0;
      ultrasonicerror = 1;
    }


    readings[readIndex] = distance;  // Store the reading
    readIndex++;                     // Move to the next index

    if (readIndex >= numReadings) {
      readIndex = 0;  // Reset index if it reaches the end of the array
    }

    delay(100);
  }

  // Find the most frequent value (mode)
  float modeValue = 0;
  int maxCount = 0;

  for (int i = 0; i < numReadings; i++) {
    int count = 0;
    for (int j = 0; j < numReadings; j++) {
      if (readings[j] == readings[i]) {
        count++;
      }
    }
    if (count > maxCount) {
      maxCount = count;
      modeValue = readings[i];
    }
  }

  // Calculate the average of the most frequent value
  float total = 0;
  int count = 0;

  for (int i = 0; i < numReadings; i++) {
    if (readings[i] == modeValue) {
      total += readings[i];
      count++;
    }
  }

  float average = 0;
  if (count > 0) {
    average = total / count;
  }

  if (average > 21.6) {
    avgcnt++;
    if (avgcnt == 10) {
      //Serial1.println(average);
      avgcnt = 0;
      prev_avg = average;
    }
    return prev_avg;
  } else {
    avgcnt = 0;
    prev_avg = average;
    return average;
  }
}







/**
 * @brief nutrient DOSE function
 * 
 */
void nutrientdosage() {
  if (!loopHold) {

    if ((i >= Downdosewaittime + 540) && (PHdownflag2)) {
      PHdownflag2 = false;
      PHdownflag3 = true;
    }

    if ((i >= updosewaittime + 540) && (PHupflag2)) {
      PHupflag2 = false;
      PHupflag3 = true;
    }


    if (initCheck + Dose_start < millis())  //60000
    {
      if ((Setpoint_Ec < ec) || (ECPumpError) || (!ecLevel_1) || (!ecLevel_2)) {

        isEcDosing = false;

      } else {

        isEcDosing = true;
        Phcountstartflag = true;
        phDcycleFlag = true;
        dayDownFlag = true;
        phUPcycleFlag = true;
        dayUPFlag = true;
      }
      if ((pH > (thresholdMaxpH)) && (isEcDosing == false)) {

        if ((phDcycleFlag) && (wLevel) && (!pHError) && (!pHDecPumpError) && (pHDown) && (dayDownFlag)) {
          if (PHdownflag1) {
            Downdosewaittime = i;

            PHdownflag1 = false;
            PHdownflag2 = true;
          }
          if (PHdownflag3) {
            PHdownflag3 = false;
            initOffflag = false;
            intruptwaterfillflag = false;
            pHTempData = pH;
            dayUPFlag = false;
            OnFlag = false;
            OffFlag = false;
            relayInit.WaterPump(0);
            irrigationStatus = 0;

            phDown();

            phdownDosageFlag = 1;

            phDcycleFlag = false;
          }
        }
      } else {
        if (PHdownflag2) {
          PHdownflag2 = false;
          PHdownflag1 = true;
        }
      }
      if ((millis() >= phTemp + (PHdowntrig * 1000)) && (testFlag == true)) {

        ECDosageTrig = false;
        relayInit.pHDown(0);
        //phdownDosageFlag = 0;
        // if (!waterChillerError) {
        relayInit.Chiller(1);
        chillerStat = 1;
        // }



        CurrentPHDosageTime = i;
        phPumpDOWNDosageFlag = true;
        testFlag = false;
      }
      if ((i >= Constant.PHDosageTime + CurrentPHDosageTime) && (phPumpDOWNDosageFlag == true)) {
        ECDosageTrig = true;

        relayInit.Chiller(0);
        //Serial1.println("PH");
        chillerStat = 0;


        pHdownDosage = true;



        pumpEOffTemp = i;
        pumpEOnTemp = i;
        OffFlag = false;
        OnFlag = true;
        relayInit.WaterPump(1);
        irrigationStatus = 1;


        testFlag1 = true;

        if (Phcountstartflag) {
          PhHrstemp = millis();
          PhTfourstart = true;
          Phcountstartflag = false;
        }


        phTemp1 = millis();
        phPumpDOWNDosageFlag = false;
      }

      if ((millis() >= (phTemp1 + (doseCheckTime * 60000))) && (testFlag1 == true)) {
        phDcycleFlag = true;
        PHdownflag1 = true;
        testFlag1 = false;
        ovrflowflag = true;
      }

      if ((pH < (thresholdMinpH)) && (isEcDosing == false)) {
        if (phUPcycleFlag && wLevel && (!pHError) && (!pHIncPumpError) && pHUp && dayUPFlag) {

          if (PHupflag1) {
            updosewaittime = i;

            PHupflag1 = false;
            PHupflag2 = true;
          }
          if (PHupflag3) {
            PHupflag3 = false;
            initOffflag = false;
            intruptwaterfillflag = false;

            pHTempData = pH;
            dayDownFlag = false;
            OnFlag = false;
            OffFlag = false;
            relayInit.WaterPump(0);
            irrigationStatus = 0;
            phUp();
            phupDosageFlag = 1;
            phUPcycleFlag = false;
          }
        }
      } else {
        if (PHupflag2) {
          PHupflag2 = false;
          PHupflag1 = true;
        }
      }
      if ((millis() >= (phTemp2 + (PHuptrig * 1000))) && (testFlag2 == true)) {

        relayInit.pHUp(0);

        ECDosageTrig = false;


        //if (!waterChillerError) {
        relayInit.Chiller(1);
        chillerStat = 1;
        // }

        CurrentPHUPDosageTime = i;
        phPumpUPDosageFlag = true;
        testFlag2 = false;
      }

      if ((i >= Constant.PHDosageTime + CurrentPHUPDosageTime) && (phPumpUPDosageFlag == true)) {
        ECDosageTrig = true;

        relayInit.Chiller(0);
        chillerStat = 0;


        pHupDosage = true;

        pumpEOffTemp = i;
        pumpEOnTemp = i;
        OffFlag = false;
        OnFlag = true;
        relayInit.WaterPump(1);
        irrigationStatus = 1;

        testFlag3 = true;

        if (Phcountstartflag) {
          PhHrstemp = millis();
          PhTfourstart = true;

          Phcountstartflag = false;
        }

        phTemp3 = millis();
        phPumpUPDosageFlag = false;
      }
      if ((millis() >= (phTemp3 + (doseCheckTime * 60000))) && (testFlag3 == true)) {
        phUPcycleFlag = true;
        testFlag3 = false;
        PHupflag1 = true;
        ovrflowflag = true;
      }

      if ((millis() > PhHrstemp + 86400000) && (PhTfourstart)) {

        PhHrstemp = millis();


        if (!dayUPFlag) {
          dayUPFlag = true;
          phUPcycleFlag = true;
        }
        if (!dayDownFlag) {
          dayDownFlag = true;
          phDcycleFlag = true;
        }
        Phcountstartflag = true;
        PhTfourstart = false;
      }


      if (ec >= (Setpoint_Ec - Ec_Delta)) {
        ectrig1 = false;
      }

      if (ec < (Setpoint_Ec - (2 * Ec_Delta))) {
        ectrig1 = true;
      }

      if (((ec < (Setpoint_Ec - (2 * Ec_Delta))) || (ec < (Setpoint_Ec - Ec_Delta))) && (ectrig1 == true)) {

        if (ecFlag && wLevel && (!ECError) && (!ECPumpError) && ecLevel_1 && ecLevel_2 && isEcDosing) {
          initOffflag = false;
          intruptwaterfillflag = false;

          OnFlag = false;
          OffFlag = false;
          relayInit.WaterPump(0);
          irrigationStatus = 0;
          EcTempData = ec;


          ecTrigger();
          ecADosageFlag = 1;
        }
      }


      if ((millis() >= (ecTemp + (ecdosetrig * 1000))) && (testFlag4 == true)) {


        relayInit.ecPump_A(0);
        ecADosageFlag = 0;
        ECDosageTrig = false;

        // if (!waterChillerError) {
        relayInit.Chiller(1);
        chillerStat = 1;
        // }


        CurrentECDosageTime = i;
        ecPumpBDosageFlag = true;
        testFlag4 = false;
      }

      if ((i >= Constant.ECDosageTime + CurrentECDosageTime) && (ecPumpBDosageFlag == true)) {

        relayInit.Chiller(0);

        chillerStat = 0;
        relayInit.ecPump_B(1);
        ecBDosageFlag = 1;
        ecBDosageOnTime = millis();
        ecBDosageOffFlag = true;
        ecPumpBDosageFlag = false;
      }

      if ((ecBDosageOffFlag) && (millis() >= ecBDosageOnTime + (ecdosetrig * 1000))) {

        relayInit.ecPump_B(0);
        ecBDosageFlag = 0;
        relayInit.Chiller(1);
        chillerStat = 1;
        ChillerOffTime = i;
        ChillerOffFlag = true;
        ecBDosageOffFlag = false;
      }
      if (ChillerOffFlag && (i >= ChillerOffTime + Constant.ECDosageTime)) {
        ECDosageTrig = true;
        relayInit.Chiller(0);
        //Serial1.println("EC b");
        chillerStat = 0;


        ecDosage = true;

        testFlag5 = true;
        pumpEOffTemp = i;
        pumpEOnTemp = i;
        OffFlag = false;
        OnFlag = true;
        relayInit.WaterPump(1);


        irrigationStatus = 1;
        intruptwaterfillflag = false;

        ChillerOffFlag = false;
        ecTemp1 = millis();
      }
      if ((millis() >= ecTemp1 + (doseCheckTime * 60000)) && (testFlag5 == true)) {


        ecFlag = true;
        testFlag5 = false;
        ovrflowflag = true;
      }
    }

    if ((wTemp >= Set_Temp) && (!waterTempError) && wLevel && (!irrigationStatus) && (growTemp <= 38)) {

      chillerStat = 1;
      relayInit.Chiller(1);
    }


    if (ECDosageTrig) {
      if (((wTemp <= Set_Temp - 1) || (!wLevel) || (irrigationStatus) || (growTemp > 38)) && ((!maintChillerFlag))) {

        chillerStat = 0;
        relayInit.Chiller(0);
      }
    }
  }
}




/**
   @brief Parsing the incoming Pi message with respect to comma

   @param data Incoming message
   @param separator ","
   @param index Position
   @return String Parsed string
*/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}




void useacknowledgementfunc() {
  if ((Setpoint_Ec < ec) || (ECPumpError) || (!ecLevel_1) || (!ecLevel_2)) {

    isEcDosing = false;
    Phcountstartflag = true;
    phDcycleFlag = true;
    dayDownFlag = true;
    phUPcycleFlag = true;
    dayUPFlag = true;
    PHdownflag3 = true;
    PHupflag3 = true;

  } else {
    isEcDosing = true;
  }
}

/**
 * @brief Error reset function
 * 
 */

void Errorresetfunc() {



  if (ECPumpError || pHIncPumpError || pHDecPumpError) {

    i = 0;
    solenoidOnTemp = 0;
    pumpEOnTemp = 0;
    pumpEOffTemp = 0;
    CurrentECDosageTime = 0;
    ChillerOffTime = 0;
    ECPumpError = 0;
    CurrentPHDosageTime = 0;
    CurrentPHUPDosageTime = 0;
    pHIncPumpError = 0;
    pHDecPumpError = 0;
    Downdosewaittime = 0;
    updosewaittime = 0;
  }



  FlowSensorError = 0;
  prevStage = 0;

  autosolenoiderror = 0;
  ultrasonicerror = 0;
  WaterPumpError = 0;
  envTempError = 0;
  waterChillerError = 0;
  errorData = 0;
  transpirationError = 0;
  waterTempError = 0;
  pHError = 0;
  ECError = 0;
}

/**
   @brief pH Down solution Triggering function

*/
void phDown() {


  relayInit.pHDown(1);

  testFlag = true;
  phTemp = millis();
}

/**
   @brief pH Up solution Triggering function

*/
void phUp() {
  //Serial1.println("up");

  relayInit.pHUp(1);

  testFlag2 = true;
  //phTemp2 = i;
  phTemp2 = millis();
}

/**
   @brief EC Supplement Triggering function

*/
void ecTrigger() {
  relayInit.WaterPump(0);
  irrigationStatus = 0;
  relayInit.ecPump_A(1);
  ecFlag = false;
  testFlag4 = true;
  //ecTemp = i;
  ecTemp = millis();
}




/**
   @brief System Timer for Water Pump and Error checking

*/
void action() {

  i += 5;

  if ((i >= pumpOnTime + pumpEOffTemp) && (OnFlag == true)) {
    
    waterChillerError = errorHandling.waterChillerCheck(wTemp, chillerStat, growTemp, Set_Temp);

    relayInit.WaterPump(0);
    irrigationStatus = 0;
    pumpEOnTemp = i;
    OnFlag = false;
    OffFlag = true;
    solenoidONflag = true;

    if (pHdownDosage) {
      tempPH = pH;
      sensorCheck();
    }
    if (pHupDosage) {
      tempPH = pH;
      sensorCheck();
    }
    if (ecDosage) {

      tempEC = ec;

      sensorCheck();
    }
    DropPHError = errorHandling.PHDropError(pH);
    if (DropPHError) {

      useacknowledgementfunc();
    }
  } else if ((i >= (pumpOffTime + pumpEOnTemp)) && (OffFlag == true) && (wLevel)) {
    waterChillerError = errorHandling.waterChillerCheck(wTemp, chillerStat, growTemp, Set_Temp);
    relayInit.WaterPump(1);
    irrigationStatus = 1;
    intruptwaterfillflag = false;


    pumpEOffTemp = i;
    OffFlag = false;
    OnFlag = true;
  }

  else if ((i > (pumpEOnTemp + Constant.solenoidOnTime)) && (OffFlag == true) && (!irrigationStatus)) {  //test
    if ((Wdistance >= 6.5) && (solenoidONflag) && (systemmodel == 0) && (!ultrasonicerror) && (!autosolenoiderror)) {
      relayInit.solenoidIN(1);
      watertopupstat = 1;

      solenoidOnTemp = i;
      solenoidOFFflag = true;
      solenoidONflag = false;
      autowatertopuperrorflag = true;
    }
    solenoidONflag = false;
    intruptwaterfillflag = true;  //this flag for manual system top up flag

    if (autowatertopuperrorflag) {
      autoprevWdistance = Wdistance;
      autowatertopupcheck = millis();
      autowatertopuperrorflag = false;
    }

    if ((Wdistance < autoprevWdistance - 0.1) && (solenoidOFFflag)) {
      autoprevWdistance = Wdistance;
      autowatertopupcheck = millis();
    }

    if (((Wdistance <= 3.8) || ultrasonicerror) && (solenoidOFFflag) && (systemmodel == 0)) {  //test to
      relayInit.solenoidIN(0);
      watertopupstat = 0;

      solenoidOFFflag = false;
    } else if ((i > solenoidOnTemp + Constant.solenoidOFFTime) && (solenoidOFFflag) && (systemmodel == 0)) {
      relayInit.solenoidIN(0);
      watertopupstat = 0;
      autosolenoiderror = 1;
      solenoidOFFflag = false;
    }

    if ((millis() > autowatertopupcheck + 180000) && (solenoidOFFflag) && (systemmodel == 0)) {

      autosolenoiderror = 1;
      solenoidOFFflag = false;
      relayInit.solenoidIN(0);
      watertopupstat = 0;
    }


  } else {
    if (((irrigationStatus) || (ultrasonicerror) || (Wdistance <= 3.8) || (!OffFlag)) && (systemmodel == 0)) {  //its for dose happen in solenoid ON time
      relayInit.solenoidIN(0);
      watertopupstat = 0;

      solenoidOFFflag = false;
    }
  }


  if (i >= 40) {
    FlowSensorError = errorHandling.waterPumpCheck(wFlow, OnFlag, wLevel);

    if (transpirationError || FlowSensorError || WaterPumpError || waterChillerError || waterTempError || envTempError || pHError || ECError || pHIncPumpError || pHDecPumpError || ECPumpError || ultrasonicerror || autosolenoiderror||(!pHUp)||(!pHDown)||(!ecLevel_1)||(!ecLevel_2)) {
      error = 1;
    } else {
      error = 0;
    }
  }
}


/**
   @brief Nutrients Motors error checking function

*/
void sensorCheck() {
  if (pHupDosage) {
    pHupDosage = false;
    Phdeccnt = 0;
    Eccnt = 0;

    if ((tempPH - pHTempData) < pumperror_ph) {

      if (pHUp) {
        if (!pHError) {
          Phinccnt++;
        }
        if (Phinccnt == 4) {

          pHIncPumpError = 1;
          Phinccnt = 0;
        }
      }
    } else {
      pHIncPumpError = 0;
      Phinccnt = 0;
    }
  }

  if (pHdownDosage) {
    pHdownDosage = false;
    Phinccnt = 0;
    Eccnt = 0;
    if ((pHTempData - tempPH) < pumperror_ph) {
      if (pHDown) {
        if (!pHError) {
          Phdeccnt++;
        }
        if (Phdeccnt == 4) {

          pHDecPumpError = 1;
          Phdeccnt = 0;
        }
      }
    } else {
      pHDecPumpError = 0;
      Phdeccnt = 0;
    }
  }

  if (ecDosage) {
    ecDosage = false;
    Phinccnt = 0;
    Phdeccnt = 0;

    if ((tempEC - EcTempData) < (pumperror_Ec)) {


      if ((ecLevel_1) && (ecLevel_2) && (!ECError)) {
        Eccnt++;
      }
      if (Eccnt == 4) {
        Eccnt = 0;
        ECPumpError = 1;
      }
    } else {
      ECPumpError = 0;
      Eccnt = 0;
    }
  }
}


/**
   @brief Water flow rate function, works on interrupt

   @return float flow rate (LPM)
*/
float waterFlowRate() {
  
  l_hour = (flow_frequency / 7.5);

  waterflowvalue = (l_hour * 12 / Calibration) / 10;
  //Serial2.println("fflow_frequency " + String(flow_frequency) + " waterflowvalue = " + String(waterflowvalue));

  flow_frequency = 0;
  // }

  return waterflowvalue;
}


/**
   @brief Messages Payload from Controller to Pi

*/
void MessagePayload() {
  Serial1.print((String) "" + wTemp + "," + wLevel + "," + wFlow + "," + growTemp + "," + growHum + "," + pH + "," + ec + "," + pHUp + "," + pHDown + "," + ecLevel_1 + "," + ecLevel_2 + "," + dutyCycle + "," + lightStat + "," + irrigationStatus + "," + chillerStat + "," + ecADosageFlag + "," + ecBDosageFlag + "," + phupDosageFlag + "," + phdownDosageFlag + "," + error + "," + ecdosetrig + "," + PHuptrig + "," + PHdowntrig + "," + wltrs + "," + watertopupstat + "," + waterflushstat + "," + CRCcheck + "," + msgType + "\n");
}

/**
   @brief Settings Payload from Controller to Pi

*/
void SettingPayload() {
  Serial1.print((String) "" + (pumpOnTime / 60) + "," + (pumpOffTime / 60) + "," + thresholdMinpH + "," + thresholdMaxpH + "," + check_errorPh + "," + pumperror_ph + "," + Setpoint_Ec + "," + Ec_Delta + "," + check_errorEc + "," + pumperror_Ec + "," + Set_Temp + "," + phconcentrationvalue + "," + Ecconcentrationvalue + "," + (doseCheckTime) + "," + systemmodel + "," + savertime + "," + saverstatus + "," + CRCcheck + "," + msgType + "\n");
}

void callibrationpayload() {
  Serial1.print((String) "" + Result.calibrationflag + "," + CRCcheck + "," + msgType + "\n");
}


void flushpayload() {
  Serial1.print((String) "" + Result.flushresult + "," + CRCcheck + "," + msgType + "\n");
}



void waterfillpayload() {
  Serial1.print((String) "" + Result.waterfillresult + "," + CRCcheck + "," + msgType + "\n");
}




/**
   @brief Error Payload from Controller to Pi

*/
void ErrorPayload() {
  Serial1.print((String) "" + WaterPumpError + "," + FlowSensorError + "," + waterChillerError + "," + waterTempError + "," + envTempError + "," + pHError + "," + ECError + "," + ECPumpError + "," + pHIncPumpError + "," + pHDecPumpError + "," + transpirationError + "," + ultrasonicerror + "," + autosolenoiderror + "," + watchdogreset + "," + BORreset + "," + CRCcheck + "," + msgType + "\n");
}

/**
   @brief Returns firmware version

*/
void versionPayload() {
  Serial1.print((String) "" + Version.versionId + "," + Version.versionSubId + "," + Version.board_ver+","+Version.board_subver+","+ CRCcheck + "," + msgType + "\n");
}

void DebugPHPayload() {
  Serial1.print(debugPHString + "," + String(confidence) + "," + String(CRCcheck) + "," + String(msgType) + "\n");
}


void DebugECPayload() {
  Serial1.print(debugECString + "," +  String(ECconfidence) + "," + String(CRCcheck) + "," + String(msgType) + "\n");
}


/**
   @brief CRC calculation for Version payload

   @return float CRC
*/
float CrcVersion() {
  return (Version.versionId + Version.versionSubId +Version.board_ver+Version.board_subver +Constant.key);
}

/**
   @brief CRC calculation for Message Payload

   @return float CRC
*/
float CrcMessage() {
  return (wTemp + wLevel + wFlow + growTemp + growHum + pH + ec + pHUp + pHDown + ecLevel_1 + ecLevel_2 + dutyCycle + lightStat + irrigationStatus + chillerStat + ecADosageFlag + ecBDosageFlag + phupDosageFlag + phdownDosageFlag + error + ecdosetrig + PHuptrig + PHdowntrig + wltrs + watertopupstat + waterflushstat + Constant.key);
}

/**
   @brief CRC calculation for Settings Payload

   @return float CRC
*/
float CrcSetting() {
  return ((pumpOnTime / 60) + (pumpOffTime / 60) + thresholdMinpH + thresholdMaxpH + check_errorPh + pumperror_ph + Setpoint_Ec + Ec_Delta + check_errorEc + pumperror_Ec + Set_Temp + phconcentrationvalue + Ecconcentrationvalue + (doseCheckTime) + systemmodel + savertime + saverstatus + Constant.key);
}

float Crccallibration() {
  return (Result.calibrationflag + Constant.key);
}

float Crcflushresult() {
  return (Result.flushresult + Constant.key);
}

float Crcwaterfillresult() {
  return (Result.waterfillresult + Constant.key);
}

/**
  @brief CRC calculation for ERROR Payload

  @return float CRC
*/
float CrcError() {
  return (WaterPumpError + FlowSensorError + waterChillerError + waterTempError + envTempError + pHError + ECError + ECPumpError + pHIncPumpError + pHDecPumpError + transpirationError + ultrasonicerror + autosolenoiderror + watchdogreset + BORreset + Constant.key);
}



float CrcPHDebug() {
  return (Total_PH + confidence + Constant.key);
}

float CrcECDebug() {
  return (Total_EC + ECconfidence+Constant.key);
}