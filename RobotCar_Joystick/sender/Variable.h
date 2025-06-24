// Mylib >> MacAddres esp
#include <MyEsp_address.h>

#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

class Control {
private:
  const uint8_t Vx_Pin = 34;  //D35
  const uint8_t Vy_Pin = 32;  //D32

  const uint8_t sampling = 10;
  const int16_t dead_zone1 = 99;  
  const int16_t dead_zone2 = 180; 
  uint16_t sumVx = 0;
  uint16_t sumVy = 0;

  const uint16_t calib = 2047;
  unsigned long lastReadTime = 0;  // บันทึกเวลาการอ่านค่าล่าสุด ควบคุมการอ่านค่าเป็นระยะ

public:
  const uint8_t led_Vx_pin;
  const uint8_t led_Vy_pin;
  const uint8_t led_espNow_Pin;
  int joy_send_Vx;
  int joy_send_Vy;
  int16_t avgVx;
  int16_t avgVy;

  Control()
    : led_Vx_pin(33),      // GPIO 33 D33
      led_Vy_pin(27),      // GPIO 27 (D27)
      led_espNow_Pin(25),  // GPIO 25 (D25)
      joy_send_Vx(0),
      joy_send_Vy(0),
      avgVx(0),
      avgVy(0) {}

  void read_adc() {
    // อ่านค่าทุกๆ 10 ms
    if (millis() - lastReadTime >= 10) {
      lastReadTime = millis();  // อัปเดตเวลาการอ่านค่าล่าสุด
      sumVx = 0;
      sumVy = 0;

      for (uint8_t i = 0; i < sampling; i++) {
        sumVx += analogRead(Vx_Pin);
        sumVy += analogRead(Vy_Pin);
        delayMicroseconds(100);  //หน่วงเวลาการอ่านหน่อย 1ms
      }
      avgVx = (sumVx / sampling) - calib;
      avgVy = (sumVy / sampling) - calib;

      // จัดการ Dead-zone
      if (abs(avgVx) < dead_zone1) avgVx = 0;
      if (abs(avgVy) < dead_zone2) avgVy = 0;

      // Map ค่า 0-4096 เป็น -1023 ถึง 1023
      joy_send_Vx = map(avgVx, -2048, 2048, -1023, 1023);
      joy_send_Vy = map(avgVy, -2048, 2048, -1023, 1023);
      led();
    }
  }

  void led() {
    // ถ้า Joystick ขยับ ให้เปิดไฟ LED
    digitalWrite(led_Vx_pin, (joy_send_Vx != 0));
    digitalWrite(led_Vy_pin, (joy_send_Vy != 0));
  }
};

#endif