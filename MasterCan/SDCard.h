/* Interface library for the SD Card */

// Include neccessary libraries
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>

class SDCard
{
  private:

    // Set a variable for the file to write to (required for SD card)
    File myFile;

  public:

    // Returns whether the SD card has initialised succesfully
    bool start(int SDPin) {
      return SD.begin(SDPin);
    }

    // Returns whether the input file exists, then creates it
    bool fileCheck(char fileName) {

      // Check if the file exists
      if (SD.exists(fileName)) {

        // If this is the case, return true
        return true;
      } else {

        // Otherwise create the file
        myFile = SD.open(fileName, FILE_WRITE);
        myFile.close();

        // Return false to show that it has been created
        return false;
      }
    }

    // Function that writes the input data to the SD Card
    bool Write(int nowTime, float temperature, float pressure, float altitude, float velocity, float battPercent, imu::Vector<3> accelData) {

      // Open results.txt so we can start writing to it
      myFile = SD.open("results.txt", FILE_WRITE);

      if (myFile) { // If the file has been opened succesfully

        // Write data to SD card
        myFile.print("t: ");// Time 
        myFile.print(nowTime);//ms
        myFile.print("T: "); // Temperature
        myFile.print(temperature, DEC);//*0.1 deg C
        myFile.print("P: ");// Pressure
        myFile.print(pressure, DEC); //Pa
        myFile.print("Alt: "); // Altitude
        myFile.print(altitude, 2); //m
        myFile.print("V: "); // Velocity
        myFile.print(velocity); //m/s
        myFile.print("Batt: "); // Battery percentage
        myFile.print(battPercent);
        myFile.print("X: "); myFile.print(accelData.x()); myFile.print("  ");
        myFile.print("Y: "); myFile.print(accelData.y()); myFile.print("  ");
        myFile.print("Z: "); myFile.print(accelData.z()); myFile.print("  ");
        myFile.println();

        // close the file:
        myFile.close();

        // Show that the operation was completed succesfully
        return true;

      } else {

        // Otherwise return false
        return false;
      }
    }
};

SDCard SDC;
