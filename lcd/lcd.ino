#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address
const int lcdAddress = 0x27;  // Adjust the I2C address based on your module

// Set the LCD dimensions
const int lcdColumns = 20;
const int lcdRows = 4;

// Create an LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

void setup() {
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available on your LCD module)
  lcd.backlight();

  // Print a welcome message
  lcd.setCursor(0, 0);
  // lcd.print("Hello Sakshi!");
}

void loop() {
  // Blink a message on the LCD
  lcd.setCursor(0, 1);
  lcd.print("hello shakshi");

  delay(1000);  // Wait for 1 second

  lcd.clear();  // Clear the LCD screen

  // delay(1000);  // Wait for 1 second before repeating
}
