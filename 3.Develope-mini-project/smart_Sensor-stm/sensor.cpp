#include "sensor.h"

#include <Arduino.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"


void checkswitch(){
}
void setup_dht{
  dht.begin();
}
void initSensors(SensorData* arr, size_t n,sampling* sample) {
  for (size_t i = 0; i < 3; ++i) { //อ่านค่า analog
    arr[i].id = i;
    arr[i].pin = SP[i]; // ตัวอย่าง map ขา
    arr[i].value = NAN; // ใช้ NAN เพื่อบ่งว่าไม่มีค่า (เหมือน NULL)
    arr[i].valid = false;
    arr[i].lastUpdate = 0;
    pinMode(arr[i].pin, INPUT);
  }
  for(size_t i = 5; i< n; ++i){ //read digital (digit interrupt)
    arr[i].id = i;
    arr[i].pin = SP[i];
    arr[i].value = NAN;
    arr[i].valid = false;
    arr[i].lastUpdate = 0;
    pinMode(arr[i].pin,INPUT);
  }
}
// ------------------ Read sensors ------------------
// ตัวอย่างตรรกะ: ถ้า raw == 0 ให้ถือว่าเป็น invalid (ขึ้นกับเซ็นเซอร์จริง)
void readSensors(SensorData* arr, size_t n) {
  for (size_t i = 0; i < 3; ++i) {
    int raw = analogRead(arr[i].pin);
  }
    // เช็คความถูกต้องค่า
    if (raw <= 0) {
      arr[i].valid = false;
      arr[i].value = NAN;
    } else {
      arr[i].valid = true;
      arr[i].value = (float)raw;
      arr[i].lastUpdate = millis();
    }
    // Get temperature event and print its value.
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  dht.temperature().getEvent(&event);
  if (isnan(!event.temperature) && isnan(!event.relative_humidity)) {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));

    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  else {
    return; // failed ให้อ่านค่าใหม่
  }
}