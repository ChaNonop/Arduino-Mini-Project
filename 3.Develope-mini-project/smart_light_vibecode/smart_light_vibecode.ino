/*
 * Smart Lamp - Main Control File
 * ไฟล์หลักสำหรับควบคุมโปรเจคโคมไฟอัจฉริยะ
 * ทำหน้าที่สร้างอ็อบเจกต์และควบคุมการทำงานของส่วนต่างๆ
 * ผู้จัดทำ: Gemini
 * วันที่: 20 มิถุนายน 2025
 */

// Include ไลบรารีและคลาสที่จำเป็นทั้งหมด
#include <Arduino.h>
#include "ProjectPins.h"  
#include "SensorManager.h"
#include "DisplayManager.h"
#include "SoundManager.h"
#include "PowerManager.h"

// --- 1. สร้างอ็อบเจกต์จากคลาสต่างๆ ---
// เปรียบเสมือนการสร้างตัวตนของแต่ละส่วนประกอบขึ้นมาใช้งาน
SensorManager sensorManager;
DisplayManager displayManager;
SoundManager soundManager;
PowerManager powerManager;

// ตัวแปรสำหรับสถานะของไฟ LED 10W
bool isPowerLedOn = false;

// --- SETUP: ฟังก์ชันที่ทำงานครั้งเดียวเมื่อเปิดเครื่อง ---
void setup() {
  Serial.begin(115200);
  Serial.println("Smart Lamp is starting up...");

  // --- 2. เรียกใช้ฟังก์ชัน `begin()` ของแต่ละอ็อบเจกต์ ---
  // เพื่อตั้งค่าเริ่มต้นที่จำเป็นสำหรับแต่ละส่วน
  sensorManager.begin();
  displayManager.begin();
  soundManager.begin();
  powerManager.begin();

  // ตั้งค่าสถานะเริ่มต้นของไฟ LED 10W (ให้ดับอยู่)
  pinMode(PIN_POWER_LED, OUTPUT);
  digitalWrite(PIN_POWER_LED, LOW); // สมมติว่า Active HIGH (ต้องปรับตาม Relay Module)

  // ส่งเสียงเตือนว่าพร้อมทำงาน
  soundManager.playReadySound();
  Serial.println("System is ready.");
}

// --- LOOP: ฟังก์ชันที่ทำงานวนซ้ำไปเรื่อยๆ ---
void loop() {
  // --- 3. เรียกใช้ฟังก์ชัน `update()` ของแต่ละอ็อบเจกต์ ---
  // เพื่อให้แต่ละส่วนทำงานของตัวเองในแต่ละรอบของ loop
  sensorManager.update();
  soundManager.update();
  displayManager.updateDisplay(); // ฟังก์ชันนี้ต้องเรียกบ่อยๆ เพื่อให้จอไม่กระพริบ

  // --- 4. ตรรกะการควบคุมหลัก (Main Logic) ---

  // อ่านค่าจาก Sensor Manager
  bool motion = sensorManager.motionDetected();
  float lightLevel = sensorManager.getLightLevel();
  float temperature = sensorManager.getTemperature();

  // ตรวจสอบเงื่อนไขการเปิดไฟจาก PIR Sensor
  // เงื่อนไข: มีการเคลื่อนไหว และ แสงน้อย
  if (motion && lightLevel < LIGHT_THRESHOLD) {
    if (!isPowerLedOn) {
      Serial.println("PIR detected! Turning light ON.");
      isPowerLedOn = true;
      digitalWrite(PIN_POWER_LED, HIGH);
    }
  }

  // ตรวจสอบเงื่อนไขการเปิด/ปิดไฟจากการตบมือ
  if (soundManager.clapEventTriggered()) {
      isPowerLedOn = !isPowerLedOn; // สลับสถานะไฟ
      digitalWrite(PIN_POWER_LED, isPowerLedOn);
      Serial.print("Clap event! Turning light ");
      Serial.println(isPowerLedOn ? "ON" : "OFF");
  }

  // อัปเดตตัวเลขที่จะแสดงบนจอ 7-segment
  // เราจะแสดงค่าอุณหภูมิ (เฉพาะเลขจำนวนเต็ม)
  displayManager.setNumberToShow((int)temperature);

  // หมายเหตุ: ส่วนของ Sleep Mode ยังไม่ได้ใส่ใน loop นี้
  // เพราะต้องออกแบบลอจิกเพิ่มเติมว่าเมื่อไหร่จะให้หลับ
  // เช่น: powerManager.goToSleepIfIdle();
}
