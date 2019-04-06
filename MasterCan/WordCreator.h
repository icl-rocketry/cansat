/* library to combine all the data to send into a single string */

#ifndef WordCreator_h
#define WordCreator_h

#include "Arduino.h"
#include <utility/imumaths.h>

class transportObject
{

  public:

  transportObject();
  String create(unsigned long packetCount, unsigned long nowTime, float pressure, float temperature,  float altitude, float velocity, float battVolt, byte softState, imu::Vector<3> accelData);

};


#endif
