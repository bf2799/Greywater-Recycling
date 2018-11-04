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

// Scores of each input-output combination
                            /* Outputs:     Sprinkler         Sewer         Sink        Hose     Inputs */
const int SCORE[NUM_INPUTS][NUM_OUTPUTS] ={{5,                1,            10,         5},   // Fresh Water
                                          { 10,               5,            1,          10},  // Laundry
                                          { 10,               5,            1,          10},  // Shower
                                          { 1,                10,           1,          1}};  // Toilet

// Each button corresponds to one output.
// The index in the score matrix where this output lies is the button score index
const int BUTTON_SCORE_INDEX[] = {2 /*Button A- Sink*/ , 1 /*Button B- Sewer*/, 3 /*Button C- Hose*/, 0 /*Button D- Sprinkler*/};

// Ananlog value for LCD Backlight
const int LCD_BACKLIGHT_VALUE = 100;

// Initialize Score lcd with digital ports
LiquidCrystal lcdScore(LCD_PORTS[0], LCD_PORTS[1], LCD_PORTS[2], LCD_PORTS[3], LCD_PORTS[4], LCD_PORTS[5]);

void setup() {
   
  // Set each button port to be an input
  for(int port : BUTTON_PORTS) {
    pinMode(port, INPUT);
  }

  // Set the backlight on the LCD
  analogWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VALUE);

  // Initialize 16 x 2 LCD and clear previous information
  lcdScore.begin(16,2);
  lcdScore.clear();

}

// Initialize user score
int userScore;

void loop() {
  
  // Reset user score for each new game
  userScore = 0;

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Delay one second to prevent button press from registering twice in a row
    delay(1000);

    // Initialize button pressed
    int buttonPressed;

    // Set isButtonHit to false.
    // isButtonHit keeps track of whether a button has been hit for that 
    bool isButtonHit = false;
    
    // Keep checking for button press while one isn't pressed
    while(!isButtonHit) {

      // For each button, if pressed, register that a button was pressed and which button it was
      for (int button = 0; button < NUM_OUTPUTS; button++) {
        if (digitalRead(BUTTON_PORTS[button]) == LOW) {
          isButtonHit = true;
          buttonPressed = button;
        }
      }
      
    }

    /************************
    Light up correct LED Path
    ************************/

    // Add the score of the input-output combination to user score
    userScore += SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]];

    // Clear the LCD screen
    lcdScore.clear();

    // Set LCD cursor to first position on screen (0, 1)
    lcdScore.setCursor(0, 0);

    // Print the user score to the LCD
    lcdScore.print("Score: " + String(SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]]));

    lcdScore.setCursor(0, 1);

    lcdScore.print("Total: " + String(userScore));

  }

  // Clear the LCD screen
  lcdScore.clear();

  // Set LCD cursor to first position on screen (0, 1)
  lcdScore.setCursor(0, 0);

  // Print the user score to the LCD
  lcdScore.print("Final Score: " + String(userScore));
    
}
