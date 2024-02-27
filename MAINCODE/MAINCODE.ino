#include <SPI.h>                                          //This library is used for communication library for SPI(Serial Peripheral Interface)
#include "MFRC522.h"                                      //This library is used for the MFRC522 RFID module.
#include <Wire.h>                                         //This library communication library for I2C (Inter-Integrated Circuit).
#include <Keypad.h>                                       //This library is used for interfacing with the keypad.
#include <LiquidCrystal_I2C.h>                            //This library is used for controlling the I2C LCD.


#define SS_PIN 53                                         //-->used to select RFID module as the active device on the SPI bus
#define RST_PIN 49                                        //->used to reset pin for the RFID module
#define STEP_PIN 2                                        //-->used to control the stepping of the motor
#define DIR_PIN 3                                         //-->used to control the direction of motor

MFRC522 mfrc522(SS_PIN, RST_PIN);                         //RFID module initialization using the specifieed SS_PIN AND RST_PIN 

//for communicating
//Keypad Set with a 4x4 layout<--
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


//-->LCD Setup for 20x4 I2C<--
LiquidCrystal_I2C lcd(0x27, 20, 4);                                                         //-->0x27 denotes the I2c address of the LCD module


String getCardUID();                                                                       //-->function declaration of getting RFID CARD
bool isValidCard(String x);                                                                  //-->function declaration for checking valid card
String getCardUID() { 
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {                                                //-->loop for iterating through each byte stored in the UID
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

 int validCardsLen = 3;                                 //-->number of elements in the array of valid card
  

  for (int i = 0; i < validCardsLen; i++) {
    if (validCards[i] == cardUID) {
      return true;
    }
  }
  return false;
}

char waitForKey() {                                   //-->function to wait for a key press
  char key = keypad.getKey();
  while (!key) {                                      //-->loop continues until a valid key is pressed
    key = keypad.getKey();                            //-->continuously updating the current pressed key value
  }
  return key;
}

void setup() {
  Serial.begin(115200);                               //-->Initializes the serial communication with a baud rate of 115200 bits per second
  SPI.begin();                                        //-->Initializes the SPI (Serial Peripheral Interface) communication.
  mfrc522.PCD_Init();                                 //-->prepares rfid module for reading cards

  // LCD Setup
  lcd.init();
  lcd.backlight();                                    //turns on backlight on lcd

  // motor setup
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
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
      lcd.print("1.Pen");
      lcd.setCursor(0, 2);
      lcd.print("2.Pencil");
      char key = waitForKey();
      if (key) {
        Serial.println(key);
      }
      delay(5000);
      if (key == '1')
      {
        lcd.clear();
        lcd.print("Pen vended!");
        digitalWrite(DIR_PIN, HIGH);
        for (int i = 0; i < 2500; i++) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(300);                                 // Reduce delay for faster speed
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(300);                                 // Reduce delay for faster speed
        }
        delay(5000);
      } // Pause for 1 second

      // Rotate the motor counterclockwise
      else if (key == '2')
      {
        lcd.clear();
        lcd.print("Pencil vended!");
        digitalWrite(DIR_PIN, LOW);
        for (int i = 0; i < 2500; i++) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(300);                               // Reduce delay for faster speed
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(300);                               // Reduce delay for faster speed
        }

        delay(5000);
      }  // Pause for 1 second
    }
    else {W
      Serial.println("Access denied!");
      lcd.setCursor(0, 0);
      lcd.print("Scan card is incorrect!");
      delay(3000);
                                                              // Perform actions corresponding to invalid access
    }
    // Halt the card
    mfrc522.PICC_HaltA();                                     //-->halt communication for currently selected card
    mfrc522.PCD_StopCrypto1();                                //-->stop cryptographic communication
  }
}