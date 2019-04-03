/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "VibMotor.h"
#include "Bell.h"
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"
#include "BMP388.h"
#include "WordCreator.h"

// set the pins used
const int battHealthPin = 2;
const int calibAlt;
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
transportObject trObj;

// Vertical speed below which CanSat will assume it is stationary, and descending.
const float velStopThresh=2;
const float velDropThresh=3;

// Respective times that CanSat has to stay in its state for to count as stationary and descending
const float velStopTimeThresh=3;
const float velDropTimeThresh=3;

unsigned int nowtime = millis();
unsigned int packetCount=1;
float altitude = 0;
byte softState=0;

/* softState variable is a single byte where the error codes are powers of two that are added.
The individual errors can be found by getting the softState and dividing it into its individual powers of two
The powers of 2 and their corresponding errors are as follows:

1	-	WARNING: Results file not detected, created
2	-	ERROR: SD card initialisation failed
4	-	ERROR: BNO055 initialisation failed
8	-	ERROR: BMP388 initialisation failed
16	-	ERROR: Failed to write to SD Card
32	-	ERROR: Battery voltage low
*/

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);
  //Wire.begin();

  // Initialise bell library
  bell.start();

  // Initialise SD card
  if (!SDC.start(10)) {
        softState=softState=2;
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
  if (!alt.start())
  {
	softState=softState+8;
	Serial.println(softState);
	bell.fatalError();
  }

  // Start vibration motor
  vib.vibstart();
}


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  int prevTime = nowtime;
  float prevAltitude = altitude;

  // Set nowtime to the time in ms since the arduino turned on
  nowtime = millis();

  // Get the altimeter data
  altitude = alt.Alt();
  float temperature = alt.Temp();
  float pressure = alt.Pres();

  // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
  float velocity = (altitude - prevAltitude) / (nowtime / 1000 - prevTime / 1000);


  static bool fell=false;
  static int prevDropTime=nowtime;

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

  static int prevStopTime=nowtime;

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

  // Get accelerometer data
  imu::Vector<3> accelData = accel.getData();

  // Read battery percentage
  float battVolt = batt.voltage();

  // Check if the battery voltage is safe
  if (battVolt < minBattVolt) {
     softState=softState+32;
  }
  
  // Concatenate the data to a single string
  String dataString=trObj.create(packetCount, nowtime, pressure, temperature,  altitude, velocity, battVolt, softState, accelData);

  // Reset softState 
  softState=0;

  // Write data to SD Card and Serial
  if ( !SDC.Write(dataString) ) {
	softState=softState+16;
	bell.error();
  }
  Serial.println(dataString);

  packetCount+=1;
  
  // Delay (for testing only)
  delay(1000);
}
