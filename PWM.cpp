
#include "Arduino.h"
#include "PWM.h"
 
/* @brief PWM wave generation function
 * 
 * @param frequency Cycle repeatation
 * @param dutyCycle Brightness level
 */
void PWM::TrigPwm(int frequency, int dutyCycle)
{
  if(dutyCycle<=0){
      digitalWrite(PC5,LOW);
  }else{
    digitalWrite(PC5,HIGH);

  }
// float pulse = dutyCycle*2.56;
// //float pulse = dutyCycle*2.30;
// analogWrite(PC5, pulse);
}

