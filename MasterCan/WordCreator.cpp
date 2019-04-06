/* library to combine all the data to send into a single string */

// Libraries
#include "Arduino.h"
#include "WordCreator.h"
#include <utility/imumaths.h>

// Create new instance of library
transportObject::transportObject() {
  
}

// Concatenates all the data into a single string
String transportObject::create(unsigned long packetCount, unsigned long nowTime, float pressure, float temperature,  float altitude, float velocity, float battVolt, byte softState, imu::Vector<3> accelData){
  // Delimiter is ":"
  return String(packetCount)+":"+String(nowTime)+":"+String(pressure,1)+":"+String(temperature,1)+":"+String(altitude,1)+":"+String(velocity,1)+":"+String(battVolt,0)+":"+String(softState)+":"+String(accelData.x(),2)+":"+String(accelData.y(),2)+":"+String(accelData.z(),2);
  
}
