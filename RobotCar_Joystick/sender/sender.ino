#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Variable.h"
#include "esp_send_data.h"
#include "MyEsp_address.h"

Address mac_address;
const uint8_t* peer_address = mac_address.Esp8266_V3;

Data_t Data_send;
Control controller;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting Sender...");
  
  setup_esp_now(peer_address);

  pinMode(controller.led_left_pin, OUTPUT);
  pinMode(controller.led_right_pin, OUTPUT);
  pinMode(controller.led_espNow_Pin, OUTPUT);
  Serial.println(F("Setup Complete...."));
}

void loop() {
  controller.read_adc();

  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 50) {
    lastSend = millis();
    send_data(&Data_send, &controller, peer_address);
    Serial.printf("Send: L = %d, R = %d\n", Data_send.joy_left, Data_send.joy_right);
  }
  yield();
}
