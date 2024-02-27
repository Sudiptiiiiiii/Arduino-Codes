// YouTube|Tech at Home

int in3 = 8;
int in4 = 9;
int speedPin = 10; // Assuming PWM pin for speed control

void setup() {
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(speedPin, OUTPUT);
}

void loop() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Set the speed using analogWrite. 0 is full stop, 255 is full speed.
  int speed = 150; // Adjust this value to set the speed (between 0 and 255)
  analogWrite(speedPin, speed);
}
