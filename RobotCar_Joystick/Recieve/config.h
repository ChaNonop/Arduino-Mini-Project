#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

class control {
public:
  // D1, D2 มอเตอร์ซ้าย || D5, D6 สำหรับมอเตอร์ขวา
  const uint8_t Pin_IN[4] = { 13, 15, 14, 12 };  // GPIO 13(D7), 15(D8), 14(D5), 12(D6)

  const uint8_t Pin_EN[2] = { 5, 4 };  // GPIO 5(D1),GPIO 4(D2)
  const uint8_t Pin_ir = 2;            // D4

  int16_t joy_left = 0;   // Forward/Backward
  int16_t joy_right = 0;  // Left/Right (Turn)

  //int16_t speedL = 0;
  //int16_t speedR = 0;
  bool blocking = 1;
  uint16_t back = 100;

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
    pinMode(Pin_ir, INPUT_PULLUP);
  }

  void cal_speed() {
    blocking = digitalRead(Pin_ir);
    //int16_t speed = map(joy_left, -1023, 1023, -255, 255);
    //int16_t turn = map(joy_right, -1023, 1023, -255, 255);

    // คำนวณความเร็วล้อซ้าย-ขวา และจำกัดค่าให้อยู่ในช่วง -255 ถึง 255
    //speedL = constrain(speed - turn, -255, 255);
    //speedR = constrain(speed + turn, -255, 255);
  }

  void drive_motor() {
    if (blocking == HIGH) {
      if (joy_left > 0) {
        // ล้อซ้าย
        digitalWrite(Pin_IN[0], HIGH);
        digitalWrite(Pin_IN[1], LOW);
        analogWrite(Pin_EN[0], abs(joy_left));
      } else {
        digitalWrite(Pin_IN[0], LOW);
        digitalWrite(Pin_IN[1], HIGH);
        analogWrite(Pin_EN[0], abs(joy_left));
      }
      if (joy_right > 0) {
        // ล้อขวา
        digitalWrite(Pin_IN[2], HIGH);
        digitalWrite(Pin_IN[3], LOW);
        analogWrite(Pin_EN[1], abs(joy_right));
      } else {
        digitalWrite(Pin_IN[2], LOW);
        digitalWrite(Pin_IN[3], HIGH);
        analogWrite(Pin_EN[1], abs(joy_right));
      }
    } else {
      digitalWrite(Pin_IN[0], LOW);
      digitalWrite(Pin_IN[1], HIGH);
      analogWrite(Pin_EN[0], abs(back));
      digitalWrite(Pin_IN[2], LOW);
      digitalWrite(Pin_IN[3], HIGH);
      analogWrite(Pin_EN[1], abs(back));
    }
  }
};

#endif

/*
  void cal_speed() {
    blocking = digitalRead(Pin_ir);
    int16_t speed = map(joy_left, -1023, 1023, -255, 255);
    int16_t turn = map(joy_right, -1023, 1023, -255, 255);

    // คำนวณความเร็วล้อซ้าย-ขวา และจำกัดค่าให้อยู่ในช่วง -255 ถึง 255
    //speedL = constrain(speed - turn, -255, 255);
    //speedR = constrain(speed + turn, -255, 255);
  }

  void drive_motor() {
    if (blocking) {
      if (speedL > 0) {
        // ล้อซ้าย
        digitalWrite(Pin_IN[0], HIGH);
        digitalWrite(Pin_IN[1], LOW);
        analogWrite(Pin_EN[0], abs(speedL));
      } else {
        digitalWrite(Pin_IN[0], LOW);
        digitalWrite(Pin_IN[1], HIGH);
        analogWrite(Pin_EN[0], abs(speedL));
      }
      if (speedR > 0) {
        // ล้อขวา
        digitalWrite(Pin_IN[2], HIGH);
        digitalWrite(Pin_IN[3], LOW);
        analogWrite(Pin_EN[1], abs(speedR));
      } else {
        digitalWrite(Pin_IN[2], LOW);
        digitalWrite(Pin_IN[3], HIGH);
        analogWrite(Pin_EN[1], abs(speedR));
      }
      else {
        digitalWrite(Pin_IN[0], LOW);
        digitalWrite(Pin_IN[1], HIGH);
        analogWrite(Pin_EN[0], abs(Back));
        digitalWrite(Pin_IN[2], LOW);
        digitalWrite(Pin_IN[3], HIGH);
        analogWrite(Pin_EN[1], abs(Back));
      }
    }
  }
};*/