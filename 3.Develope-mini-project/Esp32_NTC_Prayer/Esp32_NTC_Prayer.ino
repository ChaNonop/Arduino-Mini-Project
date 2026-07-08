#include "wifi_setup.h"
#include "ntp_time.h"
#include "prayer_time.h"
#include "Text.h"

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  getPrayerTimes();
  setupNTP();
  printCurrentTime();

  // ปิด WiFi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  getLocalTime(&timeinfo);
  printCurrentTime();
  delay(1000);
}
