#include <pins_arduino.h>
#include <stdint.h>
#include "sensor.h"

Sensor::Sensor() {
  Adafruit_VEML7700 veml = Light_sensor();
  
  //RTC_DS3231 rtc(0x68);  // Address 0x68

  temp_water_Pin = A0;
  moisture_Pin = A1;
  IR_recieve_Pin = D9;

  Pump_pin = D5;
  buzzle = D6;

  lcd_1(0x27, 16, 2);

  sampling = 5;
  now = 0;
  read_time = 500;
}

void Sensor::begin() {
  pinMode(moisture_Pin, INPUT_PULLUP);
  pinMode(Pump_pin, OUTPUT);
  pinMode(buzzle, OUTPUT);
  pinMode(led_indicator, OUTPUT);
}
void Sensor::Read_Sensor() {
  unsigned long now = 0;
  if (millis() - now <= read_time;) {
    now = millis();

    float Raw_Light_Sensor = veml.readALS();
    float white_light = veml.readWhite();
    float lux = veml.readLux();

    for (int i = 1; i < sampling; i++) {
      float h += dht.readHumidity();
      float t += dht.readTemperature();
      uint16_t temp_water += analogRead(temp_water_Pin);
      uint16_t m_water += analogRead(moisture_Pin);
    }
    float sum_h = h / sampling;
    float sum_temp = t / sampling;

    uint16_t sum_temp_water = temp_water / sampling;
    uint16_t sum_m_water = m_water / sampling;

    Display(&lux, &Raw_Light_Sensor, &white_light);
    calibrate(&sum_temp_water, &sum_m_water);
  }
}

void Sensor::calibrate(uint16_t *t_water, uint16_t water_m) {
}
void Sensor::Remote() {
  if (IrReceiver.decode())  // Received IR signal
  {
    // Print protocol
    switch (IrReceiver.decodedIRData.protocol) {
      case NEC: Serial.print("NEC: "); break;
      case SONY: Serial.print("SONY: "); break;
      case SHARP: Serial.print("SHARP: "); break;
      case PANASONIC: Serial.print("PANASONIC: "); break;
      case JVC: Serial.print("JVC: "); break;
      case RC5: Serial.print("Philips RC5: "); break;
      case RC6: Serial.print("Philips RC6: "); break;
      case DENON: Serial.print("DENON: "); break;
      case SAMSUNG: Serial.print("SAMSUNG: "); break;
      case LG: Serial.print("LG: "); break;
      case UNKNOWN: Serial.print("UNKNOWN: "); break;
    }
    // Print raw data
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    // Resume receiving
    IrReceiver.resume();
    delay(10);
  }
}

void Sensor::Pump(bool *open) {
  if (open == 1) {
    analogWrite(Pump_pin, 200);
    analogWrite(LED_BUILTIN, 40);
  }
}

void Sensor::Display(float *lux_data, float *Raw_data, float *White_data) {
  if () {  //รับค่ามาจากรีโมทแล้วจะให้แสดงผล
    lcd_1.setCursor(0, 0);
    lcd_1.print("lux:");
    lcd_1.print(*lux_data);
  }
}