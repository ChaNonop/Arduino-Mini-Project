#ifndef SENSOR_H
#define SENSOR_H

#include "pins_arduino.h"
#include "DHT.h"
#include <IRremote.hpp>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
#define IR_RECEIVE_PIN 11  // IR receiver pin 11

class Sensor(){

}

#endif