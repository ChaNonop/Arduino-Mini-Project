#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "DHT_Async.h"
#include "ProjectPins.h"

class SensorManager {
public:
  SensorManager(); // Constructor
  void begin();    // Setup function
  void update();   // Main update loop

  // Getter functions to get sensor values
  bool motionDetected();
  float getTemperature();
  float getHumidity();
  float getLightLevel();

private:
  // DHT Sensor object
  DHT_Async dht_sensor;

  // Variables for storing sensor data
  bool currentMotionState;
  float currentTemperature;
  float currentHumidity;
  float currentLightLevel;

  // Timing for non-blocking reads
  unsigned long lastDhtRead;
  unsigned long lastLdrRead;
  
  // Helper functions
  bool readDht();
  void readLdr();
  void readPir();
};

#endif
