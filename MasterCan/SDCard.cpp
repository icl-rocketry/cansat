/* Interface library for the SD Card */

// Include neccessary libraries
#include "Arduino.h"
#include "SDCard.h"
#include <SD.h>
#include <SPI.h>
#include <utility/imumaths.h>

// Create new SDCard instance
SDCard::SDCard() 
{
  File myFile;
}

// Returns whether the SD card has initialised succesfully
bool SDCard::start(int SDPin) {
  return SD.begin(SDPin);
}

// Returns whether the input file exists, then creates it
bool SDCard::fileCheck(char fileName) {

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
bool SDCard::Write(String data) {

  // Open results.txt so we can start writing to it
  myFile = SD.open("results.txt", FILE_WRITE);

  if (myFile) { // If the file has been opened succesfully

    // Write data to SD card
    myFile.println(data);

    // close the file:
    myFile.close();

    // Show that the operation was completed succesfully
    return true;

  } else {

    // Otherwise return false
    return false;
  }
}
