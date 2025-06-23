// Mylib >> MacAddres esp
#include <MyEsp_address.h>

#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

class Control {
private:
  const uint8_t Vx_Pin = A0;
  //const uint8_t Vy_Pin = A2;

  // left right button
  const byte Pinbutton[2] = { D0, D1 };

  // sampling value
  const uint8_t sampling = 10;

  // cal analog
  uint16_t sumVx = 0;
  uint16_t sumVy = 0;

  int avgVx = 0;
  int avgVy = 0;
  const uint16_t calib = 2048;
  unsigned long lastReadTime = 0;  // บันทึกเวลาการอ่านค่าล่าสุด ควบคุมการอ่านค่าเป็นระยะ

public:
  const uint8_t led_Vx_pin;  // Pin สำหรับ LED แสดงสถานะแกน Vx
  //const uint8_t led_Vy_pin;
  const uint8_t led_button_pin;  // Pin สำหรับ LED แสดงสถานะปุ่ม
  const uint8_t led_espNow_Pin;  // Pin สำหรับ LED แสดงสถานะ ESP-NOW

  int joy_send_Vx;
  //int joy_send_Vy;
  bool joy_send_button[2];

  Control()
    : led_Vx_pin(D4),
      led_button_pin(D5),  // D5 (GPIO14)
      led_espNow_Pin(10),  // GPIO10 (SD3)
      joy_send_Vx(0),
      //joy_send_Vy(0) {
      joy_send_button[0] = false;
  joy_send_button[1] = false;

  pinMode(Pinbutton[0], INPUT_PULLUP);
  pinMode(Pinbutton[1], INPUT_PULLUP);
}

void
read_adc() {
  // อ่านค่าทุกๆ 10 ms
  if (millis() - lastReadTime >= 10) {
    lastReadTime = millis();  // อัปเดตเวลาการอ่านค่าล่าสุด

    joy_send_button[0] = digitalRead(Pinbutton[0]);
    joy_send_button[1] = digitalRead(Pinbutton[1]);

    sumVx = 0;
    for (uint8_t i = 0; i < sampling; i++) {
      sumVx += analogRead(Vx_Pin);
      //sumVy += analogRead(Vy_Pin);
      delayMicroseconds(100);  //หน่วงเวลาการอ่านหน่อย 1ms
    }

    avgVx = (sumVx / sampling) - calib;
    avgVy = (sumVy / sampling) - calib;


    // ถ้าค่าอยู่ใกล้จุดศูนย์กลาง 512 มาก ให้ถือว่าเป็น 512 กันค่าคลาดเคลื่อน
    if (abs(avgVx - calib) <= 5) {
    }
    if (abs(avgVy - calib) <= 5) {}

    // Map ค่า 0-4096 เป็น -512 ถึง 512
    joy_send_Vx = map(avgVx, -2046, 2046, -512, 512);
    //joy_send_Vy = map(avgVy, -2046, 2046, -512, 512);
    led();
    yield();
  }
}

void led() {
  if (joy_send_Vx > 0 + 5 || joy < 0 - 5) {  // ถ้า Joystick ขยับหรือ่านค่าได้ ติดไฟ led
    digitalWrite(led_Vx_pin, HIGH);
  } else {
    digitalWrite(led_Vx_pin, LOW);
  }
  /*
    if (joy_send_Vy > 0 + 5 || joy_send_Vy < 0 - 5) {  // ถ้า Joystick ขยับหรือ่านค่าได้ ติดไฟ led
    digitalWrite(led_Vy_pin, HIGH);
  } else {
    digitalWrite(led_Vy_pin, LOW);
  }
  */

  // joy_send_button ถูกกลับค่า (true = กด)
  if (joy_send_button[0] || joy_send_button[1] == 0) {
    digitalWrite(led_button_pin, HIGH);
  } else {
    digitalWrite(led_button_pin, LOW);
  }
}
}
;

#endif