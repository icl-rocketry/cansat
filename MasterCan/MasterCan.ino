/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "VibMotor.h"
#include "Bell.h"
#include "BNO055.h"
#include "Batt_health.h"
#include "BMP388.h"
#include "dataSend.h"

// Set constants
const int battHealthPin = 19;
const float seaPressure = 101325;
const int vibpin = 2;
const int buzzerpin = 8;
const int minBattVolt = 3;
const int greenLEDPin = 5;
const int redLEDPin = 6;
const int sendRate = 500;

// Vertical speed below which CanSat will assume it is stationary, and descending.
const float velStopThresh = 2;
const float velDropThresh = -2;

// Respective times that CanSat has to stay in its state for to count as stationary and descending
const float velStopTimeThresh = 3000;
const float velDropTimeThresh = 3000;

Vib vib(vibpin);
Bell bell(buzzerpin, greenLEDPin, redLEDPin);
BNO055 accel;
batt batt(battHealthPin);
BMP388 alt(seaPressure);
logger logger("results.txt", ':');

unsigned long nowtime = millis();
unsigned long packetCount = 1;
byte softState = 0;

/* softState variable is a single byte where the error codes are powers of two that are added.
  The individual errors can be found by getting the softState and dividing it into its individual powers of two
  The powers of 2 and their corresponding errors are as follows:

  1	-	ERROR: SD card initialisation failed
  2	-	ERROR: BNO055 initialisation failed
  4	-	ERROR: Failed to write to SD Card
  8	-	WARNING: Battery voltage low
*/

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);

  // Initialise bell library
  bell.start();

  // Initialise SD card
  if (!logger.SDstart(10)) {
    softState = 1;
    Serial.println(softState);
    bell.fatalError();
  }

  // Initialise accelerometer
  if (!accel.start())
  {
    softState = softState + 2;
    Serial.println(softState);
    bell.fatalError();
  }

  // Initialise pressure sensor
  alt.start();

  // Start vibration motor
  vib.vibstart();


  // Open SD card file
  if (!logger.open()) {
    softState = softState + 4;
    Serial.println(softState);
    bell.fatalError();
  }

  // Beep the buzzer to show succesful run of startup sequence
  bell.buzzerStart();
  delay(300);
  bell.buzzerStop();
}


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  unsigned long prevTime = nowtime;
  static float altitude = 0;
  static unsigned long prevSendTime = 0;

  // Get accelerometer data
  imu::Vector<3> accelData = accel.getAccel();

  // Set nowtime to the time in ms since the arduino turned on
  nowtime = millis();

  if (nowtime > prevSendTime + sendRate) {

    // Reset the send time
    prevSendTime=nowtime;
    
    // Get the altimeter data
    float prevAltitude = altitude;
    altitude = alt.Alt();
    float temperature = alt.Temp();
    float pressure = alt.Pres();

    // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
    float velocity = 1000 * (altitude - prevAltitude) / (nowtime - prevTime );

    static bool fell = false;
    static unsigned long prevDropTime = nowtime;

    // If the velocity is decreasing past a certain rate
    if (velocity < velDropThresh) {

      // If this descent has continued for a while, set the flag fell to true
      if (prevDropTime + velDropTimeThresh < nowtime) {

        fell = true;

      }

      // Otherwise reset the previous drop time
    }
    else {

      prevDropTime = nowtime;

    }

    static unsigned long prevStopTime = nowtime;

    // If the velocity is small and the CanSat has fallen
    if ((abs(velocity) < velStopThresh) && fell) {

      // If the CanSat hasn't been moving for a while, start the buzzer
      if (nowtime > prevStopTime + velStopTimeThresh) {

        bell.buzzerStart();

      }

      // Otherwise reset the previous stop time
    }
    else {

      prevStopTime = nowtime;

    }

    // Read battery percentage
    float battVolt = batt.voltage();

    // Check if the battery voltage is safe
    if (battVolt < minBattVolt) {
      softState = softState + 8;
      bell.error();
    }

    // Get orientation data
    imu::Vector<3> orientData = accel.getOrient();

    logger.send(packetCount);
    logger.send(nowtime);
    logger.send(pressure);
    logger.send(temperature);
    logger.send(altitude);
    logger.send(velocity);
    logger.send(battVolt);
    logger.send(softState);
    logger.send(accelData.x());
    logger.send(accelData.y());
    logger.send(accelData.z());
    logger.send(orientData.x());
    logger.send(orientData.y());
    logger.send(orientData.z());
    logger.Flush();

    // Reset softState
    softState = 0;

  } 
  else {

    logger.SDwrite(packetCount);
    logger.SDwrite(nowtime);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(accelData.x());
    logger.SDwrite(accelData.y());
    logger.SDwrite(accelData.z());
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDwrite(0);
    logger.SDflush();

  }

  packetCount += 1;

}
