//--stm32--
#include "sensor.h"
#include "Send_value.h"

void setup() {
  Serial.begin(115200);
  for(int i = 5; i < 8; i++){
  attachInterrupt(digitalPinToInterrupt(SP[i]), checkSwitch, FALLING);
  }
}
void loop() {
}
