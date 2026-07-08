#include "SensorManager.h"

// Constructor: ตั้งค่าเริ่มต้นให้กับ Object
SensorManager::SensorManager() : dht_sensor(PIN_DHT, DHT_SENSOR_TYPE) {
  currentMotionState = false;
  currentTemperature = 0.0;
  currentHumidity = 0.0;
  currentLightLevel = 0.0;
  lastDhtRead = 0;
  lastLdrRead = 0;
}

// begin(): ตั้งค่า Pin และเริ่มต้นการทำงานของเซ็นเซอร์
void SensorManager::begin() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LDR, INPUT);
}

// update(): ฟังก์ชันหลักที่ถูกเรียกใน loop เพื่ออัปเดตค่าจากเซ็นเซอร์
void SensorManager::update() {
  // อ่านค่า PIR ทุกรอบ
  readPir();

  // อ่านค่า LDR ทุกๆ 100ms เพื่อไม่ให้หน่วงเกินไป
  if (millis() - lastLdrRead > 100) {
    lastLdrRead = millis();
    readLdr();
  }

  // อ่านค่า DHT ทุกๆ 4 วินาที (ตามข้อกำหนดของไลบรารี)
  if (millis() - lastDhtRead > 4000) {
    lastDhtRead = millis();
    readDht();
  }
}

//--- Private Helper Functions ---

void SensorManager::readPir() {
  currentMotionState = digitalRead(PIN_PIR);
}

void SensorManager::readLdr() {
  currentLightLevel = analogRead(PIN_LDR);
  // หมายเหตุ: สามารถเพิ่มการคำนวณเป็น Lux ได้ถ้าต้องการ
}

bool SensorManager::readDht() {
  // ใช้ dht_sensor.measure() จากไลบรารี DHT_Async
  if (dht_sensor.measure(&currentTemperature, &currentHumidity)) {
    Serial.print("DHT Read OK -> T: ");
    Serial.print(currentTemperature);
    Serial.print(" H: ");
    Serial.println(currentHumidity);
    return true;
  }
  return false;
}


//--- Public Getter Functions ---

bool SensorManager::motionDetected() {
  return currentMotionState;
}

float SensorManager::getTemperature() {
  return currentTemperature;
}

float SensorManager::getHumidity() {
  return currentHumidity;
}

float SensorManager::getLightLevel() {
  return currentLightLevel;
}
