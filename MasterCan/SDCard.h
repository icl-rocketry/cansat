/* Interface library for the SD Card */
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
    bool SDSetup(int SDPin) {
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
    bool Write(int nowTime, float temperature, float pressure, float altitude, float velocity, float* accelData) {

      // Open results.txt so we can start writing to it
      myFile = SD.open("results.txt", FILE_WRITE);

      if (myFile) { // If the file has been opened succesfully

        // Write data to SD card
        myFile.print("Time: ");
        myFile.print(nowTime);
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
        myFile.print("X: "); myFile.print(accelData[0]); myFile.print("  ");
        myFile.print("Y: "); myFile.print(accelData[1]); myFile.print("  ");
        myFile.print("Z: "); myFile.print(accelData[2]); myFile.print("  "); myFile.println("m/s^2 ");
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
