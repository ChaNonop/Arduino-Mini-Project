#include <Servo.h> 
#include <Keypad.h>
#include <HX711.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// การเชื่อมต่อ WiFi
const char* SSID = "non_sam";
const char* PASSWORD = "ocbr4536";

// การเชื่อมต่อ MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
char mqtt_Client[] = "cfa54c3d5-205c-4eaa-8695-non-";

WiFiClient espClient;
PubSubClient client(espClient);

// กำหนดพิน
#define LOADCELL_DOUT_PIN 13
#define LOADCELL_SCK_PIN 12

HX711 scale;
Servo myServo;
const int addr = 0x27; 
LiquidCrystal_I2C lcd(addr, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool dispensing = false;
unsigned int desiredWeight = 0;  // น้ำหนักที่ตั้งไว้สำหรับการให้อาหาร
unsigned long lastMsg = 0;
float known_weight = 1000.0;
float scale_factor;

void setup() {
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  delay(2000);
  unsigned int raw_value = scale.get_units(5);
  scale_factor = raw_value / known_weight; 
  scale.set_scale(scale_factor);

  myServo.attach(10);
  myServo.write(0);

  lcd.init();                     
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Welcome:");
  delay(2000);
  lcd.clear();
  lcd.print("Set food weight:");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client)) {
      Serial.println("connected");
      client.subscribe("/feed");  // Topic สำหรับสั่งให้ Servo ทำงาน
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "/feed" && message == "1") {  // ตรวจสอบเมื่อได้รับ logic 1
    startFeeding();
  }
}

void startFeeding() {
  if (!dispensing && desiredWeight > 0) {
    myServo.write(30);  // เปิด Servo เพื่อให้อาหาร
    dispensing = true;
    lcd.clear();
    lcd.print("Feeding...");
    delay(5000);       // รอให้ Servo ทำงาน (เวลาปรับได้ตามต้องการ)
    myServo.write(0);  // ปิด Servo หลังให้อาหารเสร็จ
    lcd.clear();
    lcd.print("Feed Complete");
    scale.tare();      // รีเซ็ตค่าน้ำหนัก
    dispensing = false;
  }
}

void handleKeypadInput(char key) {
  if (key == '*') {
    clearWeight();
  } else if (key == '#') {
    confirmWeight();
  } else if (key >= '0' && key <= '9') {
    enterWeightDigit(key);
  }
}

void clearWeight() {
  desiredWeight = 0;
  lcd.clear();
  lcd.print("Weight Cleared");
  delay(1000);
  lcd.clear();
  lcd.print("Set food weight:");
}

void confirmWeight() {
  lcd.clear();
  lcd.print("Weight Set: ");
  lcd.print(desiredWeight);
  lcd.print("g");
}

void enterWeightDigit(char key) {
  if (desiredWeight < 1000) {
    desiredWeight = desiredWeight * 10 + (key - '0');
    lcd.setCursor(0,1);
    lcd.print(desiredWeight);
    lcd.print("g   ");
  }
}

void loop() {
    if (scale.is_ready()) {
    unsigned int weight = scale.get_units(10);  // อ่านค่าเฉลี่ยจาก 10 ครั้ง
    Serial.print("น้ำหนัก: ");
    Serial.print(weight);
    Serial.println(" กรัม");
  } else {
    Serial.println("กำลังรอเซ็นเซอร์...");
  }
  delay(1000); // หน่วงเวลา 1 วินาที
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  char key = keypad.getKey();
  if (key) {
    handleKeypadInput(key);
  }

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    float currentWeight = scale.get_units(10);
    String payload = "{\"desiredWeight\":" + String(desiredWeight) + ", \"currentWeight\":" + String(currentWeight) + "}";
    client.publish("/project/weight", payload.c_str());
  }
}

