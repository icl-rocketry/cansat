/* Interface library for the BNO055 accelerometer
  Simply a conversion of the sensor_api example into a library
*/


#ifndef BNO055_h
#define BNO055_h

// Required libraries
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO055
{
  private:

    // Undocumented, but variable is possibly used to set the i2c address
    Adafruit_BNO055 bno;

  public:

    BNO055();
    bool start();
    imu::Vector<3> getData();
    uint8_t calibration();

    
};

#endif
