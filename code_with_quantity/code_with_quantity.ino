#include <SPI.h>
#include "MFRC522.h"
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 53
#define RST_PIN 49
#define STEP_PIN 2
#define DIR_PIN 3


MFRC522 mfrc522(SS_PIN, RST_PIN);

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

int in3 = 8;
int in4 = 9;
int speedPin = 10; // Assuming PWM pin for speed control

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();
  
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(speedPin, OUTPUT);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Show your card");
  delay(1000);

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = getCardUID();
    Serial.println(uid);
    if (isValidCard(uid)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scanned successfully!!!");
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Choose option :");
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
      lcd.print("Enter quantity:");
      int quantity = readQuantity(); // Read quantity from user input
      delay(2000); // Delay for 2 seconds
      if (key == '1') {
        dispensePen(quantity);
      } else if (key == '2') {
        dispensePencil(quantity);
      } else if (key == '3') {
        dispensePaper(quantity);
      }
    } else {
      Serial.println("Access denied!");
      lcd.setCursor(0, 0);
      lcd.print("Scan card is incorrect!");
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
  for (int i = 0; i < quantity; i++) {
    lcd.clear();
    lcd.print("Pen coming");
    digitalWrite(STEP_PIN, HIGH);
    delay(5000);
    digitalWrite(STEP_PIN, LOW);
    delay(2000); // Delay after each dispense
  }
}

void dispensePencil(int quantity) {
  for (int i = 0; i < quantity; i++) {
    lcd.clear();
    lcd.print("Pencil coming");
    digitalWrite(STEP_PIN, HIGH);
    delay(5000);
    digitalWrite(STEP_PIN, LOW);
    delay(2000); // Delay after each dispense
  }
}

void dispensePaper(int quantity) {
  for (int i = 0; i < quantity; i++) {
    lcd.clear();
    lcd.print("Paper coming");
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    int speed = 150; // Adjust this value to set the speed (between 0 and 255)
    analogWrite(speedPin, speed);
    delay(5000); // Adjust this delay according to your paper dispensing time
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(2000); // Delay after each dispense
  }
}
