#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_ASK.h>
#include <PubSubClient.h>
#include "DHT.h"

#include "receiver.h"

sensor::sensor(){
    : _pin(pin),
    _type(type),
    _dht(pin, type),
    _time(time),
    _lastRead(0),
    _humidity(NAN),
    _temperature(NAN),
}

bool sensor::begin(){
    _dht.begin(_pin, _type);
    return true;
}

bool sensor::update(){
    unsigned long now = millis();
    if (now - _lastRead >= _time) {
        _lastRead = now;
        float h = _dht.readHumidity();
        float t = _dht.readTemperature();

        if (isnan(h) || isnan(t)) { //ตรวจสอบค่า
            return false;
        } else {
            _humidity = h;
            _temperature = t;
            return true;
        }
    }
}

ิfloat sensor::temperature() const {
    return _temperature;
}

float sensor::humidity() const {
    return _humidity;
}

float sensor::lux() const {
    return _lux;
}




#include "receiver.h"

// Constructor Implementation
SensorReceiver::SensorReceiver(uint8_t pin) 
    : rf_driver(2000, pin, -1), _rxPin(pin) {
}

bool SensorReceiver::begin() {
    if (!rf_driver.init()) {
      Serial.println(F("RF driver init failed!"));
    } else {
      Serial.println(F("RF driver init success."));
    }
    return rf_driver.init();
}

// Constructor Implementation
Wifi::Wifi() {

}

bool Wifi::reconnectMQTT(){
  while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
        Serial.println("connected");
        client.subscribe("@msg/operator");
      } else {
        Serial.print("Failed, rc=");
        Serial.print(client.state());
        Serial.println(" trying again in 5 seconds");
        delay(5000);
      }
    }
}
