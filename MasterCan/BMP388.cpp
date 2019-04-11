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
// seaPressure=Accurate sea level pressure to calibrate bmp388 -> NEED TO FILL IN

// Create a new instance of the library
BMP388::BMP388(float seaPressure) {

  _seaPressure=seaPressure;
  
}

// Function used to initialise the sensor, returns true if initialisation was succesful
bool BMP388::start() {

   return bmp3xx.begin();

}

float BMP388::Alt() {           //Read altitude (m)
  return bmp3xx.readCalibratedAltitude(_seaPressure);
}

float BMP388::Pres() {          //Read pressure (Pa)
  return bmp3xx.readPressure();
}

float BMP388::Temp() {          //Read Temperature (Celsius)
  return bmp3xx.readTemperature();
}
