#define STEP_PIN 2   // Connect to the STEP pin on the motor driver
#define DIR_PIN 3    // Connect to the DIR pin on the motor driver

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  // Rotate the motor clockwise
  digitalWrite(DIR_PIN, HIGH);
  for (int i = 0; i <2500; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(300);  // Reduce delay for faster speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(300);  // Reduce delay for faster speed
  }

  delay(500);  // Pause for 1 second

  // Rotate the motor counterclockwise
  digitalWrite(DIR_PIN, LOW);
  for (int i = 0; i < 2500; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(300);  // Reduce delay for faster speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(300);  // Reduce delay for faster speed
  }

  delay(500);  // Pause for 1 second
}
