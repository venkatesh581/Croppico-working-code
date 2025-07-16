
#include "Arduino.h"

class Dosetime {
    public:

    int waterltrs(float wdistance);
    int ecDoseTime(int wltrs,float ec,float Setpoint_Ec,float Ec_Delta);
    int  phUpDoseTime(int wltrs,float pH,float thresholdMinpH,float thresholdMaxpH);
    int phDownDoseTime(int wltrs,float pH,float thresholdMinpH,float thresholdMaxpH);


};