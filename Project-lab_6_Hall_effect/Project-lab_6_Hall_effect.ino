#include <Arduino.h>
#include <LibPrintf.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// สร้างอ็อบเจ็กต์ของจอ LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 คอลัมน์ 2 แถว

const int ADC_PIN = A0; 
const int button_pin = 7; 
float v_ref = 5.0;

unsigned long Time_display = 0; 
unsigned long buttonpress_time = 0;
bool ButtonPressed = false;
bool systemOn = true; // สถานะการทำงานของระบบ

int value_adc;
float voltage;
float result;  

char buffer[50];

void setup() {
  Serial.begin(19200);
  pinMode(button_pin, INPUT_PULLUP); // ใช้ PULLUP ป้องกันสัญญาณรบกวน
  Wire.begin();          
  lcd.init();            
  lcd.backlight();       
  lcd.setCursor(0,0);  
  lcd.print("Hi");
  delay(1000);
  lcd.clear();
}

void calculate() {
  value_adc = analogRead(ADC_PIN); 
  voltage = (value_adc * v_ref) / 1023; 
  result = (voltage - 2.5731) / 0.0027;  
}

void display_value() {
  if (millis() - Time_display > 500) {
    Time_display = millis();
    
    lcd.setCursor(0, 0);
    sprintf(buffer, "Voltage: %.2fV", voltage);
    lcd.print(buffer);

    lcd.setCursor(0, 1);
    sprintf(buffer, "Magnetic: %.2f mT", result);
    lcd.print(buffer);

    Serial.println(value_adc);
  }
}

void checkButtonHold() {
  if (digitalRead(button_pin) == LOW) { // ปุ่มถูกกด (LOW เพราะใช้ PULLUP)
    if (!ButtonPressed) {
      buttonpress_time = millis(); // บันทึกเวลาที่เริ่มกดปุ่ม
      ButtonPressed = true;
    }
    // ถ้ากดค้างเกิน 3 วินาที ให้เข้าสู่โหมดประหยัดพลังงาน
    if (ButtonPressed && (millis() - buttonpress_time >= 3000)) {
      save_Energy();
    }
  } else {
    ButtonPressed = false; // รีเซ็ตสถานะเมื่อปล่อยปุ่ม
  }
}

void save_Energy() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entering Sleep...");
  delay(2000);
  lcd.noBacklight();  
  systemOn = false; // ปิดการทำงานของระบบ
  
  // รอจนกว่าปล่อยปุ่มเพื่อออกจากโหมดเซฟพลังงาน
  while (!digitalRead(button_pin)); 

  // เปิดจอใหม่และรีเซ็ตระบบเมื่อกดปุ่มอีกครั้ง
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resuming...");
  delay(2000);
  systemOn = true;
}

void loop() {
  if (systemOn) {
    calculate();
    display_value();
    checkButtonHold();
  }
}
