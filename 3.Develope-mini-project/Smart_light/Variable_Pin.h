#include <stdint.h>
#ifndef VARIABLE_Pin_H
#define VARIABLE_Pin_H

#include <Arduino.h>
#include <avr/pgmspace.h>
//#include "DHT.h"
#include "DHT_Async.h"

#define DHT_SENSOR_TYPE DHT_TYPE_11

class Obj {
private:
  const Power_led = D11; 
  //const byte Led_power[3] PROGMEM= { D10, D9, D8 };
  const byte buzzle_pin PROGMEM= A0;

  static const int DHT_SENSOR_PIN = D4;
  DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

  //  ldr
  const byte Ldr_pin = A1;
  //PIR
  const byte Pir_pin = D3;

  
  const float Rref = 10000.0;
  const float Gamma = 0.7;
  const float RL = 50; // หน่วยเป็น kΩ

public:
  uint_fast16_t Value_ldr[10];
  bool Value_Pir=0;

  uint8_t sampling=5;
  uint8_t sampleindex=0;
  uint_fast16_t sum =0;
  uint_fast16_t avg_ldr= 0;
  uint_fast16_t Voltage =0;
  obj(){};

  void reading(){
    bool Value_pir = digitalRead(Pir_pin);

    Value_ldr[sampleindex] = analogRead(Ldr_pin);
    sampleindex =(sampleindex+1)%sampling;

    long now = millis();

    for(uint8_t i=0; i<sampling;i++){
      sum[i] += Value_ldr;
      avg_ldr = sum/sampling;
    }
    voltage = avg_ldr * 5.0 / 1023;

      if (voltage > 0) {
        R_ldr = Rref * (voltage / (5.0 - voltage));

      } else {
        R_ldr = -1;
      }

      lux = calculateLux(R_ldr);
      return avg_ldr;
  }
     float calculateLux(float r) {
      if (r <= 0) return 0;
      float numerator = RL * 1000.0 * pow(10, Gamma); // RL หน่วย kΩ → คูณ 1000 เป็น Ω
      float l = pow(numerator / r, 1.0 / Gamma);
      if (l > 10000) l = 10000;
      return l;
    }

  void DHT(){
    bool measure_environment(float *temperature, float *humidity) {
    static unsigned long measurement_timestamp = millis();
    if (millis() - measurement_timestamp > 4000ul) {
        if (dht_sensor.measure(temperature, humidity)) {
            measurement_timestamp = millis();
            return (true);
        }
    }
    return (false);
    }
    if (measure_environment(&temperature)) {
    Serial.print("T = ");
    Serial.print(temperature, 1);
    Serial.print(" deg. C");
    }
  }


}
#endif