#include <Arduino.h>
#include <avr/pgmspace.h>
#include "segment_mylib.h"

using namespace SEGMENT_MYLIB;

// --- Pin Definitions ---
const byte latchPin = 10;
const byte clockPin = 11;
const byte dataPin = 12;
const byte button_Pin = 2;      // ปุ่มนับ (Interrupt)
const byte selectButton_Pin = 4; // ปุ่มเลือกหลัก (Polling)

// Pin 4-Digit Common Pins
const byte digit_Pin[4] = { 3, 5, 6, 9 };

// --- Global Variables ---
int count = 0;
int lastCount = -1;

// Variables for Interrupt (Count Button)
volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const long debounceDelay = 150;

// Variables for Select Button
int selectedDigit = 0;          // 0=หลักหน่วย, 1=สิบ, 2=ร้อย, 3=พัน
bool lastSelectBtnState = HIGH;
unsigned long lastSelectDebounce = 0;

// Variables for Blinking Logic
bool isDigitVisible = true;     // สถานะมองเห็นของหลักที่กระพริบ
unsigned long lastBlinkTime = 0;
const long blinkInterval = 300; // ความเร็วกระพริบ (ms)

// LED Multiplexing Control
unsigned long lasttime_led = 0;
const long led_Delay = 4;       // ลดเวลาลงเพื่อ Refresh Rate ที่ดีขึ้น (~60Hz)
byte currentDigit = 0;          

void setup() {
  Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (byte i = 0; i < 4; i++) {
    pinMode(digit_Pin[i], OUTPUT);
    turnoff_digit();
  }
  
  pinMode(button_Pin, INPUT_PULLUP);
  pinMode(selectButton_Pin, INPUT_PULLUP); // ตั้งค่าขาปุ่มใหม่

  attachInterrupt(digitalPinToInterrupt(button_Pin), checkSwitch, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  // --- 1. Handle Count Button (Debounce logic inside loop) ---
  if (buttonPressed) {
    if (currentMillis - lastDebounceTime > debounceDelay) {
      count++;
      if (count > 9999) count = 0;
      lastDebounceTime = currentMillis;
      buttonPressed = false;
    }
  }

  // --- 2. Handle Select Button (Polling) ---
  bool reading = digitalRead(selectButton_Pin);
  if (reading != lastSelectBtnState) {
    lastSelectDebounce = currentMillis;
  }
  
  if ((currentMillis - lastSelectDebounce) > 50) { // Software Debounce 50ms
    // เช็คจังหวะกดลง (Falling Edge for INPUT_PULLUP)
    if (reading == LOW && lastSelectBtnState == HIGH) { // Note: Logic might depend on prev state var
       // *Correction logic for simple edge detection*
    }
  }
  
  // Simple Edge Detection for Select Button
  // (Logic แบบกระชับที่มักใช้ในงาน Industrial MCU)
  static bool btnState = HIGH;
  static unsigned long lastBtnTime = 0;
  if (currentMillis - lastBtnTime > 100) { // Debounce Check
      bool r = digitalRead(selectButton_Pin);
      if (r == LOW && btnState == HIGH) { // Falling Edge
          selectedDigit++;
          if (selectedDigit > 3) selectedDigit = 0; // วนกลับที่หลักหน่วย
          Serial.print("Selected Digit Index: ");
          Serial.println(selectedDigit);
      }
      btnState = r;
      if(r == LOW) lastBtnTime = currentMillis;
  }


  // --- 3. Blink Timer Logic ---
  if (currentMillis - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = currentMillis;
    isDigitVisible = !isDigitVisible; // สลับสถานะ ติด/ดับ
  }

  // --- 4. Display & Monitor ---
  Display_num(count);

  if (count != lastCount) {
    Serial.print("Count: ");
    Serial.println(count);
    lastCount = count;
  }
}

// Interrupt Service Routine
void checkSwitch() {
  buttonPressed = true;
}

// Display Function (Modified)
void Display_num(int count_num) {
  static int Digit_data[4];
  // แยกตัวเลขเตรียมไว้ (Parsing)
  Digit_data[0] = count_num % 10;
  Digit_data[1] = (count_num / 10) % 10;
  Digit_data[2] = (count_num / 100) % 10;
  Digit_data[3] = (count_num / 1000) % 10;

  unsigned long msg = millis();
  
  // Multiplexing Loop
  if (msg - lasttime_led >= led_Delay) {
    turnoff_digit(); // ดับก่อนเปลี่ยนหลักเพื่อกันเงา (Ghosting)

    // เงื่อนไขการแสดงผล:
    // 1. ถ้าไม่ใช่หลักที่เลือก -> แสดงผลปกติ
    // 2. ถ้าเป็นหลักที่เลือก -> เช็ค isDigitVisible (ถ้า true แสดง, false ดับ)
    if (currentDigit != selectedDigit || isDigitVisible) {
        byte num = Digit_data[currentDigit];
        // ตรวจสอบว่ามี array Num_cathode_Hex ใน library จริงหรือไม่ 
        // (สมมติว่า library ถูกต้องตาม code เดิม)
        byte dataSend = pgm_read_byte(&Num_cathode_Hex[num]);

        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, dataSend);
        digitalWrite(latchPin, HIGH);
        
        digitalWrite(digit_Pin[currentDigit], HIGH); // Common ON
    }

    lasttime_led = msg;
    currentDigit = (currentDigit + 1) % 4;
  }
}

void turnoff_digit() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(digit_Pin[i], LOW);
  }
}