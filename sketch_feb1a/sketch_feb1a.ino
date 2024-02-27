#include <SoftwareSerial.h>

SoftwareSerial espSerial(0, 1); // RX, TX

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  Serial.println("Setting up ESP01...");
  delay(1000);

  sendCommand("AT+RST"); // Reset ESP01
  delay(2000);

  sendCommand("AT+CWMODE=1"); // Set WiFi mode to Station mode
  delay(1000);

  Serial.println("Scanning available WiFi networks...");
  scanWiFiNetworks();
}

void loop() {
  // Your main code goes here
}

void scanWiFiNetworks() {
  sendCommand("AT+CWLAP");

  while (espSerial.available()) {
    char c = espSerial.read();
    Serial.write(c);
  }

  Serial.println("WiFi Scan completed.");
}

void sendCommand(String cmd) {
  espSerial.println(cmd);
  delay(1000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
}
