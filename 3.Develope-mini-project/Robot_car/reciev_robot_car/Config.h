#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <pgmspace.h>
#include <Esp8266WiFi.h>

extern "C" {
  #include <espnow.h>
}


// --------- STRUCT DATA ---------
typedef struct struct_message {
  uint8_t id;
  uint8_t Data_Vx;
  uint8_t Data_Vy;
} struct_message;

struct_message incomingData;

// --------- PIN CONFIG ---------
const uint8_t PROGMEM IN1 = D5;
const uint8_t PROGMEM IN2 = D6;
const uint8_t PROGMEM IN3 = D7;
const uint8_t PROGMEM IN4 = D8;
const uint8_t PROGMEM EN_A = D1;
const uint8_t PROGMEM EN_B = D2;

unsigned int PROGMEM speedL;
unsigned int PROGMEM speedR;

const uint8_t PROGMEM TRIG = D3;
const uint8_t PROGMEM ECHO = D4;
const uint8_t PROGMEM BUZZER = D0;

bool PROGMEM forward;
float PROGMEM distance;

#endif