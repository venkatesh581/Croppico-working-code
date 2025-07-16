#ifndef PWM_h
#define PWM_h

#include "Arduino.h"


class PWM {
    public:
        void TrigPwm(int frequency, int dutyCycle);
};

#endif
