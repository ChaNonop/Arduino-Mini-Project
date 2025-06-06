#include <Wire.h>

#define SLAVE_ADDR 127
#define ANSWER_SIZE 5
#define INTERVAL 50

class I2CMasterHandler {
  private:
    unsigned long lastMillis;
    char response[ANSWER_SIZE + 1];  // +1 สำหรับ null-terminator

  public:
    I2CMasterHandler() : lastMillis(0) {
      memset(response, 0, sizeof(response));  // เคลียร์ buffer
    }

    void begin() {
      Wire.begin();
      Serial.begin(9600);
      Serial.println("I2C Master OOD Demonstration");
    }

    void update() {
      unsigned long currentMillis = millis();
      if (currentMillis - lastMillis >= INTERVAL) {
        lastMillis = currentMillis;
        sendData();
        receiveData();
        printResponse();
      }
    }

  private:
    void sendData() {
      Serial.println("Sending data to slave...");
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(10);  // ส่งข้อมูลไปยัง Slave
      Wire.endTransmission();
    }

    void receiveData() {
      Wire.requestFrom(SLAVE_ADDR, ANSWER_SIZE);
      int i = 0;
      while (Wire.available() && i < ANSWER_SIZE) {
        response[i++] = Wire.read();
      }
      response[i] = '\0'; // Null-terminate
    }

    void printResponse() {
      Serial.print("Receive data: ");
      Serial.println(response);
    }
};

// สร้างออบเจกต์
I2CMasterHandler master;

void setup() {
  master.begin();
}

void loop() {
  master.update();
}
