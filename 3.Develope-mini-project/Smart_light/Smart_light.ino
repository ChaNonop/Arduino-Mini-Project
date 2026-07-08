#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Variable_Pin.h"
#include "Segment.h"
#include "Sound.h"
#include "Awake.h"


unsigned long lastClapTime = 0;
byte clapCount = 0;

void setup() {
  Serial.begin(115200);

  pinMode(buzzle_pin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED บนบอร์ด Blue Pill ดับเมื่อ HIGH (Active Low)

  dht.setup(dht_pin);

  for (byte i = 0; i < 3; i++) {
    pinMode(Led_power[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // ปิด Digit ทั้งหมดเริ่มต้น
  }
    // กำหนดขาเป็น Output
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void loop() {
  reading();
  DHT();
}
