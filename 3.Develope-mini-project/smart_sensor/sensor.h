#ifndef SENSOR_H
#define SENSOR_H

#define DHTPIN 2
#define DHTTYPE DHT11

// ------------------ Struct definition ------------------
// ออกแบบให้ประหยัดหน่วยความจำ: ใช้ประเภทข้อมูลขนาดเล็กเมื่อเป็นไปได้
struct SensorData{
  uint8_t id;  // small id
  uint8_t pin; // เก็บหมายเลขพิน (A0 -> ค่าภายในเป็น 14 บน UNO แต่เก็บเป็น byte ก็พอ)
  uint8_t value; 
  bool valid; // flag ว่าค่านี้ "มีค่า" หรือเป็น NULL
  unsigned long lastUpdate; // เวลา millis() ที่อ่านล่าสุด
}
struct buttonPin{
    const byte button_1;
    const byte button_2;
    const byte button_3;
}
// ------------------ Configuration ------------------
const size_t num_sensor = 4; // เปลี่ยนตามจำนวนจริง
SensorData sensorValue[num_sensor];

}
SensorPin SP;
SensorData SD ={0,0};

#endif