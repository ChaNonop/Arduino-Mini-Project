#ifndef ESPNOW_CONTROL_H
#define ESPNOW_CONTROL_H

#include "config.h"
#include <espnow.h>

class EspNowControlProtocol : public control {
public:
  typedef struct Data {
    int id;
    int Joy_Vx = 0;
    int Joy_Vy = 0;
  } Data_t;

  Data_t incomingData;

  static void OnDataRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
    EspNowControlProtocol* instance = EspNowControlProtocol::getInstance();
    // ตรวจสอบ instance และขนาดข้อมูลถูกมัย
    if (!instance || len != sizeof(Data_t)) {
      return;
    }
    memcpy(&(instance->incomingData), data, len);

    instance->joy_Vx = instance->incomingData.Joy_Vx;
    instance->joy_Vy = instance->incomingData.Joy_Vy;

    instance->cal_speed();
    instance->drive_motor();
  }

  static EspNowControlProtocol* getInstance() {
    static EspNowControlProtocol instance;
    return &instance;
  }

  void setupEspNow() {
    if (esp_now_init() != 0) {
      Serial.println(F("ESP-NOW Init Failed. Rebooting..."));
      delay(1000);
      ESP.restart();
    }
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
  }
};

#endif