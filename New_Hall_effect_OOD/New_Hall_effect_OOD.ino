//#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Magnetic { // คลาสสำหรับเซ็นเซอร์แม่เหล็ก
  private:
    unsigned int pin_read;
    const float V_ref;
    unsigned int value_adc;
    float voltage;
    float result;
    unsigned const int samples = 10;
    long sumADC = 0; // ใช้ long เพื่อความแม่นยำในการบวกค่า
  public:
    Magnetic(int pin = A0, float vRef = 3.3) 
      : pin_read(pin), V_ref(vRef), value_adc(0), voltage(0.0), result(0.0) {}

    void begin() {
      pinMode(pin_read, INPUT);
      //analogReadResolution(12);  // ตั้งค่า ADC เป็น 12 บิต ใน esp8266 ใข้คำสั่งนี้ไม่ได้
    }

void cal_Sensor() {
  for (int i = 0; i < samples; i++) {
    sumADC += analogRead(pin_read);
  }
  value_adc = sumADC/samples;  // ค่าเฉลี่ย ADC
  voltage = (value_adc * V_ref) / 4095.0;  // คำนวณแรงดันจากค่าเฉลี่ย ADC (สำหรับ ADC 12-bit)
  result = (voltage - 2.5731) / 0.0027;    // คำนวณค่าที่ต้องการจากแรงดัน
}
    float getVoltage() {
      return voltage;
    }

    float getMagnetic() {
      return result;
    }
    
    int getADC() {
      return value_adc;
    }
};

class LCD_Display {  //อันนี้อันที่เราลองเทส
  private:
    LiquidCrystal_I2C *lcd;
  public:
    LCD_Display() {
      lcd = new LiquidCrystal_I2C(0x27, 16, 2); // จองหน่วยความจำ
    }

    void begin() {
      lcd->init();
      lcd->backlight();
      lcd->setCursor(0, 0);
      lcd->print("Magnetic Sensor");
      lcd->setCursor(0, 1);
      lcd->print("Ready!");
    }

    void showValues(float voltage, float magnet, int adcValue) {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("V: ");
      lcd->print(voltage, 2);
      lcd->print(" V");

      lcd->setCursor(0, 1);
      lcd->print("M: ");
      lcd->print(magnet, 2);
      lcd->print(" mT");

      Serial.print("\nADC: ");
      Serial.print(adcValue);
    }

      void showMessage(const char *message) {
      lcd->print(message);
    }

    /*void turnOff() { 
      lcd.noBacklight(); 
    }

    void turnOn() { 
      lcd.backlight(); 
    }*/
    ~LCD_Display() { 
      delete lcd; // ลบหน่วยความจำที่จองไว้
    }
};

/*class LCD_Display { // คลาสสำหรับ LCD  อันนี้อันปกติ
  private:
    LiquidCrystal_I2C lcd;
  public:
    LCD_Display() : lcd(0x27, 16, 2) {}

    void begin() {
      lcd.init();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print("Magnetic Sensor");
      lcd.setCursor(0, 1);
      lcd.print("Ready!");
    }

    void showValues(float voltage, float magnet, int adcValue) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("V: ");
      lcd.print(voltage, 2);
      lcd.print(" V");

      lcd.setCursor(0, 1);
      lcd.print("M: ");
      lcd.print(magnet, 2);
      lcd.print(" mT");

      Serial.print("\nADC: ");
      Serial.print(adcValue);
    }

    void showMessage(const char *message) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(message);
    }

    void turnOff() { 
      lcd.noBacklight(); 
    }

    void turnOn() { 
      lcd.backlight(); 
    }
};*/

class Button { // คลาสสำหรับปุ่มกด
  private:
    int pin;
  public:
    Button(int buttonPin) : pin(buttonPin) {}

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
};

// สร้างอ็อบเจ็กต์ของแต่ละคลาส
Magnetic sensor(A0,3.3);
LCD_Display display;
Button button(7);

bool systemOn = false;

void setup() {
  Serial.begin(115200);
  sensor.begin();
  display.begin();
  button.begin();
}

void loop() {
  if (button.isPressedOnce()) { // กดปุ่มเพื่อเปิด/ปิดการอ่านค่า
    systemOn = !systemOn;
    display.showMessage(systemOn ? "Starting..." : "Stopped");
    delay(500);
  }

  if (systemOn) {
    sensor.cal_Sensor();
    display.showValues(sensor.getVoltage(), sensor.getMagnetic(), sensor.getADC());
    delay(500);
  }
}
