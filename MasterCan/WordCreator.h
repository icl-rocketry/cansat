/* library to combine all the data to send into a single string */

#ifndef WordCreator_h
#define WordCreator_h

#include "Arduino.h"
#include <utility/imumaths.h>

class transportObject
{

  public:

  transportObject();
  String create(int packetCount, int nowTime, float pressure, float temperature,  float altitude, float velocity, float battPercent, int softState, imu::Vector<3> accelData);

};


#endif
