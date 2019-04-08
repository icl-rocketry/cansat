/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "VibMotor.h"
#include "Bell.h"
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"
#include "BMP388.h"
#include "dataSend.h"

// Set the pins used
const int battHealthPin = 2;
const int calibAlt=39;
const int vibpin = 6;
const int buzzerpin=8;
const int minBattVolt=9;
const int greenLEDPin=5;
const int redLEDPin=6;

Vib vib(vibpin);
Bell bell(buzzerpin, greenLEDPin, redLEDPin);
BNO055 accel;
SDCard SDC;
batt batt(battHealthPin);
BMP388 alt(calibAlt);
logger logger("results.txt",':');

// Vertical speed below which CanSat will assume it is stationary, and descending.
const float velStopThresh=2;
const float velDropThresh=3;

// Respective times that CanSat has to stay in its state for to count as stationary and descending
const float velStopTimeThresh=3;
const float velDropTimeThresh=3;

unsigned long nowtime = millis();
unsigned long packetCount=1;
byte softState=0;

/* softState variable is a single byte where the error codes are powers of two that are added.
The individual errors can be found by getting the softState and dividing it into its individual powers of two
The powers of 2 and their corresponding errors are as follows:

1	-	WARNING: Results file not detected, created
2	-	ERROR: SD card initialisation failed
4	-	ERROR: BNO055 initialisation failed
8	-	ERROR: Failed to write to SD Card
16	-	ERROR: Battery voltage low
*/

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);

  // Initialise bell library
  bell.start();

  // Initialise SD card
  if (!SDC.start(10)) {
        softState=2;
        Serial.println(softState);
	bell.fatalError();
  }

  // Check if the output file exists on the SD card, and create it if it doesn't
  if (!SDC.fileCheck("results.txt")) {
	softState=softState+1;
  }

  // Initialise accelerometer
  if (!accel.start())
  {
	softState=softState+4;
	Serial.println(softState);
 	bell.fatalError();
  }

  // Initialise pressure sensor
  alt.start();
  
  // Start vibration motor
  vib.vibstart();
}


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  unsigned long prevTime = nowtime;
  static float altitude = 0;
  float prevAltitude = altitude;

  // Set nowtime to the time in ms since the arduino turned on
  nowtime = millis();

  // Get the altimeter data
  altitude = alt.Alt();
  float temperature = alt.Temp();
  float pressure = alt.Pres();

  // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
  float velocity = (altitude - prevAltitude) / ((nowtime - prevTime )/1000);

  static bool fell=false;
  static unsigned long prevDropTime=nowtime;

  // If the velocity is decreasing past a certain rate
  if (velocity<velDropThresh) {
    
    // If this descent has continued for a while, set the flag fell to true
    if (prevDropTime+velDropTimeThresh<nowtime) {
      
      fell=true;

    }

  // Otherwise reset the previous drop time
  } 
  else {

    prevDropTime=nowtime;

  }

  static unsigned long prevStopTime=nowtime;

  // If the velocity is small and the CanSat has fallen
  if ((abs(velocity)<velStopThresh) && fell) {

    // If the CanSat hasn't been moving for a while, start the buzzer
    if (nowtime>prevStopTime+velStopTimeThresh) {

      bell.buzzerStart();

    }

  // Otherwise reset the previous stop time 
  } 
  else {

    prevStopTime=nowtime;

  }

  // Read battery percentage
  float battVolt = batt.voltage();

  // Check if the battery voltage is safe
  if (battVolt < minBattVolt) {
     softState=softState+16;
  }

  // Get accelerometer data
  imu::Vector<3> accelData = accel.getData();

  // Send data
  int writeError=  logger.send(packetCount);
  writeError += logger.send(nowtime);
  writeError += logger.send(pressure);
  writeError += logger.send(temperature);
  writeError += logger.send(altitude);
  writeError += logger.send(velocity);
  writeError += logger.send(battVolt);
  writeError += logger.send(softState);
  writeError += logger.send(accelData.x());
  writeError += logger.send(accelData.y());
  writeError += logger.send(accelData.z());
  writeError += logger.Flush();

  // Reset softState 
  softState=0;

  // Check if there were any write errors
  if (writeError != 0) {

    softState=8;

  }
  packetCount+=1;
  
  // Delay (for testing only)
  delay(1000);
}
