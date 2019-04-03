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

// Function that outputs the battery health in volts
float batt::voltage() {

  // Return the analog voltage and convert to percent
  return analogRead(_battHealthPin) * (5. / 1023.) * (147/47);
  // 5/1023 due to arduino analog scaling
  //11 due to reciprocal of 1/11 from Vout = R2/(R1+R2)*Vin

}
