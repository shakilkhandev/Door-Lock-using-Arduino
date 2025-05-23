#include <LiquidCrystal.h>   // Include LCD library
#include <Servo.h>           // Include Servo library
#include <Keypad.h>          // Include Keypad library

Servo myservo;               // Create servo object
int pos = 0;                 // Variable to store the position of servo motor

// Initialize the LCD with the corresponding Arduino pins
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);

// Define keypad layout
const byte rows = 4;
const byte cols = 3;S

char key[rows][cols] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

// Connect keypad rows and columns to digital pins
byte rowPins[rows] = { 0, 1, 2, 3 };
byte colPins[cols] = { 4, 5, 6 };

// Create keypad object
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);

char* password = "7369";         // Set the correct password
int currentposition = 0;         // To track current character index being matched

int red = 13;                    // Red LED pin for incorrect password
int green = 12;                  // Green LED pin for correct password

void setup() {
  pinMode(red, OUTPUT);          // Set red LED pin as output
  pinMode(green, OUTPUT);        // Set green LED pin as output
  myservo.attach(9);             // Attach servo to pin 9
  lcd.begin(16, 2);              // Start the LCD with 16x2 size
}

// Main loop to continuously check keypad input
void loop() {
  if (currentposition == 0) {
    displayscreen();             // Show initial message when no input yet
  }

  char code = keypad.getKey();   // Read the key from keypad
  if (code != NO_KEY) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD:");
    lcd.setCursor(7, 1);
    lcd.print(" ");
    lcd.setCursor(7, 1);

    // Show * for entered characters
    for (int l = 0; l <= currentposition; ++l) {
      lcd.print("*");
    }

    // Check if the entered character matches the password
    if (code == password[currentposition]) {
      ++currentposition;
      if (currentposition == 4) {
        unlockdoor();            // Unlock door if full password is correct
        currentposition = 0;
      }
    } else {
      incorrect();               // Trigger incorrect password response
      currentposition = 0;
    }
  }
}

// Function to unlock door when password is correct
void unlockdoor() {
  digitalWrite(green, HIGH);     // Turn on green LED
  delay(1000);
  digitalWrite(green, LOW);      // Turn off green LED
  delay(900);

  // Display success message
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.print("WELCOME!!");

  // Rotate servo to open the door
  for (pos = 0; pos <= 180; pos += 5) {
    myservo.write(pos);
    delay(5);
  }

  delay(2000);                   // Wait for user to enter
  counterbeep();                 // Countdown display for relocking

  // Rotate servo back to close the door
  for (pos = 180; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(15);
    currentposition = 0;
    lcd.clear();
    displayscreen();             // Show initial screen again
  }
}

// Function to show message and blink red LED for wrong password
void incorrect() {
  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  delay(500);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CODE INCORRECT");
  lcd.setCursor(3, 1);
  lcd.print("GET AWAY!!!");
  delay(3000);
  lcd.clear();
  displayscreen();
}

// Function to clear all lines on LCD
void clearscreen() {
  lcd.setCursor(0, 0); lcd.println(" ");
  lcd.setCursor(0, 1); lcd.println(" ");
  lcd.setCursor(0, 2); lcd.println(" ");
  lcd.setCursor(0, 3); lcd.println(" ");
}

// Function to display initial message to prompt password entry
void displayscreen() {
  lcd.setCursor(0, 0);
  lcd.println("*ENTER THE CODE*");
  lcd.setCursor(1, 1);
  lcd.println("TO OPEN DOOR!!");
}

// Countdown timer before the door locks again
void counterbeep() {
  delay(1200);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("GET IN WITHIN:::");

  // Countdown from 5 to 0 with delays and screen updates
  for (int i = 5; i >= 0; i--) {
    lcd.setCursor(4, 1);
    lcd.print(i);
    delay(1000);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.println("GET IN WITHIN:::");
  }

  // Show relocking message with dots
  lcd.setCursor(2, 0);
  lcd.print("RE-LOCKING");
  delay(500); lcd.setCursor(12, 0); lcd.print(".");
  delay(500); lcd.setCursor(13, 0); lcd.print(".");
  delay(500); lcd.setCursor(14, 0); lcd.print(".");
  delay(400);

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("LOCKED!");
  delay(440);
}
