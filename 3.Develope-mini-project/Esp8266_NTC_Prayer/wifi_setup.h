#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h> 
#include "Text.h"

const char ssid[] PROGMEM = "Somwang";
const char password[] PROGMEM = "03143681";

void connectToWiFi() {
  printFromProgmem(msgConnectingWiFi);
  char ssidBuffer[32], passwordBuffer[32];
  strcpy_P(ssidBuffer, ssid);
  strcpy_P(passwordBuffer, password);

  WiFi.begin(ssidBuffer, passwordBuffer);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  printFromProgmem(msgConnected);
}

#endif
