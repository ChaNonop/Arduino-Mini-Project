#ifndef NTP_TIME_H
#define NTP_TIME_H

#include "time.h"
#include "esp_sntp.h"
#include "Text.h"

const char ntpServer[] PROGMEM = "th.pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

char timeBuffer[40];
struct tm timeinfo;

void setupNTP() {
  char ntpBuffer[64];
  strcpy_P(ntpBuffer, ntpServer);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpBuffer);
  sntp_set_sync_interval(21600000);

  printFromProgmem(msgGettingTime);
  while (!getLocalTime(&timeinfo)) {
    printFromProgmem(msgWaitNTP);
    delay(500);
  }

  printFromProgmem(msgTimeOK);
}

void printCurrentTime() {
  snprintf(timeBuffer, sizeof(timeBuffer), "%02d/%02d/%04d %02d:%02d:%02d",
           timeinfo.tm_mday,
           timeinfo.tm_mon + 1,
           timeinfo.tm_year + 1900,
           timeinfo.tm_hour,
           timeinfo.tm_min,
           timeinfo.tm_sec);
  printFromProgmem(msgCurrentTime);
  Serial.println(timeBuffer);
}

#endif
