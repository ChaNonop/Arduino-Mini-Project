#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include <IRremote.hpp>
#include "RTClib.h"  // สำหรับ RTC
#include "DHT.h"
#include "Adafruit_VEML7700.h"

#define DHTPIN 2  //ขาที่ต่อเซนเซอร์ DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define IR_RECEIVE_PIN 11  // IR receiver pin 11

class Sensor {
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
public:

  //RTC_DS3231 rtc; // Address 0x68
}
#endif