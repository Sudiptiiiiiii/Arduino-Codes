// L298 Motor Driver pins
const int ENA = 7;  // Enable A pin
const int ENB = 8;  // Enable B pin
const int IN1 = 6;   // Input 1 pin
const int IN2 = 5;   // Input 2 pin

// Motor pins
const int motorPin1 = 3; // Connect to OUT3 of L298
const int motorPin2 = 4; // Connect to OUT4 of L298

int clockwise_revolutions = 10;    // Number of clockwise revolutions
int anticlockwise_revolutions = 10; // Number of anticlockwise revolutions

void setup() {
  // Set pin modes
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Enable motor driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  // Clockwise rotation
  for (int i = 0; i < clockwise_revolutions; i++) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(motorPin1, 100); // Adjust PWM value for desired speed
    delay(1000); // Adjust delay according to the speed and size of the paper
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(motorPin1, 0);
    delay(500); // Adjust delay as necessary
  }

  // Anticlockwise rotation
  for (int i = 0; i < anticlockwise_revolutions; i++) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(motorPin1, 100); // Adjust PWM value for desired speed
    delay(1000); // Adjust delay according to the speed and size of the paper
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(motorPin1, 0);
    delay(500); // Adjust delay as necessary 
  }

  // Disable motor driver until next paper dispensing
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);

  // Wait for the next paper dispensing action (adjust as necessary)
  delay(10000); // Example: 10 seconds delay before the next dispensing action
}
 