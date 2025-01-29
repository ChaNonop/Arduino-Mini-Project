#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// กำหนดค่าคงที่
#define DHTPIN 2           // พินของเซนเซอร์ DHT11
#define DHTTYPE DHT11      // ชนิดของเซนเซอร์ DHT
#define PIN_LIGHT A0       // พินสำหรับเซนเซอร์วัดแสง
#define BUTTON1 3          // ปุ่ม 1
#define BUTTON2 4          // ปุ่ม 2
#define BUTTON3 5          // ปุ่ม 3
#define LCD_ADDR 0x27      // ที่อยู่ของ LCD
#define LCD_COLS 16        // จำนวนคอลัมน์ของ LCD
#define LCD_ROWS 2         // จำนวนแถวของ LCD
#define UPDATE_INTERVAL 5000 // ช่วงเวลาอัปเดต (มิลลิวินาที)

// ออบเจกต์
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd_1(LCD_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  lcd_1.init();
  lcd_1.backlight();
  //lcd.noBacklight();

  // ตั้งค่าปุ่มเป็น Input
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);

  Serial.printf("Free Heap Memory: %d bytes\n", ESP.getFreeHeap());
}

void loop() {
  static unsigned long lastUpdate = 0; // ใช้ static เพื่อลดการใช้หน่วยความจำ
  unsigned long currentTime = millis(); // เวลาในปัจจุบัน

  // อัปเดตข้อมูลทุก 5 วินาที
  if (currentTime - lastUpdate > UPDATE_INTERVAL) {
    lastUpdate = currentTime;

    // อ่านค่าจากเซนเซอร์
    int lightValue = analogRead(PIN_LIGHT);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // ตรวจสอบความถูกต้องของค่าเซนเซอร์
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      lcd_1.setCursor(0, 0);
      lcd_1.print("DHT Error!");
    } else {
      // แสดงค่าบน Serial Monitor
      Serial.printf("Light: %d\n", lightValue);
      Serial.printf("Temp: %.2f C\n", temperature);
      Serial.printf("Humidity: %.2f %%\n", humidity);

      // แสดงค่าบน LCD
      lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.printf("Temp: %.1fC", temperature);
      lcd_1.setCursor(0, 1);
      lcd_1.printf("Humid: %.1f%%", humidity);

    delay(5000);
    Serial.printf("Free Heap Memory: %d bytes\n", ESP.getFreeHeap());//ไม่เกี่ยววัด memmor เฉยๆ
    }
  }
}
