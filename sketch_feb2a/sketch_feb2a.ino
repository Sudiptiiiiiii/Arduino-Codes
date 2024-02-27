#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* server = "YourServerIPAddress";  // Replace with your server IP address or domain
const int port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (client.connected()) {
    // Perform your regular tasks here

    // Example: Send a request to the server
    client.println("GET / HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();
    
    // Read the server response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    // Wait for a moment
    delay(5000);
  } else {
    // If the client is not connected, reconnect
    Serial.println("Lost connection to the server. Reconnecting...");
    connectToWiFi();
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  
  // Establish a connection to the server
  if (client.connect(server, port)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
  }
}

