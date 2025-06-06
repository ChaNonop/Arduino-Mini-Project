#include <Wifi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

class ADCReader_sensor {
  private:
    float h;
    float t;

    char msg[100];

    int temp = 0;

    String DataString;
    #define DHTPIN 2  
    #define DHTTYPE DHT11
    #define LED_PIN 13 //ขาที่ต่อกับ LED

    const int adcPin;
    const int sampleCount;
    const int timeRead;
    long now;
    unsigned long lastMsg;
    const int adcSamples[5];
    int sampleIndex;
    
  public:

    void time_send{
      now = millis();
      if (now-lastMsg >= 1000){ //จับเวลาส่งข้อมูลทุก ๆ 10 วินาที
        lastMsg = now;
        temp_humudity();
      }
    }
    void temp_humudity(){
      float h =dht.readHumidity();
      float t =dht.readTemperature();
      DataString = "{}"
      DataString.toArrays(msg,100);
      client.publish("/non/dht/temp",msg); // อย่าลืมแก้ไข TOPIC ที่จะทำการ PUBLISH ไปยัง MQTT BROKER
    }
    void Brightness(){
      temp =analogRead(A0);

    }
}

class setup_wifi{
  private:
    const char*ssid ="Somwang";
    const char*password ="03143681";
    const char*mqtt_server ="broker.hivemq.com";
    const int mqtt_port = 1883;
    const char* mqtt_Client = "cfa54c3d5-205c-4eaa-8695-non-";
    const char* mqtt_username = "";  //อย่าลืมแก้ไข Token
    const char* mqtt_password = ""; //อย่าลืมแก้ไข Secret
  public:
    void reconnect(){
      while(!client.connect()){
        lcd.setCursor(0,0);
        lcd.print("MQTT connect....");
        if(client.connect(mqtt_Client, mqtt_username, mqtt_password)){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Connected");
          Client.subscribe("/non");
        }
        else{
          lcd.clear();
          lcd.print("failed, rc=");
          lcd.print(client.state());
          lcd.setCursor(0,1);
          lcd.print("try again wait..");
          delay(5000);
        }
      }
    }
    void callback(char*topic, byte* payload, unsigned int length){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message :");
      lcd.print(topic);
    }
}

class On_Off{
  private:

}

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

WiFiClient espClient; 
PubSubClient client(espClient);

void setup() {
  Wire.begin(4,5); // esp8266 GPIO4 = D1 = SCL, GPIO5 = D2c = SDA
  pinMode(LED_PIN,OUTPUT);
  dht.begin();
  lcd.int();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Connecting to :%s",ssid);
  WiFi.begin(ssid, password); //เชื่อมต่อกับ WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi connected");
  lcd.setCursor(0,1); //display บรรทัดที่ 2
  lcd.print("IP address: ");
  lcd.setCursor(12,1); 
  lcd.println(WiFi.localIP()); //เชื่อมต่อกับ WIFI สำเร็จ แสดง IP
  client.setCallback(callback); //ตั้งค่าฟังก์ชันที่จะทำงานเมื่อมีข้อมูลเข้ามาผ่านการ Subscribe
  client.subscribe("/non");
}

void loop() {
    if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
