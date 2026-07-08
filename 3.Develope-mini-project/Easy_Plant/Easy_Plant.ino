#include <Arduino.h>
#include <Wire.h>


void setup() {
  Serail.begin(15200);

  wire.begin();
  lcd.init();

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  // veml.setGain(VEML7700_GAIN_1_8);
  // veml.setIntegrationTime(VEML7700_IT_100MS)

  if (rtc.lostPower()) {
    Serial.println("RTC ขาดพลังงาน กำลังตั้งค่าเวลาใหม่");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //กำหนดเวลาใหม่ ตอนถ่านหมด
  }
  // rtc.adjust(DateTime(2017,6,15,17,28,30)); //กำหนดเวลาใหม่
  
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
}

void loop() {
  DateTime now = rtc.now();
}
