/* Library to send data to both Serial and the SD card */

// Libraries
#include "Arduino.h"
#include "dataSend.h"
#include <utility/imumaths.h>
#include <SD.h>
#include <SPI.h>

// Create new instance of library
logger::logger(String fileName, char delimiter) {
  
  _fileName=fileName;
  _delimiter=delimiter;
}

// Returns whether the SD card has initialised succesfully
bool logger::SDstart(int SDPin) {
  return SD.begin(SDPin);
}

// Function to open the SD card
bool logger::open() {
  myFile=SD.open(_fileName, FILE_WRITE);

  return myFile;
}

// Terminate line and flush serial and SD to avoid errors
int logger::Flush(){

  Serial.println();
  myFile.println();

  Serial.flush();
  myFile.close();
}
