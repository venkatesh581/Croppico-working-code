#include "Arduino.h"
#include "AnalogSensor.h"


extern float Kvaluelow, ecTrig;
extern int Neutralvoltage, Acidvoltage;
extern byte wLevel;
const int sampleSize = 20;
#define pHPin PA6
#define EC_PIN PA7
#define HTUd_Pin PC_5
#define Atemp_Pin PA5

int analogpH[sampleSize], analogEC[sampleSize],analoghtd[10],analogtemp[10];
int PHrawValue, adc_ec,humidityadc,tempadc;
uint32_t Total_PH,Total_EC;
String debugPHString = "",debugECString="";
bool PHdebugflag = false,ECdebugflag=false;
float  ECvalueRaw;
float ecValue,voltage_ec;


void sortSamples(int arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}


void collectECSamples() {
  for (int i = 0; i < sampleSize; i++) {
    analogEC[i] = analogRead(EC_PIN);
    delay(10);
   Serial3.println((String)"ADC_EC =" + analogEC[i]);

    

  }
  
  Total_EC = 0;
  for (int j = 0; j < 20; j++) {

    Total_EC += analogEC[j];
  }

  debugECString = "";

  for (int j = 0; j < 20; j++) {

    debugECString += String(analogEC[j]);

    if (j < 19) {
      debugECString += ",";
    }
  }
  ECdebugflag = true;
  sortSamples(analogEC, sampleSize);
}


/**
   @brief pH calculation function

   @param analogpH ADC data from sensor
   @return float pH
*/


float convertPHfromAnalog(int adcValue) {

  float slope = (7.0 - 4.0) / ((Neutralvoltage - 1500.0) / 3.0 - (Acidvoltage - 1500.0) / 3.0);
  float intercept = 7.0 - slope * (Neutralvoltage - 1500.0) / 3.0;
  

  // Convert ADC value to voltage
  float voltage = adcValue / 4096.0 * 3300;

  // Convert voltage to pH value
  float pHValue = slope * (voltage - 1500.0) / 3.0 + intercept;

  return pHValue;
}



void collectSamples() {


  for (int i = 0; i < sampleSize; i++) {
    analogpH[i] = analogRead(pHPin);
    delay(10);
   Serial3.println((String)"ADCPH =" + analogpH[i]);
  }
  Total_PH = 0;

  for (int j = 0; j < 20; j++) {

    Total_PH += analogpH[j];
  }


  debugPHString = "";
  for (int j = 0; j < 20; j++) {

    debugPHString += String(analogpH[j]);

    if (j < 19) {
      debugPHString += ",";
    }
  }
  PHdebugflag = true;
  sortSamples(analogpH, sampleSize);
  // printSamples(analogpH, sampleSize);
}





float calculateConfidence(int arr[], int size, float referenceValue, int threshold) {
  int inRange = 0;
  for (int i = 0; i < size; i++) {
    if (arr[i] >= (referenceValue - threshold) && arr[i] <= (referenceValue + threshold)) {
      inRange++;
    }
  }


  return (float)inRange / size * 100;
}

float getMedian(int arr[], int size) {
  return arr[size / 2];
}

// Function to read pH value, calculate confidence, and handle errors
float AnalogSensor::getPHAnalogInput(float &Phvalue, byte &confidence,  byte &threshold) {

  collectSamples();
  PHrawValue = getMedian(analogpH, sampleSize);
  confidence = calculateConfidence(analogpH, sampleSize, PHrawValue, threshold);
   Serial3.println((String)"PHconfidence =" + confidence);

 


  Phvalue = convertPHfromAnalog(PHrawValue);
 
  
  return Phvalue;
}


/**
   @brief EC calculation function

   @param analogEC ADC data from sensor
   @param wtemp Water temperature data for accuracy
   @return int EC
*/
int AnalogSensor::getEc(float &wTemp, byte &ecconfidence, byte &threshold) {

  collectECSamples();
  adc_ec = getMedian(analogEC, sampleSize);
  ecconfidence = calculateConfidence(analogEC, sampleSize, adc_ec, threshold);
  Serial3.println((String)"ecconfidence =" + ecconfidence);
 voltage_ec = adc_ec / 4096.0 * 3300;

  ECvalueRaw = 1000 * voltage_ec / RES2 / ECREF * Kvaluelow * 10.0;
  ecValue = ECvalueRaw / (1.0 + 0.0185 * (wTemp - 25.0));

  return (ecValue * 1000);
}

/**
   @brief Grow area Temperature function

   @return float Temperature (*C)
*/



float AnalogSensor::GrowEnvironmentHum(float &Humvalue,byte &htdconfidence)
{
  for (int i = 0; i < 10; i++) {
    analoghtd[i] = analogRead(HTUd_Pin);
    delay(10);
    Serial3.println((String)"ADC_hum =" + analoghtd[i]);
  }
  sortSamples(analoghtd, 10);

   humidityadc= getMedian(analoghtd,10);

  htdconfidence = calculateConfidence(analoghtd, 10, humidityadc, 100);
  
  float voltage_humidity = humidityadc / 4096.0 * 3.30;

  Humvalue = -12.5 + 125 * (voltage_humidity / 3.30);
  
  return Humvalue;
  

}

/**
   @brief Grow area Humidity function

   @return float Humidity (%)
*/

float AnalogSensor::GrowEnvironmentTemp(float &growTempvalue,byte &tempconfidence)
{


  for (int i = 0; i < 10; i++) {
    analogtemp[i] = analogRead(Atemp_Pin);
    delay(10);
       Serial3.println((String)"ADC_TEMP =" + analogtemp[i]);

  }
  sortSamples(analogtemp, 10);

   tempadc= getMedian(analogtemp,10);

  tempconfidence = calculateConfidence(analogtemp, 10, tempadc, 100);
  
  float voltage_temp = tempadc / 4096.0 * 3.30;

   growTempvalue = -60.625 + 206.25 * (voltage_temp / 3.3);
  
  return growTempvalue ;
}






