/* Library to check the health of a nine volt battery */

// Include neccessary libraries
#include "Arduino.h"
#include "Batt_health.h"

// Function used to initially set up the battery reading program
batt::batt(int battHealthPin)
{
  pinMode(battHealthPin, INPUT);
  
  _battHealthPin=battHealthPin;
  
}

// Function that outputs the battery health as a percent
float batt::percent() {

  // Return the analog voltage and convert to percent
  return analogRead(_battHealthPin) * (100. / 9.) * (5. / 1023.) * (11);
  // 5/1023 due to arduino analog scaling
  //11 due to reciprocal of 1/11 from Vout = R2/(R1+R2)*Vin
  //100%9 to calculate percentage based on 9V initial health

}