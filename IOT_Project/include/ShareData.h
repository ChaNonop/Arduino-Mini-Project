#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <Arduino.h>

// Struct สำหรับแพ็คข้อมูล
// __attribute__((packed)) เพื่อบังคับให้ไม่มีการเว้นว่าง (Padding) ในหน่วยความจำ
// สำคัญมากเมื่อส่งข้อมูลข้ามระว่าง MCU คนละสถาปัตยกรรม (ARM Cortex-M0+ กับ Xtensa)
struct SensorPacket {
    uint8_t nodeId;      // ID ประจำตัวโหนด
    float temperature;   // อุณหภูมิ
    float humidity;      // ความชื้น
    uint16_t pressure;   // ความดัน
    uint32_t timestamp;  // เวลาทำงาน
};

#endif