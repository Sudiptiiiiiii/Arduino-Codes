#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

#define SS_PIN 53   //for rfid 
#define RST_PIN 49

#define STEP_PIN 2  // Connect to the STEP pin on the motor driver
#define DIR_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);
const int lcdAddress = 0x27;  // Adjust the I2C address based on your module

// Set the LCD dimensions
const int lcdColumns = 20;
const int lcdRows = 4;

// Create an LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13,12,11,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9,8,7,6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup() 
{
  Serial.begin(115200);       //Initiate serial communication
  SPI.begin();                //Initiate SPI bus
  mfrc522.PCD_Init();         //Initiate MFRC522

  // motor setup
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available on your LCD module)
  lcd.backlight();
}   

void loop() 
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())   //look for cards and select the cards
  {
    String uid = getCardUID();
    Serial.println(uid);
    lcd.setCursor(0, 2);
    lcd.print("Show your card");
    delay(2000);
    lcd.clear();


    if (isValidCard(uid)) {
      // const String python_script = "python post_api.py"; 
      // const String test = " test test";
      // const char* command = strcat(python_script, uid, test);
      // const String command = python_script + uid + test;  //concatenate the uid of the card with spaces in between
      // system(command.c_str()); 
      Serial.println("Access granted!");
      lcd.setCursor(0, 0);
      lcd.print("Scan card is correct");
      delay(1000);
      lcd.setCursor(0,0);
      lcd.print("Choose from below:");
      lcd.setCursor("0,1");
      lcd.print("1.Pen");
      let.setCursor("0,2;")
      lcd.print("2.Pencil");
      delay(1000);

      // Perform actions corresponding to valid access
      
      // Rotate the motor clockwise
      char key = keypad.getKey();
      if (key=='1')
      {
      digitalWrite(DIR_PIN, HIGH);
      for (int i = 0; i < 2500; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(300);  // Reduce delay for faster speed
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(300);  // Reduce delay for faster speed
      }

      delay(500); 
      } // Pause for 1 second

      // Rotate the motor counterclockwise
      else if(key=='2')
      {
      digitalWrite(DIR_PIN, LOW);
      for (int i = 0; i < 2500; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(300);  // Reduce delay for faster speed
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(300);  // Reduce delay for faster speed
      }
 
      delay(500);
      }  // Pause for 1 second
    } 
    else {
      Serial.println("Access denied!");
      lcd.setCursor(0,0);
      lcd.print("Scan card is incorrect!");
      // Perform actions corresponding to invalid access
    }

    // Halt the card
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
