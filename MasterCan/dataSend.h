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
  bool open();
  template <typename T> void send(T data);
  int Flush();

};


#endif

// Sends data to both serial and SD card
template<typename T> void logger::send(T data){

  Serial.print(data); 
  Serial.print(_delimiter);
  
  // Write data to SD card
  myFile.print(data);
  myFile.print(_delimiter);

}
