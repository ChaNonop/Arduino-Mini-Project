#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "config.h"
#include "control_peer_esp.h"

EspNowControlProtocol* robot;

void setup() {
  //Serial.begin(115200);
  //Serial.println(F("Receiver Starting..."));
  robot = EspNowControlProtocol::getInstance();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  //Serial.print(F("Receiver MAC: "));
  //Serial.println(WiFi.macAddress());

  analogWriteFreq(25000);
  analogWriteRange(1023);

  robot->init_pins();

  robot->setupEspNow();
  Serial.println(F("Ready. Waiting for data..."));
}
void loop() {
  static unsigned long lastPrint = 0;
  
  /*if (millis() - lastPrint > 100) {
    lastPrint = millis();
    Serial.printf("Status - L: %d, R: %d, Obstacle: %d\n", robot->joy_left, robot->joy_right, robot->obstacle_detected);
  }*/

  robot->drive_motor();
  yield();
}