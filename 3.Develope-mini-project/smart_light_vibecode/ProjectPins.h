#ifndef PROJECT_PINS_H
#define PROJECT_PINS_H

#include <Arduino.h>

// --- Pin Definitions for Sensors ---
// กำหนด Pin สำหรับเซ็นเซอร์ต่างๆ (สำหรับ Arduino Nano)
const byte PIN_PIR = 3;         // PIR motion sensor -> Digital pin 3
const byte PIN_LDR = A1;        // LDR light sensor -> Analog pin A1
const byte PIN_DHT = 4;         // DHT11 temperature and humidity sensor -> Digital pin 4
const byte PIN_SOUND = 2;       // Sound sensor (LM393) -> Digital pin 2

// --- Pin Definitions for Actuators ---
// กำหนด Pin สำหรับตัวกระทำการ
const byte PIN_POWER_LED = 1;     // Pin for 10W LED (connected to a relay) -> Digital pin 1 (หรือ pin อื่น)
const byte PIN_BUZZER = A0;      // Buzzer -> Analog pin A0 (สามารถใช้เป็น Digital ได้)

// --- Pin Definitions for 7-Segment Display (Shift Register 74HC595) ---
// กำหนด Pin สำหรับจอ 7-Segment ผ่าน Shift Register
const byte PIN_SHIFT_DATA = 5;    // DS (pin 14) -> Digital pin 5
const byte PIN_SHIFT_CLOCK = 6;   // SHCP (pin 11) -> Digital pin 6
const byte PIN_SHIFT_LATCH = 7;   // STCP (pin 12) -> Digital pin 7

// --- Pin Definitions for Digit Selection (Common Cathode) ---
// กำหนด Pin สำหรับเลือกหลักของจอ (ถ้ามี 4 หลัก)
const byte DIGIT_PINS[] = {A2, A3, A4, A5}; // ใช้ Analog pin เป็น Digital ได้
const byte NUM_DIGITS = 4;

// --- Constants ---
// ค่าคงที่ต่างๆ ที่ใช้ในโปรเจค
#define DHT_SENSOR_TYPE DHT_TYPE_11
const int LIGHT_THRESHOLD = 300; // ค่าแสงน้อยกว่านี้ถึงจะทำงาน (ปรับค่าตามสภาพแสงจริง)

#endif
