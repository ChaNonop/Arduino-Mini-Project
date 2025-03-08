#include <Arduino.h>

class ADCReader {
  private:
    const int adcPin;                // ขา ADC ที่ใช้
    const int sampleCount;           // จำนวนรอบที่ต้องการอ่านค่า
    const int timeRead;              // เวลาระหว่างการอ่านค่า (ms)
    unsigned long previousMillis;    // เก็บเวลาล่าสุดที่อ่านค่า
    int adcSamples[10];              // อาร์เรย์เก็บค่าที่อ่านได้ (ขนาดคงที่)
    int sampleIndex;                 // ตำแหน่งของข้อมูลล่าสุด
    const int ledPin;                // ขา LED (ต้องเป็นขา PWM เช่น D1, D2 บน ESP8266)
    int ledBrightness;               // ความสว่างของ LED (0-255)

  public:
    // Constructor: กำหนดค่าเริ่มต้น
    ADCReader(int pin, int samples, int interval, int ledPin)
      : adcPin(pin), sampleCount(samples), timeRead(interval), previousMillis(0), sampleIndex(0), ledPin(ledPin), ledBrightness(0) {
        pinMode(ledPin, OUTPUT);
    }

    // อ่านค่า ADC และคำนวณค่าเฉลี่ย
    int readAverage() {
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillis >= timeRead) {
            previousMillis = currentMillis;

            // อ่านค่า ADC และเก็บลงอาร์เรย์
            adcSamples[sampleIndex] = analogRead(adcPin);
            sampleIndex = (sampleIndex + 1) % sampleCount;

            // คำนวณค่าเฉลี่ย
            int sum = 0;
            for (int i = 0; i < sampleCount; i++) {
                sum += adcSamples[i];
            }
            return sum / sampleCount;
        }
        return -1; // ส่งค่า -1 ถ้ายังไม่ถึงเวลาอ่านใหม่
    }

    // ปรับความสว่างของ LED ตามค่า ADC
    void adjustLedBrightness() {
        int avg = readAverage();
        if (avg != -1) {
            // แปลงค่า ADC (0-1023) เป็นค่า PWM (0-255)
            ledBrightness = map(avg, 0, 1023, 0, 255);
            analogWrite(ledPin, ledBrightness); // ตั้งค่าความสว่างของ LED
        }
    }

    // แสดงค่าผ่าน Serial Monitor
    void printAverage() {
        int avg = readAverage();
        if (avg != -1) {
            Serial.print("ADC : ");
            Serial.print(avg);
            Serial.print(" -> LED Brightness : ");
            Serial.println(ledBrightness);
        }
    }
};

// ============================
// ⚡ โปรแกรมหลัก
// ============================
ADCReader adc(A0, 10, 20, D1); // กำหนดให้ใช้ขา A0, เก็บค่า 10 ตัว, อ่านทุก 20ms, LED ที่ขา D1 (PWM)

void setup() {
    Serial.begin(9600);
}

void loop() {
    adc.adjustLedBrightness(); // ปรับความสว่างของ LED ตามค่า ADC
    adc.printAverage();        // แสดงค่า ADC และความสว่างของ LED
}