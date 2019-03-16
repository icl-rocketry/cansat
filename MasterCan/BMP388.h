/*Interface library for the BMP388 altitude, pressure and temperature sensor*/

#ifndef BMP388_h
#define BMP388_h

// Include neccessary libraries
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include "DFRobot_BMP388.h"
#include "DFRobot_BMP388_I2C.h"
#include "bmp3_defs.h"

class BMP388
{
  private:

    DFRobot_BMP388_I2C bmp3xx;      //Create a bmp388 object to communicate with IIC.
    float _calibAlt;        //Accurate altitude (m) to calibrate bmp388 -> NEED TO FILL IN

  public:

    // Function used to initialise the sensor, returns true if initialisation was succesful
    BMP388(float calibAlt);
    bool start();
    float dispSeaLevel(int mode);
    float Alt();
    float Pres();
    float Temp();

};

#endif
