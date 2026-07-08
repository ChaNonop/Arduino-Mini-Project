#include "SoundManager.h"

SoundManager::SoundManager() {
  clapCount = 0;
  lastClapTime = 0;
  lastSoundState = LOW;
  clapEvent = false;
}

void SoundManager::begin() {
  pinMode(PIN_SOUND, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void SoundManager::update() {
  // อ่านสถานะปัจจุบันของเซ็นเซอร์เสียง
  bool currentSoundState = digitalRead(PIN_SOUND);

  // ตรวจจับขอบขาขึ้น (Rising edge) ของสัญญาณเสียง
  if (currentSoundState == HIGH && lastSoundState == LOW) {
    unsigned long now = millis();
    
    // ตรวจสอบว่าเสียงที่เกิดขึ้นอยู่ในช่วงเวลาของการตบมือหรือไม่ (ไม่ห่างไป ไม่ใกล้ไป)
    if (now - lastClapTime < 1000 && now - lastClapTime > 200) {
      clapCount++;
    } else {
      // ถ้าเสียงห่างไป ถือเป็นการตบครั้งที่ 1 ใหม่
      clapCount = 1;
    }
    lastClapTime = now;
    Serial.print("Clap detected! Count: ");
    Serial.println(clapCount);
  }

  lastSoundState = currentSoundState;

  // ถ้าตบครบ 2 ครั้ง ให้ตั้งค่า flag และรีเซ็ตตัวนับ
  if (clapCount >= 2) {
    clapEvent = true; // ตั้งค่า flag เพื่อให้ loop หลักนำไปใช้
    clapCount = 0;
  }
}

// ฟังก์ชันนี้จะถูกเรียกโดย loop หลัก
bool SoundManager::clapEventTriggered() {
  if (clapEvent) {
    clapEvent = false; // เคลียร์ flag หลังจากถูกอ่านไปแล้ว
    return true;
  }
  return false;
}

void SoundManager::playReadySound() {
  tone(PIN_BUZZER, 1000, 100); // ส่งเสียง 1000Hz นาน 100ms
}
