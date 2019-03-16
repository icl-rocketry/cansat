/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "Buzzer.h"
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"
#include "BMP388.h"
#include "WordCreator.h"

// set the pins used
const int battHealthPin = 2;
const int calibAlt;
const int buzzerpin=3;

Buzzer buzzer(buzzerpin);
BNO055 accel;
SDCard SDC;
batt batt(battHealthPin);
BMP388 alt(calibAlt);
transportObject trObj;

unsigned int nowtime = millis();
unsigned int packetCount=1;
float altitude = 0;
int softState=0;

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);
  //Wire.begin();

  // Initialise SD card
  if (!SDC.start(10)) {
    Serial.println("ERROR: SD initialisation failed");
    return;
  }

  // Check if the output file exists on the SD card, and create it if it doesn't
  if (!SDC.fileCheck("results.txt")) {
    Serial.println("WARNING: Results file not found, created");
  }


  // Initialise accelerometer
  if (!accel.start())
  {
    Serial.println("ERROR: Accelerometer initialisation failed");
  }

}


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  int prevTime = nowtime;
  float prevAltitude = altitude;

  // Set nowTime to the time in ms since the arduino turned on
  nowtime = millis();

  // Get the altimeter data
  altitude = alt.Alt();
  float temperature = alt.Temp();
  float pressure = alt.Pres();

  // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
  float velocity = (altitude - prevAltitude) / (nowtime / 1000 - prevTime / 1000);

  // Get accelerometer data
  imu::Vector<3> accelData = accel.getData();

  // Read battery percentage
  float battVolt = batt.voltage();

  // Concatenate the data to a single string
  String dataString=trObj.create(packetCount, nowtime, pressure, temperature,  altitude, velocity, battVolt, softState, accelData);

  // Write data to SD Card and Serial
  if ( !SDC.Write(dataString) ) {
    Serial.println("ERROR: Failed to write to SD Card");
  }
  Serial.println(dataString);

  packetCount+=1;
  
  // Delay (for testing only)
  delay(1000);
}
