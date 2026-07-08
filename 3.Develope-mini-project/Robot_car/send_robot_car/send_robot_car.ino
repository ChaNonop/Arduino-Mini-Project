#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  uint8_t id;
  uint8_t Data_Vx;
  uint8_t Data_Vy;
} struct_message;

#define Vx_Pin 34 // ใช้ขา Analog input ได้ เช่น GPIO34
#define Vy_Pin 35 // อีกขา analog
#define LED_SEND 2
#define SAMPLING 10

// mac address 48:E7:29:64:DF:C8
uint8_t Address[] = {0x48, 0xE7, 0x29, 0x64, 0xDF, 0xC8}; // แก้ MAC จริงตรงนี้

struct_message Data_send;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print(F("Send Status: "));
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(LED_SEND, OUTPUT);

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("ESP-NOW Init Failed"));
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, Address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println(F("Failed to add peer"));
    return;
  }
}

void loop() {
  int sumVx = 0, sumVy = 0;

  for (int i = 0; i < SAMPLING; i++) {
    sumVx += analogRead(Vx_Pin);
    sumVy += analogRead(Vy_Pin);
    delay(5);
  }

  int avgVx = sumVx / SAMPLING;
  int avgVy = sumVy / SAMPLING;

  // ลดความละเอียดให้เหลือ 8 บิต (0-255)
  Data_send.id = 1;
  Data_send.Data_Vx = map(avgVx, 0, 4095, 0, 255);
  Data_send.Data_Vy = map(avgVy, 0, 4095, 0, 255);

  digitalWrite(LED_SEND, HIGH);
  esp_now_send(Address, (uint8_t *)&Data_send, sizeof(Data_send));
  digitalWrite(LED_SEND, LOW);

  delay(100); // ส่งทุก 100 ms
}
