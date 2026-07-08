#include "PowerManager.h"

// Initialize static member variables
volatile bool PowerManager::isAwake = true;

PowerManager::PowerManager() {}

void PowerManager::begin() {
  // ตั้งค่า Interrupt ที่ขา PIR (D3) และ Sound (D2)
  // ให้ทำงานเมื่อมีสัญญาณเปลี่ยนจาก LOW ไป HIGH (RISING)
  // เมื่อมี Interrupt จะเรียกฟังก์ชัน wakeUp()
  // หมายเหตุ: ขา Interrupt อาจแตกต่างกันไปในแต่ละบอร์ด
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), wakeUp, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_SOUND), wakeUp, RISING);
}

// ฟังก์ชันที่จะถูกเรียกเมื่อมี Interrupt เกิดขึ้น
void PowerManager::wakeUp() {
  isAwake = true;
}

void PowerManager::goToSleep() {
  Serial.println("Going to sleep...");
  delay(100); // รอให้ Serial ส่งข้อมูลเสร็จ

  isAwake = false;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  
  // เข้าสู่โหมด Sleep และจะหยุดทำงานที่นี่จนกว่าจะมี Interrupt
  sleep_mode(); 
  
  // --- CPU resumes execution here after wake-up ---
  sleep_disable();
  Serial.println("...Woke up!");
}
