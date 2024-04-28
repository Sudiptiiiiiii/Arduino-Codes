#include <SPI.h>
#include "MFRC522.h"
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h> 

#define SS_PIN 53
#define RST_PIN 49
#define DIR_PIN 1 //3
#define STEP_PIN 2
#define MICROSTEPS 16 // Assuming 1/16 microstepping
#define TRIGGER_PIN  16 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     17 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 
#define ERROR_LED 3    // Red LED
#define PROCESS_LED 4  // Yellow LED
#define POWER_LED 5   // Green LED
#define IN3_PIN 0     // Paper motor pin
#define IN4_PIN 14      // Paper motor pin
#define ENABLE_PIN 15  

MFRC522 mfrc522(SS_PIN, RST_PIN);
NewPing ultrasonicSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int stepsPerRevolution = 200; // Number of steps per revolution for the stepper motor

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();
  pinMode(ERROR_LED, OUTPUT);
  pinMode(PROCESS_LED, OUTPUT);
  pinMode(POWER_LED, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(POWER_LED, HIGH);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please present your card");
  digitalWrite(PROCESS_LED, LOW); // Turn off process LED
  digitalWrite(ERROR_LED, LOW);   // Turn off error LED
  delay(1000);
 
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = getCardUID();
    Serial.println(uid);
    if (isValidCard(uid)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card scanned successfully!");
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Select an option:");
      lcd.setCursor(0, 1);
      lcd.print("1. Pen");
      lcd.setCursor(0, 2);
      lcd.print("2. Pencil");
      lcd.setCursor(0, 3);
      lcd.print("3. Paper");
      char key = waitForKey();
      if (key) {
        Serial.println(key);
      }
      delay(500);
      lcd.clear();
      if (key == '1') {
        digitalWrite(PROCESS_LED, HIGH);
        lcd.print("Enter quantity:");
        int quantity = readQuantity(); // Read quantity from user input
        delay(2000); // Pause for 2 seconds
        if (quantity <= 10) {
          dispensePen(quantity);
          if (isPenDispensed()) {
            lcd.print("Pen dispensed successfully!");
          } else {
            lcd.print("Failed to dispense pen!");
          }
        
        } else {
          lcd.print("Exceeded limit!");
          delay(2000);
        }
      } else if (key == '2') {
        digitalWrite(PROCESS_LED, HIGH);
        lcd.print("Enter quantity:");
        int quantity = readQuantity(); // Read quantity from user input
        delay(2000); // Pause for 2 seconds
        if (quantity <= 10) {
          dispensePencil(quantity);
          if (isPencilDispensed()) {
            lcd.print("Pencil dispensed successfully!");
          } else {
            lcd.print("Failed to dispense pen!");
          }
        } else {
          lcd.print("Exceeded limit!");
          delay(2000);
        }
      } else if (key == '3') {
        digitalWrite(PROCESS_LED, HIGH);
        lcd.print("Enter quantity:");
        int quantity = readQuantity(); // Read quantity from user input
        delay(2000); // Pause for 2 seconds
        if (quantity <= 30) {
          dispensePaper(quantity);
        } else {
          lcd.print("Exceeded limit!");
          delay(2000);
        }
      } else {
        lcd.print("Invalid option!");
        delay(2000);
        lcd.clear();
        lcd.print("Please choose a valid option.");
        delay(2000);
      }
    } else {
      Serial.println("Access denied!");
      lcd.setCursor(0, 0);
      digitalWrite(ERROR_LED, HIGH);
      lcd.print("Invalid card! Please try again.");
      delay(3000);
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

String getCardUID() { 
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  return uid;
}

bool isValidCard(String cardUID) {
  String validCards[] = {
    " 03 42 25 fd",
    " 33 93 41 c8",
    " d3 9c a9 c9"
  };
  int validCardsLen = 3;

  for (int i = 0; i < validCardsLen; i++) {
    if (validCards[i] == cardUID) {
      return true;
    }
  }
  return false;
}

char waitForKey() {
  char key = keypad.getKey();
  while (!key) {
    key = keypad.getKey();
  }
  return key;
}

int readQuantity() {
  String quantityStr = "";
  while (true) {
    char key = waitForKey();
    if (key == '#') { // Exit loop if '#' is pressed
      break;
    }
    if (key >= '0' && key <= '9') {
      quantityStr += key;
      lcd.print(key); // Display the entered digit on the LCD
    }
  }
  return quantityStr.toInt(); // Convert string to integer
}

void dispensePen(int quantity) {
  // Dispense pen logic
  for (int i = 0; i < quantity; i++) {
    lcd.clear();
    lcd.print("Dispensing pen...");
    
    // Rotate 120 degrees clockwise
    rotateMotor(120); // Rotate motor by 90 degrees
    delay(500); // Delay for 1 second

    // Rotate 120 degrees anticlockwise
    rotateMotor(-120); // Rotate motor by -90 degrees
    delay(500); // Delay for 1 second

    // Pause after each dispense
    delay(2000);
  }
}

void dispensePencil(int quantity) {
  for (int i = 0; i < quantity; i++) {
    lcd.clear();
    lcd.print("Dispensing pencil...");
    
    // Rotate 90 degrees counterclockwise
    rotateMotor(-90); // Rotate motor by -90 degrees
    delay(1000); // Delay for 1 second

    // Rotate 90 degrees clockwise
    rotateMotor(90); // Rotate motor by 90 degrees
    delay(1000); // Delay for 1 second

    // Pause after each dispense
    delay(2000);
  }
}

void dispensePaper(int quantity) {
  for (int i = 0; i < quantity; i++) {
    digitalWrite(IN3_PIN, HIGH); // Set direction for paper dispensing
    digitalWrite(IN4_PIN, LOW);
    int speed = 150; // Adjust this value to set the speed (between 0 and 255)
    analogWrite(ENABLE_PIN, 255); // Enable the motor driver
    analogWrite(IN3_PIN, speed);  // Start the motor rotation
    delay(3000); // Adjust this delay according to your paper dispensing time
    analogWrite(IN3_PIN, 0); // Stop the motor
    analogWrite(ENABLE_PIN, 0); // Disable the motor driver
    delay(2000); // Pause after each dispense
  }
}

void rotateMotor(int angle) {
  float steps = angle / 360.0 * stepsPerRevolution * MICROSTEPS; // Calculate number of steps to rotate by the given angle
  if (angle > 0) {
    digitalWrite(DIR_PIN, HIGH); // Set direction for clockwise rotation
  } else {
    digitalWrite(DIR_PIN, LOW); // Set direction for anticlockwise rotation
  }
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(STEP_PIN, HIGH); // Generate step pulse
    delayMicroseconds(500); // Adjust speed here
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500); // Adjust speed here
  }
}

