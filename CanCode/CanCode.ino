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

/*
This project's Website link: https://www.sparkfun.com/tutorials/253
Print SD timestamp arduino: https://gist.github.com/dhhagan/f1cca00cdb479558b6d1
https://www.pjrc.com/teensy/td_libs_Time.html
https://github.com/PaulStoffregen/Time/blob/master/examples/TimeSerial/TimeSerial.ino
https://github.com/feilipu/Arduino_RTC_Library
https://github.com/SofaPirate/Chrono 
*/

// Define Libraries required
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting (0-4), higher = better resoltion but more power 

// Set a variable for the file to write to (required for SD card)
File myFile;

// Calibration values
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 
short temperature;
long pressure;

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)

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

// Give address to accelerometer
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void setup()
{
  // Initialise serial and wire libraries
  Serial.begin(9600);
  Wire.begin();

  // Calibrate altimeter
  bmp085Calibration();

  // If the SD card cannot initialise, stop program and beep error
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    errorBeep();
    return;
  }
  Serial.println("initialization done.");

  // Check if the output file exists on the SD card, and create it if it doesn't
  if (SD.exists("results.txt")) {
    Serial.println("results.txt exists.");
  } else {
    Serial.println("results.txt doesn't exist,creating...");
    myFile = SD.open("results.txt", FILE_WRITE);
    myFile.close();
  }

  if(!accel.begin())
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

  /* Get a new accelerometer sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

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
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  Serial.println();

  // Open results.txt so we can start writing to it
   myFile = SD.open("results.txt", FILE_WRITE);

  if (myFile) { // If the file has been opened succesfully

    // Write data to SD card
    Serial.print("Writing values to results.txt...");
    myFile.print("Time: ");
    myFile.print(nowtime);
    myFile.println(" ms");
    myFile.print("Temperature: ");
    myFile.print(temperature, DEC);
    myFile.println(" *0.1 deg C");
    myFile.print("Pressure: ");
    myFile.print(pressure, DEC);
    myFile.println(" Pa");
    myFile.print("Altitude: ");
    myFile.print(altitude, 2);
    myFile.println(" m");
    myFile.print("Velocity: ");
    myFile.print(velocity);
    myFile.println(" m/s");
    myFile.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    myFile.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    myFile.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
    myFile.println();
    
    // close the file:
    myFile.close();
    Serial.println("done.");
    
  } else {
    
    // if the file didn't open, print an error:
    Serial.println("error opening results.txt");
    errorBeep();
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

// LOW LEVEL DATA READING AND INTERPRETATION
//------------------------------------------

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}
