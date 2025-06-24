#ifndef ESP_SEND_DATA_H
#define ESP_SEND_DATA_H

#include <Arduino.h>
#include "Variable.h"
//#include <ESP8266WiFi.h>
//#include <espnow.h>
#include <WiFi.h>
#include <esp_WiFi.h>
#include <esp_now.h>

// ไลบราลี่เราเอง
#include <MyEsp_address.h>

extern Address mac_address;
extern const byte *peer_address;

typedef struct Data {
  int id;
  int Joy_Vx;
  int Joy_Vy;
  //bool joy_send_button[2];
} Data;

extern Data Data_send;
extern Control controller;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (sendStatus == 0) {
    analogWrite(controller.led_espNow_Pin, 200);
  } else {
    // Fail
    digitalWrite(controller.led_espNow_Pin, HIGH);
    delay(50);
    digitalWrite(controller.led_espNow_Pin, LOW);
    delay(50);
  }
}

void setup_esp_now() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_mac_tx_power(TX_POWER);  //set tx power
  delay(10);

  while (esp_now_init() != 0) {
    Serial.println(F("ESP-NOW Init Failed... Rebooting..."));
    digitalWrite(controller.led_espNow_Pin, HIGH);
    delay(100);
    digitalWrite(controller.led_espNow_Pin, LOW);
    delay(100);
    ESP.restart();
  }

  Serial.println(F("ESP-NOW Init Success!"));

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // callback สำหรับการส่ง
  esp_now_register_send_cb(OnDataSent);

  // ตั้งค่า peer
  esp_now_add_peer(const_cast<uint8_t *>(peer_address), ESP_NOW_ROLE_SLAVE, 6, NULL, 0);

  // เชื่อมสำเร็จแล้วให้ขึ้น led ค้างไว้
  analogWrite(controller.led_espNow_Pin, 512);
}

void send_data() {
  Data_send.id++;
  Data_send.Joy_Vx = controller.joy_send_Vx;  //ดึงค่า
  Data_send.Joy_Vy = controller.joy_send_Vy;
  //Data_send.joy_send_button[0] = controller.joy_send_button[0];  //ดึงค่า
  //Data_send.joy_send_button[1] = controller.joy_send_button[1];  //ดึงค่า

  // ส่งข้อมูล
  esp_now_send(0, (uint8_t *)&Data_send, sizeof(Data_send));
}

#endif