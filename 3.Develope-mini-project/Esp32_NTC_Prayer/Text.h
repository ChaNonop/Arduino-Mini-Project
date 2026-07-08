#ifndef TEXT_H
#define TEXT_H

#include <Arduino.h>
#include <pgmspace.h>

const char msgConnectingWiFi[] PROGMEM = "กำลังเชื่อมต่อ WiFi";
const char msgConnected[] PROGMEM = "เชื่อมต่อสำเร็จ!";
const char msgGettingTime[] PROGMEM = "กำลังดึงเวลาจาก NTP...";
const char msgWaitNTP[] PROGMEM = "รอเวลา NTP...";
const char msgTimeOK[] PROGMEM = "ดึงเวลาจาก NTP สำเร็จ!";
const char msgCurrentTime[] PROGMEM = "เวลาปัจจุบัน (จาก RTC): ";

void printFromProgmem(const char* progmemStr) {
  char buffer[64];
  strcpy_P(buffer, progmemStr);
  Serial.println(buffer);
}

#endif
