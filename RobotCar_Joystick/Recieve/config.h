// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

class control {
public:
  // ขาควบคุมมอเตอร์
  const uint8_t Pin_IN[4] = {15, 13, 12, 14};  // IN1, IN2, IN3, IN4
  const uint8_t Pin_EN[2] = {5, 4};            // ENA, ENB
  
  // ขาเซ็นเซอร์ Ultrasonic
  const uint8_t Pin_trig = 1; // ต่อกับขา TX (GPIO1)
  const uint8_t Pin_echo = 3; // ต่อกับขา RX (GPIO3)

  // ตัวแปรควบคุม
  int16_t joy_left = 0;    // ค่าจอยสติ๊กซ้าย (-1023 ถึง 1023)
  int16_t joy_right = 0;   // ค่าจอยสติ๊กขวา (-1023 ถึง 1023)
  
  bool obstacle_detected = false;
  const uint16_t back_speed = 800;
  unsigned long backup_start_time = 0;
  const unsigned long BACKUP_DURATION = 500;
  unsigned long last_data_time = 0;
  const unsigned long DATA_TIMEOUT = 1000;

  long duration;
  int distance;
  const int distance_threshold = 15; // ระยะที่จะให้เริ่มถอย (หน่วยเป็น cm)

  control() {}

  void init_pins() {
    for (uint8_t i = 0; i < 4; i++) {
      pinMode(Pin_IN[i], OUTPUT);
      digitalWrite(Pin_IN[i], LOW);
    }
    for (uint8_t i = 0; i < 2; i++) {
      pinMode(Pin_EN[i], OUTPUT);
      digitalWrite(Pin_EN[i], LOW);
    }
    pinMode(Pin_trig, OUTPUT);
    pinMode(Pin_echo, INPUT);
  }
  void check_distance() {
    // ส่งสัญญาณ Trigger
    digitalWrite(Pin_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Pin_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Pin_trig, LOW);
    
    // อ่านค่า Echo และคำนวณระยะทาง
    duration = pulseIn(Pin_echo, HIGH);
    distance = duration * 0.034 / 2; // คำนวณระยะทางเป็นเซนติเมตร
  }
  void check_obstacle() {
    check_distance(); 

    // ตรวจสอบว่าเจอสของขวางมัย
    if (distance < distance_threshold && distance > 0 && !obstacle_detected) {
      obstacle_detected = true;
      backup_start_time = millis();
    }
    
    // ตรวจสอบว่าหมดเวลาถอยหลังหรือยัง
    if (obstacle_detected && (millis() - backup_start_time >= BACKUP_DURATION)) {
      obstacle_detected = false;
    }
  }

  void check_timeout() {
    if (millis() - last_data_time > DATA_TIMEOUT) {
      joy_left = 0;
      joy_right = 0;
    }
  }

  void drive_motor() {
    check_obstacle();
    check_timeout();

    if (obstacle_detected) {
      // โหมดถอยหลังเมื่อเจอของ
      digitalWrite(Pin_IN[0], LOW);
      digitalWrite(Pin_IN[1], HIGH);
      analogWrite(Pin_EN[0], back_speed);
      
      digitalWrite(Pin_IN[2], LOW);
      digitalWrite(Pin_IN[3], HIGH);
      analogWrite(Pin_EN[1], back_speed);
    } 
    else {
      // ล้อซ้าย
      if (joy_left > 10) {
        digitalWrite(Pin_IN[0], HIGH);
        digitalWrite(Pin_IN[1], LOW);
        analogWrite(Pin_EN[0], abs(joy_left));
      } 
      else if (joy_left < -10) {
        digitalWrite(Pin_IN[0], LOW);
        digitalWrite(Pin_IN[1], HIGH);
        analogWrite(Pin_EN[0], abs(joy_left));
      } 
      else {
        digitalWrite(Pin_IN[0], LOW);
        digitalWrite(Pin_IN[1], LOW);
        analogWrite(Pin_EN[0], 0);
      }
      
      // ล้อขวา
      if (joy_right > 10) {
        digitalWrite(Pin_IN[2], HIGH);
        digitalWrite(Pin_IN[3], LOW);
        analogWrite(Pin_EN[1], abs(joy_right));
      } 
      else if (joy_right < -10) {
        digitalWrite(Pin_IN[2], LOW);
        digitalWrite(Pin_IN[3], HIGH);
        analogWrite(Pin_EN[1], abs(joy_right));
      } 
      else {
        digitalWrite(Pin_IN[2], LOW);
        digitalWrite(Pin_IN[3], LOW);
        analogWrite(Pin_EN[1], 0);
      }
    }
  }
};

#endif
