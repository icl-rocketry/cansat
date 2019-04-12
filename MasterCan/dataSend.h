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
  
  logger(String fileName, char delimiter);
  bool SDstart(int SDPin);
  bool open();
  template <typename T> void send(T data);
  template <typename T2> void SDwrite(T2 data);
  void Flush();
  void SDflush();

};


#endif

// Writes data to SD card
template<typename T2> void logger::SDwrite(T2 data){

  myFile.print(data);
  myFile.print(_delimiter);

}

// Sends data to both serial and SD card
template<typename T> void logger::send(T data){

  Serial.print(data); 
  Serial.print(_delimiter);
  
  // Write data to SD card
  logger::SDwrite(data);

}
