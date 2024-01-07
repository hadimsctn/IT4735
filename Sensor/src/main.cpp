#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Wire.h>
#include <MFRC522.h> // Thư viện RFID-RC522
#include <ESP32Servo.h>
#include <ArduinoJson.h>

#define SDA_PIN 21 // Chọn chân GPIO tương ứng với chân SDA
#define RST_PIN 22 // Chọn chân GPIO tương ứng với chân RST

MFRC522 mfrc522(SDA_PIN, RST_PIN); // Sử dụng thư viện RFID-RC522
WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo;
const char *ssid = "Trangbeo"; // Enter your WiFi name
const char *password = "ngocctrang99@";
const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_username = "nguyenha";
const char *mqtt_password = "12345678";
const char *topicRfid = "Sensor/RFID";
const char *topicDoor = "Sensor/Door";
const int mqtt_port = 1883;

// void parseJsonPayloadUpdateName(String jsonStr)
// {
//   DynamicJsonDocument doc(8192); // Kích thước đối tượng JSON tương ứng với payload

//   // Phân tích chuỗi JSON
//   deserializeJson(doc, jsonStr);

//   // Trích xuất giá trị từ các trường JSON
//   const char *idDevice = doc["IdDevice"];

//   // Xử lý dữ liệu
//   JsonArray idDeviceArray = doc["IdDevice"];
//   for (const auto &value : idDeviceArray)
//   {
//     if (value == "string")
//     {
//       const char *name = doc["Name"];
//       updateName(String(name));
//       break; // Nếu đã tìm thấy giá trị "string", thoát khỏi vòng lặp
//     }
//   }
// }

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  if (strcmp(topic, topicDoor)==0){
    payload[length] = '\0';
    String jsonStr = String((char *)payload);
    myServo.write(0); // Đưa Servo về góc 90 độ     // Chờ 1 giây
    delay(3000);
    myServo.write(90);
    //parseJsonPayloadOpenDoor(jsonStr);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(4096);
  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Public emqx mqtt broker connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(topicDoor);
  myServo.attach(13);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Waiting for an RFID card...");
}

void loop()
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    Serial.println("Card detected!");

    Serial.print("UID Value: ");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    client.publish(topicRfid,"test");
    Serial.println();
    mfrc522.PICC_HaltA();
    Serial.println("Waiting for an RFID card...");
  }
  client.loop();
}
