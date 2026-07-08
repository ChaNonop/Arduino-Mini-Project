#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <Arduino.h>
#include "DHT.h"
#include "Adafruit_VEML7700.h"

class Sensor{
  private:
  const byte dht;
  const byte Pump_pin;
  const byte temp_water_Pin;
  const byte moisture_Pin;

  const byte buzzle;
  const byte IR_recieve_Pin;

  const byte sampling;
  unsigned long now;
  unsigned long read_time;

  Adafruit_VEML7700 veml = Light_sensor(); 
}
#endif