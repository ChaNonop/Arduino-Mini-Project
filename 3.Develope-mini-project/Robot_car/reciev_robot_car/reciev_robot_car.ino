// ฝั่งรับ ESP8266: รับข้อมูลหลายชุด, ควบคุมมอเตอร์, ตรวจสอบ ultrasonic
#include <ESP8266WiFi.h>
#include <Config.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // MOTOR PIN SETUP
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(EN_A, OUTPUT); pinMode(EN_B, OUTPUT);

  // ULTRASONIC SETUP
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  analogWrite(EN_A, 0);
  analogWrite(EN_B, 0);

  if (esp_now_init() != 0) {
    Serial.println(F("ESP-NOW init failed"));
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  distance = readDistanceCM();
  Serial.print(F("[SENSOR] Distance:"));
  Serial.print(distance);
  Serial.println("cm\n");
  if (distance < 20) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }
  delay(100);
}

// --------- MOTOR CONTROL ---------
void driveMotor(int vx, int vy) {
  speedL = constrain(map(vx + vy - 255, 0, 255, 0, 1023), 0, 1023);
  speedR = constrain(map(vx - vy, 0, 255, 0, 1023), 0, 1023);

  // ทิศทาง (forward/backward)
  forward = (vy >= 128);

  digitalWrite(IN1, forward);
  digitalWrite(IN2, !forward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, !forward);

  analogWrite(EN_A, speedL);
  analogWrite(EN_B, speedR);
}

// --------- SONIC ---------
float readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.0343 / 2;
}

// --------- CALLBACK ---------
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  if (len != sizeof(struct_message)) return;
  memcpy(&incomingData, data, sizeof(incomingData));

  //Serial.printf("[RECV] ID: %d | Vx: %d | Vy: %d\n", 
  //            incomingData.id, incomingData.Data_Vx, incomingData.Data_Vy);

  driveMotor(incomingData.Data_Vx, incomingData.Data_Vy);
}
