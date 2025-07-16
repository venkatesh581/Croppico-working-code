#include "Arduino.h"
#include "DoseTime.h"
float ECdoseSec = 0, PhDowndose, Phupdose;
extern float Ecconcentrationvalue, phconcentrationvalue;



int Dosetime::waterltrs(float wdistance) {


  const float tolerance = 0.01; // Small tolerance to handle floating point precision issues

  if (wdistance == 0) {
    return 1;
  } 
  else if (wdistance > 1 && wdistance <= 4.3 + tolerance) {
    return 40;
  } else if (wdistance >= 4.4 - tolerance && wdistance <= 5.7 + tolerance) {
    return 38;
  } else if (wdistance >= 5.8 - tolerance && wdistance <= 6.4 + tolerance) {
    return 36;
  } else if (wdistance >= 6.5 - tolerance && wdistance <= 6.8 + tolerance) {
    return 34;
  } else if (wdistance >= 6.9 - tolerance && wdistance <= 7.6 + tolerance) {
    return 32;
  } else if (wdistance >= 7.7 - tolerance && wdistance <= 8.5 + tolerance) {
    return 30;
  } else if (wdistance >= 8.6 - tolerance && wdistance <= 10.4 + tolerance) {
    return 28;
  } else if (wdistance >= 10.5 - tolerance && wdistance <= 10.8 + tolerance) {
    return 26;
  }else if (wdistance >= 10.9 - tolerance && wdistance <= 11.2 + tolerance) {
    return 24;
  }else if (wdistance >= 11.3 - tolerance && wdistance <= 11.7 + tolerance) {
    return 22;
  } else if (wdistance >= 11.8 - tolerance && wdistance <= 12.2 + tolerance) {
    return 20;
  } else if (wdistance >= 12.3 - tolerance && wdistance <= 12.9 + tolerance) {
    return 18;
  } else if (wdistance >= 13 - tolerance && wdistance <= 14.4 + tolerance) {
    return 16;
  } else if (wdistance >= 14.5 - tolerance && wdistance <= 15.3 + tolerance) {
    return 14;
  } else if (wdistance >= 15.4 - tolerance && wdistance <= 16.4 + tolerance) {
    return 12;
  } else if (wdistance >= 16.5 - tolerance && wdistance <= 16.9 + tolerance) {
    return 10;
  } else if (wdistance >= 17 - tolerance && wdistance <= 17.6 + tolerance) {
    return 8;
  } else if (wdistance >= 17.7 - tolerance && wdistance <= 18.8 + tolerance) {
    return 6;
  } else if (wdistance >= 18.9 - tolerance && wdistance <= 19.9 + tolerance) {
    return 4;
  } else if (wdistance >= 20 - tolerance && wdistance <= 21.4 + tolerance) {
    return 2;
   }
   else if  (wdistance > 21.4) {
    return 0;
   }
   // else {
  //   // This should not be reached, indicating an error
  //   //Serial1.println("Error: wdistance does not fall within any expected range.");
  //   return 66;
  // }
}

int Dosetime::ecDoseTime(int wltrs, float ec, float Setpoint_Ec, float Ec_Delta) {
 

  float waterper = (1.0 / 40) * wltrs;
  ECdoseSec = (waterper * 40) * Ecconcentrationvalue;
  ECdoseSec = round(ECdoseSec);

  if (ECdoseSec <= 20) {
    ECdoseSec = 20;
  }

  return ECdoseSec;
  
}

int Dosetime::phUpDoseTime(int wltrs, float pH, float thresholdMinpH, float thresholdMaxpH) {
  
    float waterper = (1.0 / 40) * wltrs;
    Phupdose = (waterper * 10) * phconcentrationvalue;
    Phupdose = round(Phupdose);
    if (Phupdose <= 5) {
      Phupdose = 5;
    }

    return Phupdose;
 

 
}


int Dosetime::phDownDoseTime(int wltrs, float pH, float thresholdMinpH, float thresholdMaxpH) {

  
    float waterper = (1.0 / 40) * wltrs;

    PhDowndose = (waterper * 10) * phconcentrationvalue;


    PhDowndose = round(PhDowndose);
    if (PhDowndose <= 5) {
      PhDowndose = 5;
    }



    return PhDowndose;
  
  
}