#ifndef PRAYER_TIMES_H
#define PRAYER_TIMES_H

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char API_prayer[] = "http://api.aladhan.com/v1/timingsByCity?city=Bangkok&country=Thailand&method=2";

void getPrayerTimes() {
  WiFiClient client;
  HTTPClient http;

  // ใช้รูปแบบใหม่ของ begin() ที่ต้องส่ง WiFiClient ด้วย
  http.begin(client, API_prayer);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      JsonObject timings = doc["data"]["timings"];
      const char* fajr = timings["Fajr"];
      const char* dhuhr = timings["Dhuhr"];
      const char* asr = timings["Asr"];
      const char* maghrib = timings["Maghrib"];
      const char* isha = timings["Isha"];
      const char* date = doc["data"]["date"]["gregorian"]["date"];

      Serial.println("เวลาละหมาดในกรุงเทพฯ:");
      Serial.print("วันที่: ");
      Serial.println(date);
      Serial.print("Fajr: ");
      Serial.println(fajr);
      Serial.print("Dhuhr: ");
      Serial.println(dhuhr);
      Serial.print("Asr: ");
      Serial.println(asr);
      Serial.print("Maghrib: ");
      Serial.println(maghrib);
      Serial.print("Isha: ");
      Serial.println(isha);
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