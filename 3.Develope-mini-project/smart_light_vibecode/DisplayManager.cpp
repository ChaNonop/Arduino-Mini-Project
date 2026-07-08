#include "DisplayManager.h"

DisplayManager::DisplayManager() {
  numberToDisplay = 0;
  currentDigitIndex = 0;
  lastDigitUpdateTime = 0;
  for (byte i = 0; i < NUM_DIGITS; i++) {
    digits[i] = 0;
  }
}

void DisplayManager::begin() {
  // ตั้งค่าขา Shift Register
  pinMode(PIN_SHIFT_LATCH, OUTPUT);
  pinMode(PIN_SHIFT_CLOCK, OUTPUT);
  pinMode(PIN_SHIFT_DATA, OUTPUT);

  // ตั้งค่าขาเลือกหลัก
  for (byte i = 0; i < NUM_DIGITS; i++) {
    pinMode(DIGIT_PINS[i], OUTPUT);
    digitalWrite(DIGIT_PINS[i], HIGH); // ปิดทุกหลัก (Common Cathode)
  }
}

void DisplayManager::setNumberToShow(int number) {
  if (number < 0) number = 0;
  if (number > 9999) number = 9999; // รองรับได้ 4 หลัก
  numberToDisplay = number;
  breakNumberIntoDigits();
}

void DisplayManager::breakNumberIntoDigits() {
  // แยกตัวเลขเป็นหลักๆ สำหรับจอ 4 หลัก
  int num = numberToDisplay;
  digits[0] = num / 1000;
  digits[1] = (num / 100) % 10;
  digits[2] = (num / 10) % 10;
  digits[3] = num % 10;
}

// updateDisplay(): หัวใจของการแสดงผลแบบ multiplexing
void DisplayManager::updateDisplay() {
  // หน่วงเวลาเล็กน้อยระหว่างการสลับหลักเพื่อให้จอไม่กระพริบ
  if (millis() - lastDigitUpdateTime < 2) {
    return;
  }
  lastDigitUpdateTime = millis();

  // 1. ปิดหลักปัจจุบันก่อนเปลี่ยน
  digitalWrite(DIGIT_PINS[currentDigitIndex], HIGH);

  // 2. เลื่อนไปยังหลักถัดไป
  currentDigitIndex++;
  if (currentDigitIndex >= NUM_DIGITS) {
    currentDigitIndex = 0;
  }

  // 3. ส่งข้อมูล (Pattern) ของตัวเลขสำหรับหลักใหม่ไปยัง Shift Register
  // เราจะแสดงเฉพาะ 2 หลักขวาสำหรับอุณหภูมิ (หลักที่ 2 และ 3)
  // แต่โค้ดนี้รองรับการแสดงผลทั้ง 4 หลัก
  byte patternToSend = B00000000; // ค่าเริ่มต้นคือดับทั้งหมด
  if(numberToDisplay < 100 && currentDigitIndex >= 2){
      // แสดงอุณหภูมิ 2 หลัก (เช่น 28) ที่ 2 หลักขวา
      patternToSend = digitPatterns[digits[currentDigitIndex]];
  } else if (numberToDisplay >= 100) {
      // ถ้าตัวเลขมากกว่า 100 ก็แสดงตามปกติ
      patternToSend = digitPatterns[digits[currentDigitIndex]];
  }

  digitalWrite(PIN_SHIFT_LATCH, LOW);
  shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, MSBFIRST, patternToSend);
  digitalWrite(PIN_SHIFT_LATCH, HIGH);

  // 4. เปิดไฟที่หลักใหม่
  // จะเปิดไฟเฉพาะเมื่อมีตัวเลขให้แสดง (ไม่แสดงเลข 0 นำหน้า)
   if (numberToDisplay >= 1000 && currentDigitIndex == 0) {
        digitalWrite(DIGIT_PINS[currentDigitIndex], LOW);
    } else if (numberToDisplay >= 100 && currentDigitIndex == 1) {
        digitalWrite(DIGIT_PINS[currentDigitIndex], LOW);
    } else if (numberToDisplay >= 10 && currentDigitIndex == 2) {
        digitalWrite(DIGIT_PINS[currentDigitIndex], LOW);
    } else if (currentDigitIndex == 3) { // หลักหน่วยแสดงเสมอ
        digitalWrite(DIGIT_PINS[currentDigitIndex], LOW);
    }
}
