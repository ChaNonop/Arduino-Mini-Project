#include "config.h"
#include "receiver.h"
#include "Transmit.h"



 sensor dht(4, DHT11, 2000); 
// #define TRANSMITTER_rp2040  // เลือกบรรทัดนี้ถ้าจะ Flash ลง Pi Pico
#define RECEIVER_esp8266     // เลือกบรรทัดนี้ถ้าจะ Flash ลง ESP8266


#ifdef TRANSMITTER_rp2040
    #include "Transmit.h"
    // Pi Pico: ขา Data ต่อ GP0
    SensorTransmitter tx(0, 1); // Pin 0, Node ID 1

    void setup() {
        Serial.begin(115200);
        if (tx.begin()) {
            Serial.println("TX Ready (Pi Pico)");
        } else {
            Serial.println("TX Init Failed");
        }
    }

    void loop() {
        // จำลองค่า Sensor (Simulation)
        // ในงานจริง: อ่านค่าจาก Sensor ตรงนี้
        float temp = 25.0 + (rand() % 100) / 10.0;
        float hum = 60.0 + (rand() % 200) / 10.0;
        uint16_t pres = 1013;

        Serial.printf("Sending: T=%.2f H=%.2f P=%d\n", temp, hum, pres);
        tx.sendData(temp, hum, pres);
        
        delay(2000); // ส่งทุก 2 วินาที (Duty Cycle)
    }

#elif defined(RECEIVER)
    #include "Receiver.h"
    // ESP8266: ขา Data ต่อ D2 (GPIO 4)
    SensorReceiver rx(D2); 
    SensorPacket data;

    void setup() {
        Serial.begin(115200);
        if (rx.begin()) {
            Serial.println("RX Ready (ESP8266)");
        } else {
            Serial.println("RX Init Failed");
        }
    }

    void loop() {
        // Polling รับค่าตลอดเวลา
        if (rx.update(data)) {
            Serial.printf("Received from Node %d: T=%.2f H=%.2f P=%d Time=%u\n", 
                data.nodeId, 
                data.temperature, 
                data.humidity, 
                data.pressure, 
                data.timestamp
            );
        }
    }

#endif



// class Wifi {
//   public :
//   long lastMsg = 0;
//   char msg[100];

//   Wifi (){ //defalt constructor

//   };

//   // Function to reconnect to MQTT broker
//   void reconnectMQTT() {
//     while (!client.connected()) {
//       Serial.print("Attempting MQTT connection...");
//       if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
//         Serial.println("connected");
//         client.subscribe("@msg/operator");
//       } else {
//         Serial.print("Failed, rc=");
//         Serial.print(client.state());
//         Serial.println(" trying again in 5 seconds");
//         delay(5000);
//       }
//     }
//   }
//   // Callback function for รับข้อความจาก mqtt แล้วทำไงต่อ
//   void callback(char* topic, byte* payload, unsigned int length) {
//   String message;
//   for (unsigned int i = 0; i < length; i++) {
//     message += (char)payload[i];
//   }

//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("]: ");
//   Serial.println(message);
//   }
//   // Function to publish sensor data to MQTT
//   void publishSensorData(float temp, float humidity, float Lux) {
//     snprintf(msg, sizeof(msg), "{\"data\":{\"temperature\":%.2f,\"humidity\":%.2f,\"lux\":%.2f}}", temp, humidity, Lux);
//     Serial.print("Publishing message: ");
//     Serial.println(msg);
//     client.publish("@shadow/data/update", msg);
//   }
//   void begin(){
//     WiFi.begin(config.getWiFiSSID(), config.getWiFiPassword());
    
//     mqtt.setCredentials(
//         config.getMQTTToken(),
//         config.getMQTTSecret()
//   } 
// };
// class sensor{
//   private:
  
//   public:
// };
 
// class riceiver{
//   private:

//   RH_ASK rf_driver(2000,D2,D1,0); // rf riciver

//   void setup() {
//     Serial.begin(115200);
    
//     if (!rf_driver.init()) {
//       Serial.println(F("RF driver init failed!"));
//     } else {
//       Serial.println(F("RF driver init success."));
//     }
//   }
//   public:

//   int data(){

//   }
//   uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//   uint8_t buflen = sizeof(buf);

//   // Check if a message has been received.
//   if (rf_driver.recv(buf, &buflen)) {
//     // Print the message if it is received.
//     Serial.print(F("Message Received: "));
//     Serial.println((char*)buf);
//   }
// };


// void setup() {
//   Serial.begin(115200);
  
//   connectToWiFi(); // Connect to WiFi
//   client.setServer(mqtt_server, mqtt_port); // Set MQTT server
//   client.setCallback(callback); // Set MQTT callback
// }

