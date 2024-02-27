#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  Serial.println("Setting up ESP01...");
  delay(1000);

  sendCommand("AT+RST"); // Reset ESP01
  delay(2000);

  sendCommand("AT+CWMODE=2"); // Set WiFi mode to Station mode
  delay(1000);

  // Replace "SSID" and "PASSWORD" with your WiFi credentials
  String cmd = "AT+CWJAP=\"Redmi 9\",\"012345678\"";
  sendCommand(cmd);
  delay(5000);

  Serial.println("Connected to WiFi");
  sendCommand("AT+CWJAP?");
}

void loop() {
  // Your main code goes here
}

void sendCommand(String cmd) {
  espSerial.println(cmd);
  delay(1000);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
}
