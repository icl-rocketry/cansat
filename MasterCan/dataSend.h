/* Library to send data to both Serial and the SD card */

#ifndef dataSend_h
#define dataSend_h

#include "Arduino.h"
#include <utility/imumaths.h>
#include <SD.h>
#include <SPI.h>

class logger
{
  private:

  File myFile;
  String _fileName;
  char _delimiter;

  public:
  
  bool SDstart(int SDPin);
  logger(String fileName, char delimiter);
  template <typename T> int send(T data);
  int Flush();

};


#endif

// Sends data to both serial and SD card
template<typename T> int logger::send(T data){

  Serial.print(data); 
  Serial.print(_delimiter);
  
  // Open SD card file
  myFile=SD.open(_fileName, FILE_WRITE);

  if (myFile) { // If the file has been opened succesfully

    // Write data to SD card
    myFile.print(data);
    myFile.print(_delimiter);

    return 0;

  } else {

    return 1;
  }
  
}
