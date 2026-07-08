#include <Pin_arduino.h>
#include "Sensor_data"

Sensor_data::Sensor(){
  float lux = veml.readLux(VEML_LUX_AUTO);
  

}