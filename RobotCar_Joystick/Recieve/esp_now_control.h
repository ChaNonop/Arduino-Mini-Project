#ifndef ESPNOW_CONTROL_H
#define ESPNOW_CONTROL_H

#include "config.h"
#include <espnow.h>

class EspNowControlProtocol : public control {
public:
  typedef struct Data {
    int id;
    int Joy_Vx;
    bool joy_send_button[2];
  } Data;

  Data incomingData;

  static void OnDataRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
    EspNowControlProtocol* instance = EspNowControlProtocol::getInstance();
    if (!instance) return;

    if (len != sizeof(Data)) return;

    memcpy(&(instance->incomingData), data, len);

    instance->joy_Vx = instance->incomingData.Joy_Vx;
    instance->calib();
    instance->drive_motor(instance->speedL, instance->speedR);
  }

  static EspNowControlProtocol* getInstance() {
    static EspNowControlProtocol instance;
    return &instance;
  }

  void setupEspNow() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0) {
      Serial.println(F("ESP-NOW Init Failed"));
      ESP.restart();
      return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
  }
};

#endif