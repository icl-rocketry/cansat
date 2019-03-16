/* Interface library for the SD Card */

#ifndef SDCard_h
#define SDCard_h

// Include neccessary libraries
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include <utility/imumaths.h>

class SDCard
{
  private:
  
    File myFile;

  public:

    SDCard();
    bool start(int SDPin);
    bool fileCheck(char fileName);
    bool Write(String data);
};

#endif
