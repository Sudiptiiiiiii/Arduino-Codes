// L298 Motor Driver pins
const int ENA = 9;  // Enable A pin
const int IN1 = 8;   // Input 1 pin
const int IN2 = 7;   // Input 2 pin

// Motor pins
const int motorPin1 = 12; // Connect to OUT1 of L298
const int motorPin2 = 13; // Connect to OUT2 of L298

void setup() {
  // Set pin modes
  pinMode(ENA, OUTPUT);
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

  // Rotate motor to dispense paper
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

 // Set motor speed (PWM)
  analogWrite(motorPin1, 200); // Adjust PWM value for desired speed

  // Display motor status
  Serial.println("Dispensing paper...");

  // Delay for paper dispensing (adjust as necessary for A4 size)
  delay(5000); // Example: 5 seconds for A4 paper

  // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(motorPin1, 0);

  // Display motor status
  Serial.println("Paper dispensed.");

  // Disable motor driver until next paper dispensing
  digitalWrite(ENA, LOW);

  // Wait for the next paper dispensing action (adjust as necessary)
  delay(10000); // Example: 10 seconds delay before the next dispensing action
}