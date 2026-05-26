#include <Arduino.h>
#include <Wire.h>
#include "PCF8574.h"

class Operator{
  public:
  const byte Opearator_Pin[5]={};
  const byte led_operator[2][5] = {
    {1,1,1,1,1}, // บวก led matrix
    {1,0,1,0,1}  // ลบ  led matrix
  }
  bool Plus_minus = false;
  void led_operator(){
    msg = millis();
    if(Plus_minus == True){
      for(int x =0; x<5; x++){
        digitalWrite(Opearator_Pin[x],led_operator[1][5]);
      }
    }
    else{
      for(int x=0; x<5; x++){
        digitalWrite(Opearator_Pin[x],led_operator[0][5]);
      }
    }
  }
};
class Pin_led ::Button {
  public:
  const byte latchPin[3] = {PA6,PB12,PA12};  // ต่อ Pin 12 ของ shift register
  const byte clockPin[3] = {PB0,PB14,PA11};  // ต่อ Pin 11 ของ shift register
  const byte dataPin[3] = {PA7,PB13,PA15};   // ต่อ Pin 14 ของ shift register
  const byte INT = PB1;

  const uint8_t Num_Anode_Hex[12] = {
  0x03, // เลข 0
  0x9F, // เลข 1
  0x25, // เลข 2
  0x0D, // เลข 3
  0x99, // เลข 4
  0x49, // เลข 5
  0x41, // เลข 6
  0x1F, // เลข 7
  0x01, // เลข 8
  0x09, // เลข 9
  0xFD, // ขีด
  0xFF // blanlk
  };

  //ใช้สำหรับ 7segment ของ hint
  const byte digit_Pin[4] = {PA10, PA9, PA8, PA15}; 
  const byte LED_state = PB4;

  // LED delay
  unsigned long lasttime_led = 0;
  const long led_Delay = 8;

  private:
  //Pin ตัวบวก ลบ คูณ หาร
  
  //Pin Answer
  const byte latch_Pin = &latchPin[0];
  const byte clock_Pin = &clockPin[0];
  const byte data_Pin = &dataPin[0];

  //Pin led matrix
  const byte latchPin_matrix = &latchPin[1];
  const byte clockPin_matrix = &clockPin[1];
  const byte dataPin_matrix = &dataPin[1];
  
  //Pin hint led
  const byte latchPin_hint = &latchPin[3];
  const byte clockPin_hint = &clockPin[3];
  const byte dataPin_hint = &dataPin[3];

  uint8_t num_digit = 0;
  uint8_t count[4]={0};
  uint8_t count_2[2]={0};

  int Answer = 0;

  void state_logic(){
    Button.processButtons();

    if(state_button[0] == 1){ //ปู่ม 1
      count[num_digit]++;

      if(count[num_digit] > 9){
        count[num_digit] = 0;
      }
      Operator.plus_minus++;
    }
    if(state_button[1] == 1){ //ปู่ม 2
      count[num_digit]--;

      if(count[num_digit] < 0){
        count[num_digit] = 9;
      }
      Operator.plus_minus--;
    }
    if(state_button[2] == 1){ //ปู่ม 3
      if(num_digit == 3 ){
        Operator.plus_minus != Operator.plus_minus;
      }
      else if(num_digit > 4){
        num_digit = 0;
      }
      else{
        num_digit++;
      }
    }
    if(state_button[3] == 1){
      calculate(); // ให้โชว์คำตอบที่คำนวณ
    }
    
    // ==ไบ้คำตอบ==
    if(state_button[4] == 1){

    }
    if(state_button[5] == 1){

    }
    if(state_button[6] == 1){

    }
    if(state_button[7] == 1){

    }
  }

  void calculate(bool *value){
    int value_main(const uint8_t *count) {
      int main_num = 0;
      uint8_t n = 2;
      for (size_t i = 0; i < n; i++) {
          main_num = main_num * 10 + count[i];
      }
      return main_num;
    }
    int value_second(const uint8_t *count) {
      int second_num = 0;
      uint8_t n = 4;
      for (size_t i = 2; i < n; i++) {
          second_num = second_num * 10 + count[i];
      }
      return second_num;
    }

    if(Operator.plus_minus == 1){
      Answer = value_main + Value_second;
    }
    else if(Operator.plus_minus == 0){
      Answer = value_main - Value_second;
    }
    else(){

    }
    int* Answer_adr = Answer;
  }
  
