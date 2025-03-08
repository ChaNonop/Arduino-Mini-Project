#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// คลาสสำหรับเซ็นเซอร์แม่เหล็ก
class Magnetometer {
private:
    int pin;
    float v_ref;
    int value_adc;
    float voltage;
    float result;
public:
    Magnetometer(int adcPin, float vRef = 5.0) : pin(adcPin), v_ref(vRef) {

    }

    void begin() {
        pinMode(pin, INPUT);
    }

    void readSensor() {
        value_adc = analogRead(pin);
        voltage = (value_adc * v_ref) / 1023;
        result = (voltage - 2.5731) / 0.0027;
    }

    float getVoltage() { return voltage; }
    float getMagneticField() { return result; }
};

// คลาสสำหรับ LCD
class LCD_Display {
private:
    LiquidCrystal_I2C lcd;
public:
    int value_adc;
    LCD_Display() : lcd(0x27, 16, 2) {}

    void begin() {
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Hi, Welcome");
    }

    void showValues(float voltage, float magneticField) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("V: ");
        lcd.print(voltage, 2);
        lcd.print(" V");

        Serial.print("\nADC: ");
        Serial.print(value_adc, 2);

        Serial.print("\nV: ");
        Serial.print(voltage, 2);
        Serial.print(" V\n");

        lcd.setCursor(0, 1);
        lcd.print("M: ");
        lcd.print(magneticField, 2);
        lcd.print(" mT");

        Serial.print("M: ");
        Serial.print(magneticField, 2);
        Serial.print(" mT");
    }

    void showMessage(const char *message) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(message);
    }

    void turnOff() { lcd.noBacklight(); }
    void turnOn() { lcd.backlight(); }
};

// 🔹 คลาสสำหรับปุ่มกด
class Button {
private:
    int pin;
    unsigned long pressStartTime;
    bool isPressed;
public:
    Button(int buttonPin) : pin(buttonPin), isPressed(false) {}

    void begin() {
        pinMode(pin, INPUT_PULLUP);
    }

    bool isPressedOnce() {
        if (digitalRead(pin) == LOW) {
            delay(50); // กันสัญญาณเด้ง
            while (digitalRead(pin) == LOW); // รอจนกว่าปล่อยปุ่ม
            return true;
        }
        return false;
    }

    bool isHeldFor(unsigned long duration) {
        if (digitalRead(pin) == LOW) {
            if (!isPressed) {
                pressStartTime = millis();
                isPressed = true;
            }
            if (millis() - pressStartTime >= duration) {
                return true;
            }
        } else {
            isPressed = false;
        }
        return false;
    }
};

// สร้างอ็อบเจ็กต์ของแต่ละคลาส
Magnetometer sensor(A1);
LCD_Display display;
Button button(7);

bool systemOn = false; // เริ่มต้นอยู่ในโหมดพัก (แสดง "Hi, Welcome")
bool sleepMode = false;

void setup() {
    Serial.begin(9600);
    sensor.begin();
    display.begin();
    button.begin();
}

void loop() {
    if (!systemOn) { 
        if (button.isPressedOnce()) { // กดปุ่มครั้งเดียวเพื่อเริ่มทำงาน
            systemOn = true;
            display.showMessage("Starting...");
            delay(1000);
        }
        return; // ยังไม่เริ่มโชว์ค่าเซ็นเซอร์
    }

    if (sleepMode) {
        if (button.isPressedOnce()) { // ออกจากโหมดพัก
            sleepMode = false;
            display.turnOn();
            display.showMessage("Resuming...");
            delay(1000);
        }
        return;
    }

    sensor.readSensor();
    display.showValues(sensor.getVoltage(), sensor.getMagneticField());

    if (button.isHeldFor(2000)) { // กดปุ่มค้าง 2 วิ เข้าสู่โหมดพัก
        sleepMode = true;
        display.showMessage("Entering Sleep...");
        delay(2000);
        display.turnOff();
    }
     delay(200); // ทำให้การแสดงผลช้าลง
}
