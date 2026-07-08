#ifndef NTP_TIME_H
#define NTP_TIME_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Text.h"

const char ntpServer[] PROGMEM = "th.pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;  // GMT+7 สำหรับประเทศไทย
const int daylightOffset_sec = 0;      // ไม่ใช้ Daylight Saving Time

char timeBuffer[40];
time_t now;
struct tm *timeinfo;

void setupNTP() {
  char ntpBuffer[64];
  strcpy_P(ntpBuffer, ntpServer);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpBuffer);
  
  printFromProgmem(msgGettingTime);
  
  // รอการเชื่อมต่อเวลา
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {  // ตรวจสอบว่าเวลาได้ถูกตั้งค่าหรือยัง
    delay(500);
    printFromProgmem(msgWaitNTP);
    now = time(nullptr);
  }

  printFromProgmem(msgTimeOK);
}

void printCurrentTime() {
  time(&now);                   // ดึงเวลาปัจจุบัน
  timeinfo = localtime(&now);   // แปลงเป็นโครงสร้างเวลา

  snprintf(timeBuffer, sizeof(timeBuffer), "%02d/%02d/%04d %02d:%02d:%02d",
           timeinfo->tm_mday,
           timeinfo->tm_mon + 1,
           timeinfo->tm_year + 1900,
           timeinfo->tm_hour,
           timeinfo->tm_min,
           timeinfo->tm_sec);
  printFromProgmem(msgCurrentTime);
  Serial.println(timeBuffer);
}

#endif