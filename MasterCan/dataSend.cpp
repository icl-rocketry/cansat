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

// Terminate line and flush serial and SD to avoid errors
int logger::Flush(){

  Serial.println();

  // Open SD card file
  myFile=SD.open(_fileName, FILE_WRITE);

  if (myFile) { // If the file has been opened succesfully

    // Write data to SD card
    myFile.println();

    return 0;

  } else {

    return 1;
  }

  Serial.flush();
  myFile.close();
}
