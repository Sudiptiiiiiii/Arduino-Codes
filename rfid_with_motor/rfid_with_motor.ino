#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>

#define SS_PIN 53   //for rfid 
#define RST_PIN 49

#define STEP_PIN 2  // Connect to the STEP pin on the motor driver
#define DIR_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  Serial.begin(115200);       //Initiate serial communication
  SPI.begin();                //Initiate SPI bus
  mfrc522.PCD_Init();         //Initiate MFRC522

  // motor setup
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}   

void loop() 
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())   //look for cards and select the cards
  {
    String uid = getCardUID();
    Serial.println(uid);

    if (isValidCard(uid)) {
      // const String python_script = "python post_api.py"; 
      // const String test = " test test";
      // const char* command = strcat(python_script, uid, test);
      // const String command = python_script + uid + test;  //concatenate the uid of the card with spaces in between
      // system(command.c_str()); 
      Serial.println("Access granted!");
      // Perform actions corresponding to valid access
      
      // Rotate the motor clockwise
      digitalWrite(DIR_PIN, HIGH);
      for (int i = 0; i < 2500; i++) {
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
    } else {
      Serial.println("Access denied!");
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
