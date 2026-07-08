// rp2040 easy_plane Kepp data to sd card
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

const int chipselect = 17;

void setup() {
  Serial.begin(115200);

  if(!veml.begin()){
    Serial.println("Sensor not found")
    while(1);
  }
  Serial.println("Sensor Found")
}
  while(!Serial) delay(10);

  SPI.setRX(16); //MISO
  SPI.setTx(19) // MOSI
  SPI.setSCX(18) //SCK

  if(!SD.begin(chipSelect)){
    Serial.println("failed")
    return
  }
  Serial.println("SD Card.")

  DynamicJsonDocument doc<256>;
  doc["Day"] =
  doc["Time"] = time;
  doc["Temperature"] = Temp;
  doc["humidity"] = humi;
  doc["Lux"] = Lux;
  doc["moisture"] = moi;
  doc["leveling water"] = gauge;
}

void loop() {
  // put your main code here, to run repeatedly:

}
