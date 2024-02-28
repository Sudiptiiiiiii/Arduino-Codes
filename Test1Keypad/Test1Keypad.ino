#include <Keypad.h>

const byte ROWS = 4; // Number of rows in the keypad
const byte COLS = 4; // Number of columns in the keypad

// Define the keymap - adjust according to your keypad layout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define pin numbers connected to the keypad rows and columns
byte rowPins[ROWS] = {9, 8, 7, 6}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // Connect to the column pinouts of the keypad

// Create the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  // Get key pressed by the user
  char key = keypad.getKey();

  // Check if a key is pressed
  if (key != NO_KEY) {
    // Send the pressed key data serially to the NodeMCU
    Serial.println("You pressed: " + String(key));
  }
  delay(100); // Delay to avoid reading the keypad too fast
}
