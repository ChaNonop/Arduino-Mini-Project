#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>
#include <avr/sleep.h>
#include "ProjectPins.h"

class PowerManager {
public:
  PowerManager();
  void begin();
  void goToSleep();
  
  // ISR (Interrupt Service Routine) ต้องเป็นฟังก์ชัน global หรือ static
  static void wakeUp(); 

private:
  // volatile keyword is important for variables used in ISRs
  static volatile bool isAwake; 
};

#endif
