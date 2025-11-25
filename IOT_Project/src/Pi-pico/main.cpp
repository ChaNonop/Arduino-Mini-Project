#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_ASK.h>

#define RX_PIN 27
#define TX_PIN 26

// (Speed 2000bps)
RH_ASK rf_driver(1000, RX_PIN, TX_PIN, 0);
 

const int button_Pin = 15;
const char *msg = "Welcome home";

void setup() {
  Serial.begin(19200);
  pinMode(button_Pin,INPUT_PULLUP);

  if (!rf_driver.init()) {
    Serial.println(F("RF driver init failed!"));
  } else {
    Serial.println(F("RF driver init success."));
  }
}

void loop() {
  int logic = digitalRead(button_Pin);
  Serial.println(logic);
  delay(100);

  if (logic == LOW) {  // กดปุ่มแล้วส่ง
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    Serial.println(F("Message Sent: Welcome home"));
    Serial.print(F("Logic : "));
    Serial.println(logic);
    delay(100); 
  } else {
    Serial.print(F("Logic : "));
    Serial.println(logic);
    Serial.println(F("Not sending data"));
  }
}

// class sensor{
//   private:

//   public:      
// }
// class Wifimanager{
//   private:

//   public:      
// }

