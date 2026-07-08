// คำอธิบาย: โค้ดนี้ใช้เทคนิค Circular Buffer และ Moving Average
// เพื่ออ่านค่าเซ็นเซอร์และหาค่าเฉลี่ยแบบไม่บล็อก (Non-blocking)
// ทำให้ CPU สามารถทำงานอื่นได้ในลูปหลัก

#include <Arduino.h>
#include <Wire.h>

const int range_index = 10;                    
unsigned long time_before = 0;                  
const int Dry_net = 3680;                        // ค่า Calibrate เมื่อเซ็นเซอร์แห้งสนิท
const int Wet_net = 1395;                        // ค่า Calibrate เมื่อเซ็นเซอร์เปียกสนิท

int sensor_read[range_index] = {0};             
float avg_moisture = 0.0;                      

float Percent_moisture = 0;

const uint8_t Moisture_pin = 4; // ขา D4       
uint8_t read_index = 0;                        
long total_sum = 0;                           

void setup() {
  Serial.begin(19200);
  pinMode(Moisture_pin, INPUT);
}

void loop() {

}
