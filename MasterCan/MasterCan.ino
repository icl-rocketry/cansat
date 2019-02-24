/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"
#include "BMP388.h"

// Initialise variables used to calculate velocity
float prevAltitude;
float velocity;
unsigned long prevTime;

// Initialise altitude and current time, then set a preliminary value
unsigned long nowtime = millis();
float altitude = 0;

// set the pins used
const int battHealthPin = 2;

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);
  Wire.begin();

  // Initialise SD card
  if (!SDC.SDSetup(4)) {
    Serial.println("ERROR: SD initialisation failed");
    return;
  }

/* Commented to save space
   Check if the output file exists on the SD card, and create it if it doesn't
  if (!SDC.fileCheck("results.txt")) {
    Serial.println("WARNING: Results file not found, created");
  }
*/

  // Initialise accelerometer
  if (!accel.accelSetup())
  {
    Serial.println("ERROR: Accelerometer initialisation failed");
  }

  // Initialise BMP388
  if (!alt.Setup()) {
    Serial.println("ERROR: Pressure sensor initialisation failed");
  }

  // Initialise battery percentage reader
  batt.Setup(battHealthPin);

}


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  prevTime = nowtime;
  prevAltitude = altitude;

  // Set nowTime to the time in ms since the arduino turned on
  nowtime = millis();

  // Get the altimeter data
  static float altitude = alt.Alt();
  float temperature = alt.Temp();
  float pressure = alt.Pres();

  // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
  velocity = (altitude - prevAltitude) / (nowtime / 1000 - prevTime / 1000);

  // Get accelerometer data
  float* accelData = accel.getData();

  // Read battery percentage
  float battPercent=batt.percent(battHealthPin);
  
  // Print data to serial for debugging
  Serial.print("Time: ");
  Serial.print(nowtime);
  Serial.println(" ms");
  Serial.print("Temperature: ");
  Serial.print(temperature, DEC);
  Serial.println(" *0.1 deg C");
  Serial.print("Pressure: ");
  Serial.print(pressure, DEC);
  Serial.println(" Pa");
  Serial.print("Altitude: ");
  Serial.print(altitude, 2);
  Serial.println(" m");
  Serial.print("Velocity: ");
  Serial.print(velocity);
  Serial.println(" m/s");
  Serial.print("Battery percentage: ");
  Serial.print(battPercent);
  Serial.println("%");
  Serial.print("X: "); Serial.print(accelData[0]); Serial.print("  ");
  Serial.print("Y: "); Serial.print(accelData[1]); Serial.print("  ");
  Serial.print("Z: "); Serial.print(accelData[2]); Serial.print("  "); Serial.println("m/s^2 ");
  Serial.println();

  // Write data to SD Card
  if ( !SDC.Write(nowtime, temperature, pressure, altitude, velocity, battPercent, accelData) ) {
    Serial.println("ERROR: Failed to write to SD Card");
  }

  // Delay (for testing only)
  delay(1000);
}
