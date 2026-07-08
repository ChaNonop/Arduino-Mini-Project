// Transmitter จอยสติกบังคับ

#include <"Arduino.h">
#include <RH_ASK.h>
#include <SPI.h>

unsigned int = 10; // Arduino nano
bool send = false;
RH_ASK driver;

class Control{ //อ่านค่าจาก Joystick
  private:
    unsigned const int Pin1_Vx = A0;
    unsigned const int Pin1_Vy = A1;
    unsigned const int Pin1_z = 1;

    unsigned const int Pin1_Vx =;
    unsigned const int Pin1_Vy;
    unsigned const int Pin1_z;
    int Joy1_Vx; //ตัวแปรเก็บค่าที่จะส่ง
    int Joy1_Vy;
    int Joy1_z;

    int Joy2_Vx;
    int Joy2_Vy;
    int Joy2_z;

  public:
    AdcRead(int pinJoy_1 ,int pinJoy_2){
      Joystick_1 = pinJoy_1;
      joystick_2 = pinJoy_2;
    }
    void Read_value(){
      Joystick_1 = analogRead(PinJoy_1);
      Joystick_2 = analogRead(PinJoy_2);
    }
}

class Transmitter{ // รับค่าจากอ่านแล้วส่งสัญญาณ
  private:
    unsigned int = 10; // Arduino nano
    bool send = false;
  public:
    void ()
}

void setup() {
  pinMode(button,INPUT);
  Serial.begin(9600);
  if(!driver.init()){
    Serial.println("Transmitter failed!");
  }
}

void loop() {

  if(button == True){
    const char*msg =
  }
    

}
