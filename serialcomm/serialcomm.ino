void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.println();
    // Process the received command
    if (command == '1') {
      digitalWrite(LED_BUILTIN, HIGH);  // Turn on the built-in LED
    } else if (command == '0') {
      digitalWrite(LED_BUILTIN, LOW);   // Turn off the built-in LED
    }
  }
}
