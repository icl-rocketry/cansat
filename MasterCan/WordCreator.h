/* library to combine all the data to send into a single string */

#ifndef WordCreator_h
#define WordCreator_h

#include "Arduino.h"
#include <utility/imumaths.h>

class transportObject
{

  public:

  transportObject();
  String create(int nowTime, float temperature, float pressure, float altitude, float velocity, float battPercent, imu::Vector<3> accelData);

};


#endif
