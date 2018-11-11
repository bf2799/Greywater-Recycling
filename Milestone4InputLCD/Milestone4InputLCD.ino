// Load the LiquidCrystal library. Gives commands to interface to the LCD
#include <LiquidCrystal.h>

// Greywater Input number look-up
// Instead of making each input to an object, each input is given a number to make the code loops easier
//    0 = Fresh Water
//    1 = Laundry
//    2 = Shower
//    3 = Toilet

// Button Digital Input Ports
const int BUTTON_PORTS[] = {13 /*A*/, 12 /*B*/, 11 /*C*/, 10 /*D*/};

// LCD Ports
const int LCD_PORTS[] = {7, 6, 5, 4, 3, 2};

// Port of the LCD Backlight
const int LCD_BACKLIGHT_PORT = 9;

// Keep track of number of inputs and outputs
const int NUM_INPUTS = 4;
const int NUM_OUTPUTS = 4;

const String GREYWATER_INPUTS[NUM_INPUTS] = {"Fresh Water", "Laundry", "Shower", "Toilet"};

// Ananlog value for LCD Backlight
const int LCD_BACKLIGHT_VALUE = 100;

// Initialize Score lcd with digital ports
LiquidCrystal lcdInput(LCD_PORTS[0], LCD_PORTS[1], LCD_PORTS[2], LCD_PORTS[3], LCD_PORTS[4], LCD_PORTS[5]);

void setup() {

  // Set each button port to be an input
  for(int port : BUTTON_PORTS) {
    pinMode(port, INPUT);
  }

  // Set the backlight on the LCD
  analogWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VALUE);

  // Initialize 16 x 2 LCD and clear previous information
  lcdInput.begin(16,2);
  lcdInput.clear();

}

void loop() {

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Clear LCD screen of previous information
    lcdInput.clear();

    // Set LCD cursor to top left
    lcdInput.setCursor(0, 0);

    // Print the current greywater input
    lcdInput.print(GREYWATER_INPUTS[inputCounter]);

    // Delay one second to prevent button press from registering twice in a row
    delay(1000);

    // Set isButtonHit to false.
    // isButtonHit keeps track of whether a button has been hit for that 
    bool isButtonHit = false;

    // Initialize how many buttons were previously hit
    int prevNumButtonsHit = 0;
    
    // Keep checking for button press while one isn't pressed
    while(!isButtonHit) {

      // Initialize the number of buttons currently hit to 0
      int numButtonsHit = 0;
      
      // For each button, if pressed, increase the number of buttons that were pressed
      for (int button = 0; button < NUM_OUTPUTS; button++) {
        if (digitalRead(BUTTON_PORTS[button]) == LOW) {
          numButtonsHit++;
        }
      }

      // If the number of buttons currently pressed and pressed last time were 1
      //    Set that a button was hit to true
      // This method almost eliminates 'ghost' presses
      if (numButtonsHit == 1 && prevNumButtonsHit == 1) {
        isButtonHit = true;
      }

      // Set the previous number of buttons hit to the current number of buttons pressed
      prevNumButtonsHit = numButtonsHit;
      
    }

  }

}
