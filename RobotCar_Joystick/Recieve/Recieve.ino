#include <ESP8266WiFi.h>
#include <Arduino.h>

#include "config.h"
#include "esp_now_control.h"

EspNowControlProtocol* robot;

void setup() {
    Serial.begin(115200);
    Serial.println(F("Initializing Receiver..."));

    robot = EspNowControlProtocol::getInstance();
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    Serial.print(F("Receiver MAC: "));
    Serial.println(WiFi.macAddress());
    
    robot->init_motor_pins();
    robot->setupEspNow();
    
    Serial.println(F("Receiver Ready. Waiting for data..."));
}

void loop() {
    delay(10);
}