#include <Arduino.h>
#include <Servo.h>
#include <avr/pgmspace.h>  // สำหรับ PROGMEM 

struct Arm_commandPin {
  //ใช้จอย
  const int BasePin_Vx = A0;      // พินควบคุมฐาน (อ่าน analog)
  const int BasePin_Vy = A1;       //

  const int ShoulderPin_Vx = A2;  // พินควบคุมหัวไหล่ (อ่าน analog)
  const int ShoulderPin_Vy = A3;

  const int Gripper_Pin = A5;    // พินควบคุมปากจับ (digital input)
  
  //ปุ่ม button
  const int ElbowPin_Up = 8;      // ตั้งพินสำหรับการควบคุมข้อศอก
  const int ElbowPin_Down = 9;    // ตั้งพินสำหรับการควบคุมข้อศอก

  //const int Elbow_Pin = A3;
};

// servoPin เก็บใน Flash (PROGMEM)
const uint8_t servoPin[4] PROGMEM = {5, 6, 9, 10};

struct Control {
  Servo servolist[4];
  unsigned int speed = 100;
};

Arm_commandPin command;
Control C;

void Read_Input(){
  // อ่านค่า joystick
  unsigned int Base_Vx = analogRead(command.BasePin_Vx);
  unsigned int Base_Vy = analogRead(command.BasePin_Vy);
  unsigned int Shoulder_Vx = analogRead(command.ShoulderPin_Vx);
  unsigned int Shoulder_Vy = analogRead(command.ShoulderPin_Vy);

  //อ่านค่าปู่ม button 
  int Toggle_Gripper = digitalRead(command.Gripper_Pin);
  int Elbow_Up = digitalRead(command.ElbowPin_Up);
  int Elbow_Down = digitalRead(command.ElbowPin_Down);
  delay(2);
}

void process(){

}

uint8_t servoPins[4]; // ตัวแปรเก็บ pin ที่อ่านจาก PROGMEM

void setup() {
  Serial.begin(115200);

  // อ่านค่าจาก PROGMEM มาเก็บใน SRAM
  for (int i = 0; i < 4; i++) {
    servoPins[i] = pgm_read_byte(&(servoPin[i]));
  }
  // ตั้งค่า pin input สำหรับปุ่ม digital
  pinMode(command.Gripper_Pin, INPUT);
  pinMode(command.ElbowPin_Up, INPUT);
  pinMode(command.ElbowPin_Down, INPUT);

  // set ขาควบคุม serrvo
  for (int i = 0; i < 4; i++) {
    C.servolist[i].attach(servoPins[i]);
  }
}

void loop() {
  Read_Input();
  delay(500);
}
