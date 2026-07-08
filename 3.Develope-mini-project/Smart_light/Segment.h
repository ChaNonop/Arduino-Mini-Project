#ifndef SEGMENT_H
#define SEGMENT_H

#include <Arduino.h>
#include "Variable_Pin.h"
class led : Obj {
private:
  // กำหนดขาต่อกับ Shift Register
const int dataPin = D5;   // DS (pin 14) ของ 74HC595
const int clockPin = D6;  // SHCP (pin 11) ของ 74HC595
const int latchPin = D7;   // STCP (pin 12) ของ 74HC595

// กำหนดขาเลือก Digit (Common Cathode)
  const byte Digit_4[4] PROGMEM= [A2,A3,A4,A5]; // ขาสำหรับเลือก Digit 1-4

// ตารางรหัสสำหรับเลข 0-9 (Common Cathode)
const byte digitPatterns[10] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

// ตารางเลือก Digit
const byte digitSelect[4] = {
  B11101111, // Digit 1
  B11011111, // Digit 2
  B10111111, // Digit 3
  B01111111  // Digit 4
};
}
public:


// ฟังก์ชันสำหรับแสดงตัวเลข 4 หลัก
void displayNumber(int num) {
  int digits[4];
  
  // แยกตัวเลขเป็นหลักๆ
  digits[0] = num / 1000;          // หลักพัน
  digits[1] = (num / 100) % 10;    // หลักร้อย
  digits[2] = (num / 10) % 10;     // หลักสิบ
  digits[3] = num % 10;            // หลักหน่วย
  
  // สแกนแต่ละ Digit
  for (int i = 0; i < 4; i++) {
    // ปิดทุก Digit ก่อน
    for (int j = 0; j < 4; j++) {
      digitalWrite(digitPins[j], HIGH);
    }
    
    // เปิด Digit ปัจจุบัน
    digitalWrite(digitPins[i], LOW);
    
    // ส่งข้อมูลไปยัง Shift Register
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, digitPatterns[digits[i]]);
    digitalWrite(latchPin, HIGH);
    
    // หน่วงเวลาเล็กน้อยเพื่อให้มองเห็น
    delay(2);
  }

}
#endif