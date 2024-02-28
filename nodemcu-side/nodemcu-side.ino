#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Redmi";
const char* password = "12345678";
const char* serverAddress = "http://127.0.0.1:8000/check-availability/";
String keypadInput;

void connectWiFi();
void sendHttpRequest(String);

void setup(){
  Serial.begin(115200);
  connectWiFi();
}

void loop(){
  if (Serial.available()>0){
    keypadInput = Serial.readStringUntil('\n');
    sendHttpRequest(keypadInput);
  }
}

void connectWiFi(){
  Serial.println("NODEMCU Starting WiFi test...");
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendHttpRequest(String keypadInput) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(serverAddress, 80)) {
      client.print("POST /check-availability/ HTTP/1.1\r\n");
      client.print("Host: yourbackend.com\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Content-Length: ");
      client.print(keypadInput.length());
      client.print("\r\n\r\n");
      client.print(keypadInput);
      client.print("\r\n");
    } else {
      Serial.println("Connection to server failed");
    }
  } else {
    Serial.println("WiFi not connected");
  }
}