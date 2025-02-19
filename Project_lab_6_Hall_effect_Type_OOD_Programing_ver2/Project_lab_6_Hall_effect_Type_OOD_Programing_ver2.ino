#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ADC_PIN = A0;
const int BUTTON_PIN = 7;

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long pressStartTime = 0;
bool systemOn = false;
bool sleepMode = false;
bool isPressed = false;

void setup() {
    Serial.begin(19200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Magnetic Field");
}

void loop() {
    // อ่านค่าปุ่ม
    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!isPressed) {
            pressStartTime = millis();
            isPressed = true;
        }

        // กดปุ่มค้าง 3 วิ → Sleep Mode
        if (millis() - pressStartTime >= 2000) {
            sleepMode = !sleepMode;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(sleepMode ? "Sleep Mode" : "Resuming...");
            delay(1000);
            lcd.noBacklight();
            while (digitalRead(BUTTON_PIN) == LOW); // รอให้ปล่อยปุ่มก่อน
        }
    } else {
        if (isPressed) {
            isPressed = false;
            if (!systemOn) {
                systemOn = true;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Starting...");
                delay(1000);
            }
        }
    }

    if (!systemOn || sleepMode) {
        return;
    }

    // อ่านค่าเซ็นเซอร์
    int value_adc = analogRead(ADC_PIN);
    float voltage = (value_adc * 5.0) / 1023;
    float result = (voltage - 2.5731) / 0.0027;

    // แสดงผล
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V: ");
    lcd.print(voltage, 2);
    lcd.print(" V");
    lcd.setCursor(0, 1);
    lcd.print("B: ");
    lcd.print(result, 1);
    lcd.print(" mT");
    delay(600);
}
