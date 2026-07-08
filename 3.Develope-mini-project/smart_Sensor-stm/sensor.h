#ifndef SENSOR_H
#define SENSOR_H

//#define DHTPIN 2
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

/*อ่านค่าเซ็นเซอร์ อะไรบ้าง มี 
    1.MQ Gas
    2.DHT11
    3.Lux
    4.sound
*/
struct SensorData{
  uint8_t id;  // id
  uint8_t pin; // เก็บหมายเลขพิน (A0 -> ค่าภายในเป็น 14 บน UNO แต่เก็บเป็น byte ก็พอ)
  float value; 
  bool valid; // flag ว่าค่านี้ "มีค่า" หรือเป็น NULL
  unsigned long lastUpdate; 

struct SensorPin{
    const byte Lux_Pin;
    const byte sound_Pin;
    const byte MQ_Pin;
    const byte DHTPIN;

    const byte digi_sound;
    const byte button_1;
    const byte button_2;
    const byte button_3;
}
const size_t num_read = 8; 
const sampling = 5;
//SensorData sensorValue[num_read,sampling];
SensorData sensorValue[sizeof(sensorData),sampling];

SensorPin SP ={A0,A1,A2,A3,2,3,4,5};
SensorData SD ={0,0,0,0,0,0};

#endif