bool isPenDispensed() {
  // Measure initial distance
  unsigned int initialDistance = ultrasonicSensor.ping_cm();

  // Dispense pen (call the function that dispenses the pen)

  // Measure distance after dispensing
  unsigned int finalDistance = ultrasonicSensor.ping_cm();

  // Calculate the difference in distance
  unsigned int difference = initialDistance - finalDistance;

  // Define the range for fall detection (20-40 cm)
  unsigned int minRange = 20; // Minimum distance in cm
  unsigned int maxRange = 40; // Maximum distance in cm

  // Check if the final distance falls within the specified range
  if (finalDistance >= minRange && finalDistance <= maxRange) {
    return true; // Pen has been dispensed
  } else {
    return false; // Pen has not been dispensed
  }
}

bool isPencilDispensed() {
  // Measure initial distance
  unsigned int initialDistance = ultrasonicSensor.ping_cm();

  // Dispense pencil (call the function that dispenses the pencil)

  // Measure distance after dispensing
  unsigned int finalDistance = ultrasonicSensor.ping_cm();

  // Calculate the difference in distance
  unsigned int difference = initialDistance - finalDistance;

  // Define the range for fall detection (20-40 cm)
  unsigned int minRange = 20; // Minimum distance in cm
  unsigned int maxRange = 40; // Maximum distance in cm

  // Check if the final distance falls within the specified range
  if (finalDistance >= minRange && finalDistance <= maxRange) {
    return true; // Pencil has been dispensed
  } else {
    return false; // Pencil has not been dispensed
  }
}
