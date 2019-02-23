/* Interface library for the BNO055 accelerometer
Simply a conversion of the sensor_api example into a library
*/

// Required libraries
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO055
{
  public:

    // Used to initially set up the accelerometer, returns true if succesful and false if not
    bool accelSetup() {
      
      // Undocumented, but I think is used to set the i2c address
      Adafruit_BNO055 bno = Adafruit_BNO055(55);
      
      // Initialise the sensor
      if (bno.begin())
      {
        // return true if successful
        return true;
      }
      else {
        // Otherwise return false
        return false;
      }

      // This step isn't documented but all examples seem to have it
      bno.setExtCrystalUse(true);
    }

    // Get data from accelerometer, given as array of form {x,y,z}
    float getData() {

      // Get data from the sensor
      sensors_event_t event;
      bno.getEvent(&event);

      // Save the new data to an array
      float accelData[] = {event.orientation.x, event.orientation.y, event.orientation.z};

      // Return this new data
      return accelData;

    }

    // Return calibration status of the accelerometer
    uint8_t calibration() {

      // Set up the variables
      uint8_t system, gyro, accel, mag = 0;

      // Poll the accelerometer for the values
      bno.getCalibration(&system, &gyro, &accel, &mag);

      // Save those values to an array
      uint8_t calStatus[] = {system, gyro, accel, mag};

      // Return the array
      return calStatus;
    }
}
