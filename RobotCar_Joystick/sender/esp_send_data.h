#ifndef ESP_SEND_DATA_H
#define ESP_SEND_DATA_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Variable.h"
#include "MyEsp_address.h"

typedef struct Data {
  int32_t id;
  int16_t joy_left;
  int16_t joy_right;
} __attribute__((packed)) Data_t;

extern Control controller;

// ใช้ volatile สำหรับแฟล็กที่แชร์ระหว่าง interrupt และ loop
volatile bool sendSuccess = false;
volatile bool sendFailed = false;
unsigned long lastLedChange = 0;

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    sendSuccess = true;
    sendFailed = false;
    digitalWrite(controller.led_espNow_Pin, HIGH);
  } else {
    sendSuccess = false;
    sendFailed = true;
    digitalWrite(controller.led_espNow_Pin, LOW);
    //Serial.printf("Send Fail\n");
  }
}

void setup_esp_now(const uint8_t* peer_address) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    delay(1500);
    ESP.restart();
  }
  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer_address, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    //Serial.println(F("Failed to add peer"));
    return;
  }
  //Serial.println(F("ESP-NOW Init Success!"));
}

void update_led_status() {
  unsigned long now = millis();
  
  if (sendFailed && (now - lastLedChange > 100)) {
    digitalWrite(controller.led_espNow_Pin, !digitalRead(controller.led_espNow_Pin));
    lastLedChange = now;
  } 
  else if (sendSuccess) {
    digitalWrite(controller.led_espNow_Pin, HIGH);
  }
}

void send_data(Data_t* data, Control* controller, const uint8_t* peer_address) {
  data->id++;
  data->joy_left = controller->joy_send_left;
  data->joy_right = controller->joy_send_right;
  esp_now_send(peer_address, (uint8_t*)data, sizeof(Data_t));
}

#endif
