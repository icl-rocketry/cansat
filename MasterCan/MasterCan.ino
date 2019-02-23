/* BMP085 Extended Example Code
  by: Jim Lindblom
  SparkFun Electronics
  date: 1/18/11
  updated: 2/26/13
  license: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/
  
  Get pressure and temperature from the BMP085 and calculate 
  altitude. Serial.print it out at 9600 baud to serial monitor.

  Update (7/19/11): I've heard folks may be encountering issues
  with this code, who're running an Arduino at 8MHz. If you're 
  using an Arduino Pro 3.3V/8MHz, or the like, you may need to 
  increase some of the delays in the bmp085ReadUP and 
  bmp085ReadUT functions.
*/

// Define Libraries required
#include <Wire.h>
#include "BNO055.h"
#include "Batt_health.h"
#include "SDCard.h"

// Initialise variables used to calculate velocity
float prevAltitude;
float velocity;
unsigned long prevTime;

// Initialise altitude and current time, then set a preliminary value 
unsigned long nowtime = millis();
float altitude=0;

// set the pins for the buzzer and LED
const int buzzerpin=3;
const int LEDpin=5;

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);
  Wire.begin();

  // If the SD card cannot initialise, stop program and beep error
  if (!SDCard.SDSetup(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Check if the output file exists on the SD card, and create it if it doesn't
  SDCard.fileCheck("results.txt");

  if(!BNO055.accelSetup())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    errorBeep();
  }

  // Turn on work LED
  digitalWrite(LEDpin, HIGH);
  }


void loop()
{

  // Set prevTime and prevAltitude to the current values of time and altitude
  prevTime=nowtime;
  prevAltitude=altitude;

  // Set nowTime to the time in ms since the arduino turned on
  nowtime = millis();

  // Set temperature to the measured temperature
  temperature = bmp085GetTemperature(bmp085ReadUT());

  // Set pressure to the measured pressure
  pressure = bmp085GetPressure(bmp085ReadUP());

  // Calculate the altitude using the pressure, then set variable altitude to that value
  altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));

  // Calculate vertical velocity using differences in altitude over difference in time, save to velocity
  velocity=(altitude-prevAltitude)/(nowtime/1000-prevTime/1000);

  // Get accelerometer data
  accelData=BNO055.getData();

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
  Serial.print("X: "); Serial.print(accelData[0]); Serial.print("  ");
  Serial.print("Y: "); Serial.print(accelData[1]); Serial.print("  ");
  Serial.print("Z: "); Serial.print(accelData[2]); Serial.print("  ");Serial.println("m/s^2 ");
  Serial.println();

  // Write data to SD Card
  if (!SDCard.Write(nowtime,temperature,pressure,altitude,velocity,accelData) {
    Serial.println("Failed to write to SD Card!");
  }
  
  delay(1000);
}

//Code to Start buzzer
void buzzerStart()
{
  tone(buzzerpin, 10000);
}

// Code to end buzzer
void buzzerStop()
{
  noTone(buzzerpin);
}

void errorBeep()
{
  while (true)
  {
    buzzerStop();
    delay(500);
    buzzerStart();
    delay(500);
  }
}
