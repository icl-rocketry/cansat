/*Interface for bmp388 altitude, pressure and temperature sensor*/
#include "arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include "DFRobot_BMP388.h"
#include "DFRobot_BMP388_I2C.h"
#include "bmp3_defs.h"

class BMP388 {
    private:
        DFRobot_BMP388_I2C bmp3xx;      //Create a bmp388 object to communicate with IIC.
        const float calibAlt = 38.0;        //Accurate altitude (m) to calibrate bmp388 -> NEED TO FILL IN
        float seaLevel;                 //Calibrated sea level altitude (m) for bmp388  
    public:
        bool Setup() {
            //Initialise sensor
            if bmp3xx.begin()
                return false;       //By default it returns an error
            else
                return true;
        }
        float dispSeaLevel(int mode) {      //Use calibrated altitude to calibrate sea level air pressure, use this as reference to obtain calibrated altitude
            seaLevel = bmp3xx.readSeaLevel(calibAlt);
            if (mode == 0) {                //Mode determines whether to return sealevel air pressure or sea level altitude
                return seaLevel;            //Read sea level air pressure (Pa)
            else
                return bmp3xx.readCalibratedAltitude(seaLevel); //Read sea level altitude (m)
            }
        }
        float Alt() {           //Read altitude (m)
            return bmp3xx.readAltitude();
        }
        float Pres() {          //Read pressure (Pa)
            return bmp3xx.readPressure();
        }
        float Temp() {          //Read Temperature (Celsius)
            return bmp3xx.readTemperature();
        }
};