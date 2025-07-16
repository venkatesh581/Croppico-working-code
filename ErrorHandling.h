#ifndef ErrorHandling_h
#define ErrorHandling_h

#include "Arduino.h"
//#include <stm32g0xx_hal.h>  // Adjust the include according to your STM32 series


class ErrorHandling {
    public:
        int waterPumpCheck(float waterFlow,bool flag,int wLevel);
        int waterChillerCheck(float waterTemp, bool chillerState,float growTemp,int Set_Temp);
        int waterTempCheck(float waterTemp);
        int pHErrorCheck(float pHData,int pHTError,float check_errorPh);
        int ECErrorCheck(int ecData, int waterLevel,int ECTError,int check_errorEc);
        int TranspirationCheck(float ec, float Setpoint_Ec,int wLevel);
        int humitidyErrorCheck(float growHum, float growTemp,int htdconfidence,int tempconfidence);
        //int ECDropError(float ec);
        int PHDropError(float pH);


};
#endif
