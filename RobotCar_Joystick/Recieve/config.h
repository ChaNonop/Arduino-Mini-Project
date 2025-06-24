#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

class control {
public:
  // D1, D2 มอเตอร์ซ้าย || D5, D6 สำหรับมอเตอร์ขวา
  const uint8_t Pin_IN[4]  = { 5, 4, 14, 12 }; // GPIO 5(D1), 4(D2), 14(D5), 12(D6)
  
  // Pin Enable สำหรับ PWM, D3 และ D7
  const uint8_t Pin_EN[2]  = { 0, 13 }; // GPIO 0(D3), 13(D7)

  const uint8_t Pin_ir = 15; // D8

  int16_t joy_Vx = 0;
  int16_t joy_Vy = 0;

  int16_t speedL = 0;
  int16_t speedR = 0;

  control() {}

  void init_motor_pins() {
    for (uint8_t i = 0; i < 4; i++) {
      pinMode(Pin_IN[i], OUTPUT);
      digitalWrite(Pin_IN[i], LOW);
    }
    for (uint8_t i = 0; i < 2; i++) {
      pinMode(Pin_EN[i], OUTPUT);
      digitalWrite(Pin_EN[i], LOW);
    }
  }

  void calib() {
    int16_t speed = map(joy_Vx, -1023, 1023, 0, 1023);
    int16_t turn  = map(joy_Vy, -1023, 1023, -255, 255);

    // คำนวณความเร็วล้อซ้าย-ขวา และจำกัดค่าให้อยู่ในช่วง -255 ถึง 255
    speedL = constrain(speed - turn, -255, 255);
    speedR = constrain(speed + turn, -255, 255);
  }

  void drive_motor(int left, int right) {

    // ล้อซ้าย
    digitalWrite(Pin_IN[0], left > 0);
    digitalWrite(Pin_IN[1], left < 0);
    analogWrite(Pin_EN[0], abs(left));

    // ล้อขวา
    digitalWrite(Pin_IN[2], right > 0);
    digitalWrite(Pin_IN[3], right < 0);
    analogWrite(Pin_EN[1], abs(right));
  }
};

#endif