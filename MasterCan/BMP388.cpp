/*Interface library for the BMP388 altitude, pressure and temperature sensor*/

// Include neccessary libraries
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include "BMP388.h"
#include "DFRobot_BMP388.h"
#include "DFRobot_BMP388_I2C.h"
#include "bmp3_defs.h"

DFRobot_BMP388_I2C bmp3xx;      //Create a bmp388 object to communicate with IIC.
const float calibAlt = 38.0;        //Accurate altitude (m) to calibrate bmp388 -> NEED TO FILL IN

// Function used to initialise the sensor, returns true if initialisation was succesful
BMP388::BMP388(float calibAlt) {

  _calibAlt=calibAlt;
  
  //Initialise sensor
  bmp3xx.begin();

}

// Function that returns the sea level air pressure
float BMP388::dispSeaLevel(int mode) {      //Use calibrated altitude to calibrate sea level air pressure, use this as reference to obtain calibrated altitude
  float seaLevel = bmp3xx.readSeaLevel(_calibAlt);
  if (mode == 0) {                //Mode determines whether to return sealevel air pressure or sea level altitude
    return seaLevel;            //Read sea level air pressure (Pa)
  } else {
    return bmp3xx.readCalibratedAltitude(seaLevel); //Read sea level altitude (m)
  }
}

float BMP388::Alt() {           //Read altitude (m)
  return bmp3xx.readAltitude();
}

float BMP388::Pres() {          //Read pressure (Pa)
  return bmp3xx.readPressure();
}

float BMP388::Temp() {          //Read Temperature (Celsius)
  return bmp3xx.readTemperature();
}
