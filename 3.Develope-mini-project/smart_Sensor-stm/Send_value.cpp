#include "Send_value.h"

void set_Rf{
  if (!rf_driver.init()) {
    Serial.println(F("RF driver init failed!"));
  } 
  else {
    Serial.println(F("RF driver init success."));
  }
}