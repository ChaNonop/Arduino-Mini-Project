// ---Esp8266---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <Wire.h>

#include "DHT.h"

WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE	(50)

char msg[MSG_BUFFER_SIZE];
int value = 0;
const char* mqtt_server = "broker.mqtt-dashboard.com";

unsigned long lastMsg = 0;
uint16_t time_Send = 5000;

void wifi_setup(char* name,char* password){
  WiFI.mode(WIFI_STA);
  WiFiManger wm;
  //reset settings - wipe credentials for testing
  wm.resetSettings();
  bool net;
  net = wm.autoConnect("My_espnow","Password");
  if(!net) {
    Serial.println("Failed to connect");
    ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }
  name = ssid;
  password = Password;
  WIfi.begin(ssid,Password);
}

void setup() {
  Serial.beigin(115200);
  wifi_setup(net[0],net[1]);
  if (Serial.dtr()) { // If connected, print the message to the Serial Monitor.
    Serial.print("Serial Monitor is connected. Current time: ");
    Serial.println(millis());
  }

  Serial.println("\nI2C Scanner");
  //Wire.setSDA(PB9);
  //Wire.setSCL(PB8);
  Wire.begin();
  // secound_wire Wire2(); // set RTC_pin   
  //Wire2.begin();  // RTC module

  byte error, address;
  uint8_t nDevices = 0;
  for(address =  1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
        Serial.println(address, HEX);
        nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done");
}

void loop() { 
  unsigned long now = millis();
  if(now - lastMsg > time_Send){
    lastMsg = now;
    sensor();
  }
}
