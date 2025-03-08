void setup() {
  Serial.begin(19200);
}

void loop() {
  Serial.println("ESP chip ID: "+String(ESP.getChipId())); // เช็คของ esp8266
  //Serial.println("\nESP chip M0del: "+String(ESP.getChipModel()));//เช็คของesp32
  delay(500);
}
