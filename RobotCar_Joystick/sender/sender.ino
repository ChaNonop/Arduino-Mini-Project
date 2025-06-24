#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
//#include <espnow.h>
#include <esp_now.h>
#include "Variable.h"
#include "esp_send_data.h"

Address mac_address;
const byte *peer_address = mac_address.Esp8266_V3;

Data Data_send;
Control controller;

#define TX_POWER

/*void printMacAddresses() {
  // แสดง MAC Address ของตัวส่ง (Sender)
  Serial.print("Sender MAC: ");
  Serial.println(WiFi.macAddress());

  // แสดง MAC Address ของอุปกรณ์ปลายทาง (Peer)
  Serial.print("Peer MAC: ");
  for (int i = 0; i < 6; i++) {
    if (peer_address[i] < 0x10) { // เติม 0 นำหน้าถ้าเป็นเลขฐาน 16 หลักเดียว
      Serial.print("0");
    }
    Serial.print(peer_address[i], HEX); // แสดงในรูปแบบ HEX
    
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println(); // ขึ้นบรรทัดใหม่
}*/

void setup() {
  Serial.begin(115200);
  Serial.println("Booting Sender...");
  //printMacAddresses(); // แสดง MAC Addresses

  pinMode(controller.led_Vx_pin, OUTPUT);
  pinMode(controller.led_Vy_pin, OUTPUT);
  //pinMode(controller.led_button_pin, OUTPUT);
  //pinMode(controller.led_espNow_Pin, OUTPUT);
  Serial.println(F("Setup Complete...."));
}

void loop() {
  controller.read_adc();

  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 15) {  // ส่งทุก 15ms
    send_data();
    lastSend = millis();
    Serial.printf("Send: Vx=%d, B1=%d, B2=%d\n",
                  Data_send.Joy_Vx,
                  Data_send.joy_send_button[0],
                  Data_send.joy_send_button[1]);
  }
  yield();
}