#include "esp32-hal-adc.h"
#include "sensor.h"

#include <Arduino.h>
#include <math.h>

void initSensors(SensorData* arr, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    arr[i].id = i;
    arr[i].pin = A0 + i; // ตัวอย่าง map A0..A3
    arr[i].value = NAN; // ใช้ NAN เพื่อบ่งว่าไม่มีค่า (เหมือน NULL)
    arr[i].valid = false;
    arr[i].lastUpdate = 0;
    pinMode(arr[i].pin, INPUT);
  }
}

// ------------------ Read sensors ------------------
// ตัวอย่างตรรกะ: ถ้า raw == 0 ให้ถือว่าเป็น invalid (ขึ้นกับเซ็นเซอร์จริง)
void readSensors(SensorData* arr, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    int raw = analogRead(arr[i].pin);
    
    // เช็คความถูกต้องของค่าดิบ
    if (raw <= 0) {
      arr[i].valid = false;
      arr[i].value = NAN;
    } else {
      arr[i].valid = true;
      arr[i].value = (float)raw;
      arr[i].lastUpdate = millis();
    }
  }
}