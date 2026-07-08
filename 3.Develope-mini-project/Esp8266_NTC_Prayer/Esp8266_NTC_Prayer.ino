#include "wifi_setup.h"
#include "ntp_time.h"
#include "prayer_time.h"

void setup() {
  Serial.begin(115200);
  delay(1000);  // รอให้ Serial พร้อม
  
  connectToWiFi();    // เชื่อมต่อ WiFi
  setupNTP();         // ตั้งค่าเวลา NTP
}

void loop() {
  printCurrentTime();  // แสดงเวลาปัจจุบัน
  getPrayerTimes();    // ดึงและแสดงเวลาละหมาด
  delay(60000);        // ตรวจสอบทุก 1 นาที
}