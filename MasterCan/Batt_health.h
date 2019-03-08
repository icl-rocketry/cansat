/* Library to check the health of a nine volt battery */

#ifndef batt_h
#define batt_h

// Include neccessary libraries
#include "Arduino.h"

class batt {

  private:

    int _battHealthPin;
    
  public:
  
    batt(int battHealthPin);
    float percent();
};

#endif
