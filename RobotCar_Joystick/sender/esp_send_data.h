#ifndef ESP_SEND_DATA_H
#define ESP_SEND_DATA_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_WiFi.h>
#include <esp_now.h>

#include "Variable.h"
// ไลบราลี่เราเอง
#include <MyEsp_address.h>

typedef struct Data {
  int id;
  int joy_left = 0;
  int joy_right = 0;
} Data_t;

extern Control controller;

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    digitalWrite(controller.led_espNow_Pin, HIGH);
  } else {
    // Fail
    digitalWrite(controller.led_espNow_Pin, HIGH);
    delay(10);
    digitalWrite(controller.led_espNow_Pin, LOW);
    delay(10);
    Serial.println("Send Fail");
  }
}

void setup_esp_now(const uint8_t* peer_address) {
  WiFi.mode(WIFI_STA);
  WiFi.channel(1);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("ESP-NOW Init Failed... Rebooting..."));
    delay(1000);
    ESP.restart();
  }
  // callback สำหรับการส่ง
  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer_address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println(F("Failed to add peer"));
    return;
  }
  Serial.println(F("ESP-NOW Init Success!"));
}

void send_data(Data_t* data, Control* controller, const uint8_t* peer_address) {
  data->id++;
  data->joy_left = controller->joy_send_left;
  data->joy_right = controller->joy_send_right;
  // ส่งข้อมูล
  esp_now_send(peer_address, (uint8_t*)data, sizeof(Data_t));
}

#endif