#include <Arduino.h>
#include <LibPrintf.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// สร้างอ็อบเจ็กต์ของจอ LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 คอลัมน์ 2 แถว

const int ADC_PIN = A0; 
long Time_display = 0; 
float v_ref = 5.0;

unsigned long count;
unsigned long time;
int value_adc;
float voltage;
float result;  //magnetic "B" (mT)
int button;

char buffer_line_1[20];
char buffer_line_2[20];

void setup() {
  Serial.begin(19200);
  Wire.begin();      
  lcd.init();            // เริ่มต้นจอ LCD
  lcd.backlight();       // เปิดไฟแบ็คไลท์  
  lcd.setCursor(0,0);  
  lcd.print("Hi");  
}
void calculate(){
  value_adc = analogRead(ADC_PIN); 
  voltage = (value_adc*v_ref)/1023; 
  result = (voltage - 2.5731)/ (0.0027);  // สูตรคำนวณสนามแม่เหล็ก
}
void display_value(){
  count = millis();
  if(count - Time_display > 500) {
    Time_display = count;
    //printf("ADC Value: %d\n",value_adc);
    //printf("voltage: %.2f\n",voltage);
    //printf("Magnetic field : %.2f mT\n",result);
    lcd.setCursor(0, 0);
    lcd.printf(sprintf(buffer_line_1,"voltage: %.2f\n",voltage));
    lcd.setCursor(0, 1);
    lcd.printf(sprintf(buffer_line_2,"Magnetic field : %.2f mT\n",result));
  }
}
void time_shutdown(){
  time = millis();
  if(time <= 600000){ //มากกว่า 10 นาทีให้เคลียจอ
    button = 1;
    display_value();
  }
}

void loop() {
  if (button = 1){ //ปุ่มเริ่มอ่านค่า
    calculate();
    time_shutdown();
  }
  else{
    button = 0;
    lcd.setCursor(0, 0);              
    lcd.print("Do not use ,will shutsown");
    delay(1000);
    lcd.clear();
  }
}
