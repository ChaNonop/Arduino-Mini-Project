#ifndef AWAKE_H
#define AWAKE_H

#include <Arduino.h>
#include "Variable_Pin.h"

class wake : Obj {
public:
    const byte ldr_awake = D2;
    Volatile bool awake = false;
  void  wakeupNow(){
    awake = true;
  }
  void gotosleep(){
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    adcsra &= ~(1 << ADEN);
    SLEEP_MODE();
    sleep_disable;
    adcsra |= (1 << ADEN); 
  }

}
#endif