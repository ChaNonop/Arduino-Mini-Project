#include <ESP8266WiFi.h>
#include <Arduino.h>

#include "config.h"
#include "esp_now_control.h"

EspNowControlProtocol* robot;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Receiver..."));

  robot = EspNowControlProtocol::getInstance();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print(F("Receiver MAC: "));
  Serial.println(WiFi.macAddress());

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected!"));
  }

  robot->init_motor_pins();
  robot->setupEspNow();

  Serial.println(F("Ready. Waiting for data..."));
}

void loop() {
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 40) {
        lastCheck = millis();        
        Serial.printf("Last received - Vx: %d, Vy: %d\n", 
                     robot->incomingData.Joy_Vx, 
                     robot->incomingData.Joy_Vy);
    }
    yield(); 
}