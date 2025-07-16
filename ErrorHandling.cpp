#include "Arduino.h"
#include "ErrorHandling.h"

extern volatile int irrigationStatus;




bool checkWTemp = true, InitFlag = true, checkEC = true, Ecdropflag = false,PHdropflag = false,checkPH =true ,chillerbufflag=false;

unsigned long long PumpCheckTime = 0, SampleWTime = 0;
float waterTempBuf, wTempBuf,ECValueBuf,PHValueBuf;
int errorData = 0, prevStage = 0, TempError = 0,ECDError=0,PHDError;

/**
   @brief Water pump check function

   @param waterFlow water flow rate
   @param flag water pump current status
   @param maintFlag maintenance mode water pump on status
   @return int error status
*/
int ErrorHandling ::waterPumpCheck(float waterFlow, bool flag, int wLevel) {
  if (wLevel) {

    if ((flag) && InitFlag) {

      PumpCheckTime = millis();
      InitFlag = false;
      return prevStage;
    }
    if ((millis() > PumpCheckTime + 15000) && (!InitFlag)) {

      if (flag) {

        InitFlag = true;

        if (waterFlow > 0.6) {

          prevStage = 0;
          return prevStage;
        }

        else if ((waterFlow <= 0.6) && (waterFlow > 0.3))

        {

          prevStage = 1;
          return prevStage;
        }

        else if (waterFlow <= 0.3) {

          prevStage = 2;
          return prevStage;
        }
      }

      else {

        InitFlag = true;
        return prevStage;
      }
    } else {
      return prevStage;
    }
  } else {

    return 0;
  }
}



/**
   @brief Water chiller check function

   @param waterTemp Water temperature
   @param chillerState Chiller current status
   @return int error status
*/
int ErrorHandling ::waterChillerCheck(float waterTemp, bool chillerState, float growTemp,int Set_Temp)

{
  if ((irrigationStatus)&&(waterTemp>=Set_Temp)){
    waterTempBuf = waterTemp;
    chillerbufflag=true;
    return errorData;
  }
  


  if (((chillerState)||(!chillerState))&&(chillerbufflag)&&(!irrigationStatus)) {
       chillerbufflag=false;

    if ((waterTemp <= waterTempBuf - 1)||(growTemp>=38)||(waterTemp<= Set_Temp)) {
   
      errorData = 0;
      return errorData;

    }
    else{
 
    
     errorData = 1;
      return errorData;


    }

  } else {
   
      chillerbufflag=false;
      errorData = 0;
    return errorData;
  }


  
}

/**
   @brief Water temperature sensor check function

   @param waterTemp Water temperature data
   @return int error status
*/
int ErrorHandling ::waterTempCheck(float waterTemp) {

  if ((waterTemp <= 0) || (waterTemp >= 100)) {
    return 1;
  }

  else {
    return 0;
  }
}

/**
   @brief pH sensor check function

   @param pHdata Current pH data
   @return int error status
*/
int ErrorHandling ::pHErrorCheck(float pHdata,int pHTError,float check_errorPh) {
  if ((pHdata < check_errorPh)||(pHTError>=20))
 {

    return 1;
  } else {
    
    return 0;
  }
}

/**
   @brief EC sensor check function

   @param ecData Current EC data
   @param waterLevel water level status
   @return int error status
*/
int ErrorHandling ::ECErrorCheck(int ecData, int waterLevel,int ECTError,int check_errorEc) {

  if ((ecData < check_errorEc)||(ECTError>=20)) {
    if (waterLevel) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
/**
 * @brief Transpiration ERROR
 * 
 * @param ec Current EC data
 * @param Setpoint_Ec EC thershold value
 * @param wLevel water level status
 * @return int  error status
 */
int ErrorHandling ::TranspirationCheck(float ec, float Setpoint_Ec, int wLevel) {
  if ((ec > (Setpoint_Ec + 300)) && ((wLevel == 1)||(wLevel ==0)))  ///.....
  {
    return 1;

  } else if ((ec < (Setpoint_Ec + 300)) || (wLevel == 2))  //....
  {
    return 0;
  }
  else{
    return 0;

  }
}



int ErrorHandling ::PHDropError(float pH) {

  if ((pH <5.0) || (pH>7.0))
  {
     //Serial1.print("ON");
    return 1;
  }
  else{
    // Serial1.print("OFF");
    return 0;
  }

}




int ErrorHandling ::humitidyErrorCheck(float growHum, float growTemp,int HumErrorcnt,int temperrorcnt) {

if(growHum<=0 || growHum >= 100 || growTemp<=0 || growTemp >= 100||HumErrorcnt>=50||temperrorcnt>=50)
{

  return 1;
}
else{
  return 0;
}

}