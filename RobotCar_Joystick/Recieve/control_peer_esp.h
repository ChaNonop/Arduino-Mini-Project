#ifndef CONTROL_PEER_ESP_H
#define CONTROL_PEER_ESP_H

#include "config.h"
#include <espnow.h>

class EspNowControlProtocol : public control {
public:
  typedef struct Data {
    int32_t id;
    int16_t joy_left;
    int16_t joy_right;
  } __attribute__((packed)) Data_t;

  Data_t incomingData;

  static void OnDataRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
    if (len != sizeof(Data_t)) {
      return;
    }
    
    if (!esp_now_is_peer_exist(mac)) {
        esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    }

    EspNowControlProtocol* instance = getInstance();
    if (!instance) {
      return;
    }
    memcpy(&(instance->incomingData), data, len);

    instance->joy_left = instance->incomingData.joy_left;
    instance->joy_right = instance->incomingData.joy_right;
    instance->last_data_time = millis();
  }

  static EspNowControlProtocol* getInstance() {
    static EspNowControlProtocol instance;
    return &instance;
  }

  void setupEspNow() {
    if (esp_now_init() != 0) {
      //Serial.println(F("ESP-NOW Init Failed. Rebooting..."));
      delay(1000);
      ESP.restart();
    }
    
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(OnDataRecv);
    //Serial.println(F("ESP-NOW Ready"));
  }
};

#endif
