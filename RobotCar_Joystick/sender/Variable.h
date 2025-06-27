// Mylib >> MacAddres esp
#include <MyEsp_address.h>

#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

class Control {
private:
  const uint8_t Vx_Pin = 34;  //D34
  const uint8_t Vy_Pin = 32;  //D32

  const uint8_t sampling = 10;
  const int16_t dead_zone1 = 100;
  const int16_t dead_zone2 = 204;

  const int16_t calib_L = 2049;
  const int16_t calib_R = 2049;
  unsigned long lastReadTime = 0;  // บันทึกเวลาการอ่านค่าล่าสุด ควบคุมการอ่านค่าเป็นระยะ

public:
  const uint8_t led_left_pin;
  const uint8_t led_right_pin;
  const uint8_t led_espNow_Pin;
  volatile int joy_send_left;
  volatile int joy_send_right;
  int32_t setVx;
  int32_t setVy;

  Control()
    : led_left_pin(33),    // GPIO 33 D33
      led_right_pin(27),   // GPIO 27 (D27)
      led_espNow_Pin(25),  // GPIO 25 (D25)
      joy_send_left(0),
      joy_send_right(0),
      setVx(0),
      setVy(0) {}

  void read_adc() {
    // อ่านค่าทุกๆ 1 ms
    if (micros() - lastReadTime >= 100) {
      lastReadTime = micros(); 
      uint32_t sumVx = 0;
      uint32_t sumVy = 0;

      for (uint8_t i = 0; i < sampling; i++) {
        sumVx += analogRead(Vx_Pin);
        sumVy += analogRead(Vy_Pin);
      }
      uint32_t avgVx = (sumVx / sampling);
      uint32_t avgVy = (sumVy / sampling);

      // คำนวณค่า set จากจุดศูนย์กลาง
      setVx = avgVx - calib_L;
      setVy = avgVy - calib_R;

      if (abs(setVx) < dead_zone1) setVx = 0;
      if (abs(setVy) < dead_zone2) setVy = 0;

      joy_send_left = map(setVx, -calib_L, calib_L, -1023, 1023);
      joy_send_right = map(setVy, -calib_R, calib_R, -1023, 1023);

      int last_joy_left = 0;
      int last_joy_right = 0;

      if (joy_send_left != 0 || joy_send_right != 0) {
        led();
        last_joy_left = joy_send_left;
        last_joy_right = joy_send_right;
      }
    }
  }

  void led() {
    // ถ้า Joystick ขยับ ให้เปิดไฟ LED
    digitalWrite(led_left_pin, (joy_send_left != 0));
    digitalWrite(led_right_pin, (joy_send_right != 0));
  }
};

#endif