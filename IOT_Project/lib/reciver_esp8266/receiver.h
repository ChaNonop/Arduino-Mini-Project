#ifndef RICEVER_H
#define RICEVER_H

#include <Arduino.h>
#include "config.h"

struct SensorPacket {
    byte nodeId;      // ID ประจำตัวโหนด
    float temperature;   // อุณหภูมิ
    float humidity;      // ความชื้น
    uint16_t pressure;   // ความดัน
    uint32_t timestamp;  // เวลาทำงาน
};

class sensor {
private:
    byte _PIN;
    byte _type;
    DHT _dht;

    //==== Pin config ======
    byte  I2c;
    const byte soundPin;
    const byte buzzzlePin;

    unsigned long _time;
    unsigned long _lastRead;
    float _humidity;
    float _temperature;
    float _lux;
public:
    sensor(); //defalt constructor
    bool begin();
    bool update();

    // Getter
    float temperature() const;
    float humidity() const;
    float lux() const;
};

class Dat_management {
    private:

    public:
        Wifi();

};
class Display{
    public:
    // ==== Oled config ====
    byte SCREEN_WIDTH = 128;
    byte SCREEN_HEIGHT = 64;
    byte OLED_RESET = 4;
    byte SCREEN_ADDRESS = 0x3C;
}

#endif



#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <RH_ASK.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

struct SensorPacket {
    uint8_t nodeId;      // ID ประจำตัวโหนด
    float temperature;   // อุณหภูมิ
    float humidity;      // ความชื้น
    uint16_t pressure;   // ความดัน
    uint32_t timestamp;  // เวลาทำงาน
};


class SensorReceiver {
private:
    RH_ASK rf_driver;
    uint8_t _rxPin;

public:
    // Constructor
    SensorReceiver(uint8_t pin);

    // เริ่มต้นการทำงาน
    bool begin();

    // ตรวจสอบและดึงข้อมูล (Pass by Reference เพื่อประหยัด Memory)
    bool update(SensorPacket &outputData);
};


class Wifi {
  public :
  const char* ssid ="Chanon";
  const char* password = "03143681";
 
  // MQTT broker settings
  const char* mqtt_server = "broker.netpie.io";
  const int mqtt_port = 1883;
  const char* mqtt_Client = "47eb28e0-0fe7-4eed-b90f-b86cd49baa38";
  const char* mqtt_username = "tVgNN3VMTFJzBEYoyj5pVEj23aM9Pc3c";
  const char* mqtt_password = "x6pR5J5fFmRHLDAonsigbBWozLd33oZpH";
  WiFiClient espClient;
  PubSubClient client(espClient);

  long lastMsg = 0;
  char msg[100];

  Wifi (){ //defalt constructor
  };

  // Function to reconnect to MQTT broker
  bool reconnectMQTT() {}

  // Callback function for รับข้อความจาก mqtt แล้วทำไงต่อ
  void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
  }
  // Function to publish sensor data to MQTT
  void publishSensorData(float temp, float humidity, float Lux) {
    snprintf(msg, sizeof(msg), "{\"data\":{\"temperature\":%.2f,\"humidity\":%.2f,\"lux\":%.2f}}", temp, humidity, Lux);
    Serial.print("Publishing message: ");
    Serial.println(msg);
    client.publish("@shadow/data/update", msg);
  }
};

class sensor{
  private:
  
  public:
};
 
class riceiver{
  private:

  RH_ASK rf_driver(2000,D2,D1,0); // rf riciver

  void setup() {
    Serial.begin(115200);

    if (!rf_driver.init()) {
      Serial.println(F("RF driver init failed!"));
    } else {
      Serial.println(F("RF driver init success."));
    }
  }
  public:

  int data(){

  }
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  // Check if a message has been received.
  if (rf_driver.recv(buf, &buflen)) {
    // Print the message if it is received.
    Serial.print(F("Message Received: "));
    Serial.println((char*)buf);
  }
};

class SensorReceiver {
private:
    RH_ASK driver;
    uint8_t _rxPin;

public:
    // Constructor
    SensorReceiver(uint8_t pin);

    // เริ่มต้นการทำงาน
    bool begin();

    // ตรวจสอบและดึงข้อมูล (Pass by Reference เพื่อประหยัด Memory)
    bool update(SensorPacket &outputData);
};

#endif