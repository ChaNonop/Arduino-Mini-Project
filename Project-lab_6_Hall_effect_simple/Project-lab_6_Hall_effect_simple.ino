#include <Arduino.h>

const int ADC_PIN = A0; 
float v_ref = 5.00;

//float v_ref = 3.3;

unsigned long Time_display = 0;

int value_adc;
float voltage;
float result;

void setup() {
  Serial.begin(19200);
}

void calculate() {
  value_adc = analogRead(ADC_PIN);
  voltage = (value_adc * v_ref*(1000)) / 1023.00;
  result = (voltage/1000 - 2.5333)/(0.0138);
}

void display_value() {
  if (millis() - Time_display > 500) { // อัปเดตค่าทุก 500ms
    Time_display = millis();
    
    Serial.print("ADC: ");
    Serial.print(value_adc);
    
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 2);
    Serial.print(" mV | Magnetic: ");
    Serial.print(result, 2);
    Serial.println(" mT");
  }
}

void loop() {
  calculate();
  display_value();
}

