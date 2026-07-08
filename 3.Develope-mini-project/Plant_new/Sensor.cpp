#include <pins_arduino.h>
#include "sensor.h"

Sensor::Sensor(){
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
}

void Sensor::sensor_moisture(){
    unsigned long time_now = millis();

  if (time_now - time_before >= 250) {
    time_before = time_now;

    int value = analogRead(Moisture_pin);

    // ลบค่าเก่าออกจากผลรวมและเพิ่มค่าใหม่เข้าไป
    total_sum = total_sum - sensor_read[read_index] + value;
    sensor_read[read_index] = value;

    avg_moisture = (float)total_sum / range_index;

    // เลื่อนดัชนีไปตำแหน่งถัดไป (Circular Buffer)
    read_index++;
    if (read_index >= range_index) {
      read_index = 0; 
    }
    Percent_moisture = map(avg_moisture, Dry_net, Wet_net, 0, 100);
    // แสดงผล
    Serial.print("Moisture: ");
    Serial.print(avg_moisture);
    Serial.print(" | Percent: ");
    Serial.println(Percent_moisture);
  }
}