    // Display Function
  void Display_num(*Answer_adr) {
    static int Answer[4];
    // แยกตัวเลขเตรียมไว้ 
    Answer[0] = count_num % 10;
    Answer[1] = (count_num / 10) % 10;

    Answer[2] = count_num % 10;
    Answer[3] = (count_num / 10) % 10;

    unsigned long msg = millis();
    if (msg - lasttime_led >= led_Delay) {
      turnoff_digit();

      byte num = Answer[currentDigit];
      byte dataSend = pgm_read_byte(&Num_cathode_Hex[num]);

      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, dataSend);
      digitalWrite(latchPin, HIGH);
      digitalWrite(digit_Pin[currentDigit], HIGH);
      currentDigit = (currentDigit + 1) % 4;
    }
  }
  void Display_hint(){
    
  }
};

class Button{
  public:
  const byte switch_mode_Pin = PA2;
  const bool switch_mode_state = false;
  const byte button_Pin_lcd[4] = {PC14,PC15,PA0,PA1};
  PCF8574 pcf(Addr_button);
  byte button_i2c = 0xFF; // 0xFF = 0b11111111 สถานะเริ่มต้นคือไม่กด

  // button I2c config
  const byte Addr_button 0x20;

  // status variable
  unsigned long msg;
  const long debounceDelay = 100;
  
  volatile bool pcf_state = false;
  byte state_button[8] = {0};
  
  void pcf_check(){
    detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));
    bool val = pcf.read8();
    unsigned long now = millis();

    if (now - msg >  debounceDelay){
      msg = now;
      processButtons(&val);
    }
    else{
      return;
    }
    pcf_state = LOW;
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));
  }
 
  void processButtons(bool *stabledata){
    if(stabledata == button_i2c) return;
    for (int i = 0; i < 8; i++) {
      if((stabledata &(1<<i)) == 0){
        state_button[i] = 1; //มีการกดปุ่ม
      }
      else{
        state_button[i] = 0; // ไม่มีการกดปุ่ม
      }
    }
    return state_button;
  }

  private:
}


void setup() {
  Serial.begin(115200);
  wire.begin();

  if (!pcf.begin()) {
    Serial.println("Error: PCF8574 not found!");
    while (1);
  }
  for (uint8_t i = 0; i <= 6; i++) {
    pcf.pinMode(i, INPUT); 
  }
  // ขา P7 ไม่ได้ใช้ (จะปล่อยเป็น Input หรือ Output ก็ได้)
  // pcf.pinMode(P7, INPUT);

  for(int i = 0; i < 5; i++) {
    pinMode(latchPin[i], OUTPUT);
  }
  for(int i = 0; i < 4; i++) {
    PinMode(clockPin[i], OUTPUT);
    pinMode(dataPin[i], OUTPUT);
  } 
  for(int i = 0;i < 4; i++){
    pinMode(button_Pin_lcd[i], INPUT_PULLUP);
  }
  for(int i = 0;i < 5; i++){
    pinMode(Operator_Pin[i],OUTPUT);
  }

  pinMode(Button.INT,INPUT);
  attachInterrupt(digitalPinToInterrupt(INT),Button.pcf_check,FALLING);
  pinMode(Button.switch_mode_Pin,INPUT_PULLUP);
}

void loop() {

}

void button_check() {
  unsigned long now = millis();
  if (now - msg >  debounceDelay && buttonPressed[i] == TRUE) {
    msg =  now;
    if (buttonPreeed[0] == true){

    }
    if (buttonPreeed[1] == true){

    }
    if (buttonPreeed[2] == true){
      
    }
    if (buttonPreeed[2] == true){
      
    }
    PCF8574::ButtonValues values = pcf.digitalReadALL();
    ButtonPreesed[i] = false;
  }
}
void function_button(){
  if(values.p0 == LOW){
    if(state_digit[i]){
      count[i]++;
    }
  }
  if(values.p3 == LOW){
    status[i++] = 1;
    if(size0f(status_digit)>4){
      status_digit[i] = 0;
    }
    if(state)
  }
}


void Display_matrix(){

}
void Display_hint(){

}