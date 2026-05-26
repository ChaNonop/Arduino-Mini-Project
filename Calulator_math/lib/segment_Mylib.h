/*
  ตัวอย่างการเรียกใช้านไลบรารี segment_Mylib.h
  สร้างโดย Chanon
  วันที่ 8 พ.ย. 2568
*/

#ifndef SEGMENT_MYLIB_H
#define SEGMENT_MYLIB_H

#include <Arduino.h>
#include <avr/pgmspace.h>
  // กำหนดรูปแบบการแสดงผลตัวเลข 0-9 บนจอ 7-segment แบบ Common Anode ทำ bitmap แบบ Binary
namespace SEGMENT_MYLIB {
  const byte Num_Anode_binary[10] PROGMEM = {
    B00000011, // เลข 0
    B10011111, // เลข 1
    B00100101, // เลข 2
    B00001101, // เลข 3
    B10011001, // เลข 4
    B01001001, // เลข 5
    B01000001, // เลข 6
    B00011111, // เลข 7
    B00000001, // เลข 8
    B00001001 // เลข 9
  };
  // กำหนดรูปแบบการแสดงผลตัวเลข 0-9 บนจอ 7-segment แบบ Common Cathode ทำ bitmap แบบ Binary
  const byte Num_cathode_binary[10] PROGMEM = {
    B11111100, // เลข 0
    B01100000, // เลข 1
    B11011010, // เลข 2
    B11110010, // เลข 3
    B01100110, // เลข 4
    B10110110, // เลข 5
    B10111110, // เลข 6
    B11100000, // เลข 7
    B11111110, // เลข 8
    B11110110
  // เลข 9
  };

  // กำหนดรูปแบบการแสดงผลตัวเลข 0-9 บนจอ 7-segment แบบ Common Anode ทำ bitmap แบบ Hexadecimal
  const byte Num_Anode_Hex[10] PROGMEM = {
    0x03, // เลข 0
    0x9F, // เลข 1
    0x25, // เลข 2
    0x0D, // เลข 3
    0x99, // เลข 4
    0x49, // เลข 5
    0x41, // เลข 6
    0x1F, // เลข 7
    0x01, // เลข 8
    0x09 // เลข 9
  };
  // กำหนดรูปแบบการแสดงผลตัวเลข 0-9 บนจอ 7-segment แบบ Common Cathode ทำ bitmap แบบ Hexadecimal
  const byte Num_cathode_Hex[10] PROGMEM = {
    0xFC,  // เลข 0
    0x60,  // เลข 1
    0xDA,  // เลข 2
    0xF2,  // เลข 3
    0x66,  // เลข 4
    0xB6,  // เลข 5
    0xBE,  // เลข 6
    0xE0,  // เลข 7
    0xFE,  // เลข 8
    0xF6   // เลข 9
  };
} // namespace SEGMENT
#endif
/*
  วิธีใช้งานโค้ดตัวอย่างนี้:
  1. สร้างไฟล์ segment_Mylib.h และคัดลอกโค้ดด้านบนลงไป

#include <Arduino.h>
#include "segment_mylib.h"  // include ไลบรารีที่เราสร้าง

using namespace SEGMENT_MYLIB;  // เพื่อเรียกใช้ array โดยไม่ต้องพิมพ์ SEGMENT::

const byte segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};  
// ขาแต่ละขาของ 7-segment (a–g, DP)

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void displayDigit(byte num) {
  // อ่านข้อมูลจาก PROGMEM
  byte pattern = pgm_read_byte(&Anode_Bin[num]);

  // เขียนค่าบิตแต่ละบิตลงขา output
  for (byte i = 0; i < 8; i++) {
    bool state = bitRead(pattern, 7 - i); // เรียงบิตจากซ้ายไปขวา
    digitalWrite(segPins[i], state);
  }
}

void loop() {
  for (byte i = 0; i < 10; i++) {
    displayDigit(i);
    delay(1000);
  }
}

  2. ในไฟล์หลักของโปรเจค Arduino ให้ include ไลบรารีนี้ด้วยคำสั่ง #include "segment_Mylib.h"
  3. ใช้ namespace SEGMENT เพื่อเข้าถึง array ต่างๆ ได้ง่ายขึ้น
*/