// void loop() {
//   printf("Hello World x= %d y = %d\n", x, y);
//   delay(1000);
// }



//================================================
// const char* ssid = "Somwang";
//  //การประกาศสร้างตัวแปรเก็บ Username เครื่อข่าย WiFi ชื่อว่า ssid 
 
// const char* pass = "03143681";
//  //การประกาศสร้างตัวแปรเก็บ Password ของเครื่อข่าย WiFi ชื่อว่า pass

// void setup () {
//   Serial.begin(115200); //
//   WiFi.begin(ssid, pass); //ทำการ Connect SSID และ Pass
// while (WiFi.status() != WL_CONNECTED) { // ถ้าไม่สามารถเชื่อมต่อได้
//     // ทำการ Print "Connectiong..." ทุก 1000ms
//     Serial.println("Connecting...  ");  
//     // แสดงสถานะการเชื่อมต่อ
//     Serial.printf("Connection Status: %d\n", WiFi.status()); 
//     delay(1000);
//   }
//   // จะหลุดออกจาก while ก็ต่อเมือ Connected เรียบร้อย
//   Serial.print("Wi-Fi connected."); 
//   Serial.print("IP Address : ");
//   Serial.println(WiFi.localIP());  // ทำการ Print IP ที่ได้รับมาจาก 
// }
// void loop () { 
//   Serial.printf("Connection Status: %d\n", WiFi.status());// แสดงสถานะการเชื่อมต่อ
//   delay(2000);
// }






// ///==============================================
// void setup() {
//   Serial.begin(115200);
//   Serial.println(F("\nESP8266 WiFi scan example"));

//   // Set WiFi to station mode
//   WiFi.mode(WIFI_STA);

//   // Disconnect from an AP if it was previously connected
//   WiFi.disconnect();
//   delay(100);
// }

// void loop() {
//   String ssid;
//   int32_t rssi;
//   uint8_t encryptionType;
//   uint8_t *bssid;
//   int32_t channel;
//   bool hidden;
//   int scanResult;

//   Serial.println(F("Starting WiFi scan..."));

//   scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

//   if (scanResult == 0) {
//     Serial.println(F("No networks found"));
//   } else if (scanResult > 0) {
//     Serial.printf(PSTR("%d networks found:\n"), scanResult);

//     // Print unsorted scan results
//     for (int8_t i = 0; i < scanResult; i++) {
//       WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

//       // get extra info
//       const bss_info *bssInfo = WiFi.getScanInfoByIndex(i);
//       String phyMode;
//       const char *wps = "";
//       if (bssInfo) {
//         phyMode.reserve(12);
//         phyMode = F("802.11");
//         String slash;
//         if (bssInfo->phy_11b) {
//           phyMode += 'b';
//           slash = '/';
//         }
//         if (bssInfo->phy_11g) {
//           phyMode += slash + 'g';
//           slash = '/';
//         }
//         if (bssInfo->phy_11n) {
//           phyMode += slash + 'n';
//         }
//         if (bssInfo->wps) {
//           wps = PSTR("WPS");
//         }
//       }
//       Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %-11s %3S %s\n"), i, channel, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : '*', hidden ? 'H' : 'V', phyMode.c_str(), wps, ssid.c_str());
//       yield();
//     }
//   } else {
//     Serial.printf(PSTR("WiFi scan error %d"), scanResult);
//   }

//   // Wait a bit before scanning again
//   delay(5000);
// }


// RH_ASK rf_driver(1000,D2,D1,0);

// void setup() {
//   Serial.begin(115200);

//   if (!rf_driver.init()) {
//     Serial.println(F("RF driver init failed!"));
//   } else {
//     Serial.println(F("RF driver init success."));
//   }
// }

// void loop() {
//   uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//   uint8_t buflen = sizeof(buf);

//   // Check if a message has been received.
//   if (rf_driver.recv(buf, &buflen)) {
//     // Print the message if it is received.
//     Serial.print(F("Message Received: "));
//     Serial.println((char*)buf);
//   }
// }


// RH_ASK rf_driver(1000, 11);

// void setup() {
//   Serial.begin(115200);

//   if (!rf_driver.init()) {
//     Serial.println(F("RF driver init failed!"));
//   } else {
//     Serial.println(F("RF driver init success."));
//   }
// }

// void loop() {
//   uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//   uint8_t buflen = sizeof(buf);

//   // Check if a message has been received.
//   if (rf_driver.recv(buf, &buflen)) {
//     // Print the message if it is received.
//     Serial.print(F("Message Received: "));
//     Serial.println((char*)buf);
//   }
// }