#include <SPI.h>
#include "MFRC522.h"
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 53
#define RST_PIN 49

#define STEP_PIN 2  // Connect to the STEP pin on the motor driver
#define DIR_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);
// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,12,11,10};
byte colPins[COLS] = {9,8,7,6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// LCD Setup for 20x4 I2C
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adjust the I2C address if necessary

String getCardUID();
bool isValidCard(String x);

String getCardUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  return uid;
}

bool isValidCard(String cardUID) {
  String validCards[]={
    " 03 42 25 fd",
    " 33 93 41 c8",
    " d3 9c a9 c9"
  };
  int validCardsLen = 3;
  // Implement your validation logic here, e.g., check against a database
  // Dummy example: Allow access if the card UID is "01 23 45 67"

  for(int i=0; i<validCardsLen; i++){
    if(validCards[i] == cardUID){
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

 // LCD Setup
  lcd.init();
  lcd.backlight();

  // motor setup
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}   

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
  {
    String uid = getCardUID();
    Serial.println(uid);
    lcd.setCursor(0,0);
    lcd.print("show your card");

    if (isValidCard(uid)) 
    {
      // Serial.println("Access granted!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scanned successfully!!!");
      
      // lcd.print("Access Granted");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose option :");
      lcd.setCursor(0, 1);

      lcd.print("1.Pen");
      lcd.setCursor(0, 2); 
      lcd.print("2.Pencil");
      // Perform actions corresponding to valid access
      // Rotate the motor clockwise
      char key = keypad.getKey();
      if (key == '1') 
      {
        // Handle option 1 - Pen
        // Add your logic here
        lcd.clear();
        lcd.print("Pen Selected");
        digitalWrite(DIR_PIN, HIGH);
        for (int i = 0; i < 2500; i++) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(300);  // Reduce delay for faster speed
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(300);
        } 
        delay(500);  // Pause for 1 second
      }
      else if (key == '2') 
      {
        // Handle option 2 - Pencil
        // Add your logic here
        lcd.clear();
        lcd.print("Pencil Selected");
        // Rotate the motor counterclockwise
        digitalWrite(DIR_PIN, LOW);
        for (int i = 0; i < 2500; i++) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(300);  // Reduce delay for faster speed
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(300);  // Reduce delay for faster speed
        }

        delay(500);  // Pause for 1 second

          // Reduce delay for faster speed
      }      
     else {
      // Serial.println("Access denied!");
      // Perform actions corresponding to invalid access
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scanned Unsuccessful!!!");
    }
  }
      // Halt the card
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
}


