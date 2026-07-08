#include "esp32-hal-uart.h"
#ifndef SENSOR_VALUE_H
#define SENSOR_VALUE_H

#include <RH_ASK.h>
#include <SPI.h> 

// bitRate=1000, rxPin=0 (ไม่ใช้), txPin=1, pttPin=0
byte rxpin = 0;
byte txpin = 1;
RH_ASK rf_driver(1000, 3, txpin, rxpin); 


#endif