#ifndef PRAYER_TIMES_H
#define PRAYER_TIMES_H

#include <HTTPClient.h>
#include <ArduinoJson.h>

const char API_prayer[] = "https://api.aladhan.com/timingsByAddress?address=London,%20UK&method=99&methodSettings=18.5,null,17.5";

void getPrayerTimes() {
  HTTPClient http;
  http.begin(API_prayer);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      const char* fajr = doc["data"]["timings"]["Fajr"];
      const char* dhuhr = doc["data"]["timings"]["Dhuhr"];
      const char* asr = doc["data"]["timings"]["Asr"];
      const char* maghrib = doc["data"]["timings"]["Maghrib"];
      const char* isha = doc["data"]["timings"]["Isha"];
      const char* date = doc["data"]["date"]["gregorian"]["date"];

      Serial.println("เวลาละหมาดในกรุงเทพฯ:");
      Serial.print("วันที่: "); Serial.println(date);
      Serial.print("Fajr: "); Serial.println(fajr);
      Serial.print("Dhuhr: "); Serial.println(dhuhr);
      Serial.print("Asr: "); Serial.println(asr);
      Serial.print("Maghrib: "); Serial.println(maghrib);
      Serial.print("Isha: "); Serial.println(isha);
    } else {
      Serial.print("JSON Parsing Failed: ");
      Serial.println(error.c_str());
    }
  } else {
    Serial.printf("HTTP Request Failed, code: %d\n", httpCode);
  }

  http.end();
}

#endif
