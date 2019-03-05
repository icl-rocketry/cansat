/* Main code file for the ICLR CanSat */

// Define Libraries required
#include <Wire.h>
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"
#include "BMP388.h"

BNO055 accel;

// Initialise altitude and current time, then set a preliminary value
unsigned long nowtime = millis();
float altitude = 0;

// set the pins used
const int battHealthPin = 2;

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

  // Initialise BMP388
  if (!alt.start()) {
    Serial.println("ERROR: Pressure sensor initialisation failed");
  }

  // Initialise battery percentage reader
  batt.start(battHealthPin);

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
  float battPercent = batt.percent(battHealthPin);

  // Print data to serial for debugging
  Serial.print("t: ");// Time
  Serial.println(nowtime);//ms
  Serial.print("T: "); // Temperature
  Serial.println(temperature, DEC);//*0.1 deg C
  Serial.print("P: ");// Pressure
  Serial.println(pressure, DEC); //Pa
  Serial.print("Alt: "); // Altitude
  Serial.println(altitude, 2); //m
  Serial.print("V: "); // Velocity
  Serial.println(velocity); //m/s
  Serial.print("Batt: "); // Battery percentage
  Serial.println(battPercent);
  Serial.print("X: "); Serial.print(accelData.x()); Serial.print("  ");
  Serial.print("Y: "); Serial.print(accelData.y()); Serial.print("  ");
  Serial.print("Z: "); Serial.print(accelData.z()); Serial.print("  ");
  Serial.println();

  // Write data to SD Card
  if ( !SDC.Write(nowtime, temperature, pressure, altitude, velocity, battPercent, accelData) ) {
    Serial.println("ERROR: Failed to write to SD Card");
  }

  // Delay (for testing only)
  delay(1000);
}
