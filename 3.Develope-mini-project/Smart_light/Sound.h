#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Variable_Pin.h"

class sound :Obj{
  private:
  const byte buzzle_pin PROGMEM= ;
  int wave[sampling];
  bool vibration_senser;
  int sum_wave;
  public:
  sound(){}

void respons(){
  for(uint8_t i = 0; i < sampling; i++){
    wave[i] = analogRead(sound_sensor_pin);
    sum_wave += wave[i];
    vibration_senser = digitalRead(vibration_senser_pin);
    }

    static bool prevState = LOW;
  bool soundState = digitalRead(SOUND_PIN);

  if (soundState == HIGH && prevState == LOW) {
    unsigned long now = millis();
    if ((now - lastClapTime) < 1000 && (now - lastClapTime) > 200) {
      clapCount++;
    } else {
      clapCount = 1;
    }
    lastClapTime = now;

    Serial.print("Clap detected! Count: ");
    Serial.println(clapCount);

    if (clapCount >= 2) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // toggle
      clapCount = 0;
    }
  }

  prevState = soundState;
}


#endif