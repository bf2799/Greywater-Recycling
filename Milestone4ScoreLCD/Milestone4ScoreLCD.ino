// Load the LiquidCrystal library. Gives commands to interface to the LCD
#include <LiquidCrystal.h>

// Load the FastLED library. Gives control over LED strips
#include <FastLED.h>

// Greywater Input number look-up
// Instead of making each input to an object, each input is given a number to make the code loops easier
//    0 = Fresh Water
//    1 = Laundry
//    2 = Shower
//    3 = Toilet
//    4 = Dishwasher
//    5 = Kitchen Sink

// Button Digital Input Ports
const int BUTTON_PORTS[] = {A0 /*A*/, A2 /*B*/, A1 /*C*/, A3 /*D*/};

// LCD Ports
const int LCD_PORTS[] = {22, 24, 26, 28, 30, 32};

// Ports of the LED Strips
const int LED_STRIP_A_PORT = 51;
const int LED_STRIP_B_PORT = 50;
const int LED_STRIP_C_PORT = 53;
const int LED_STRIP_D_PORT = 52;

// Keep track of number of inputs and outputs
const int NUM_INPUTS = 6;
const int NUM_OUTPUTS = 4;

// Scores of each input-output combination
                            /* Outputs:     Hose              Sink          Toilet Bowl Sewer    Inputs */
const int SCORE[NUM_INPUTS][NUM_OUTPUTS] ={{5,                10,           5,          0 },  // Fresh Water
                                          { 10,               1,            10,         5},   // Laundry
                                          { 10,               1,            10,         5},   // Shower
                                          { 0,                0,            0,          10},  // Toilet
                                          { 10,               1,            10,         5},   // Dishwasher
                                          { 3,                1,            3,          7}};  // Kitchen Sink          

// Each button corresponds to one output.
// The index in the score matrix where this output lies is the button score index
const int BUTTON_SCORE_INDEX[] = {1 /*Button A- Sink*/ , 3 /*Button B- Sewer*/, 0 /*Button C- Hose*/, 2 /*Button D- Toilet Bowl*/};

// The number of LEDs in each LED Strip
const int NUM_LEDS[] = {23 /*Button A- Sink*/, 27 /*Button B- Sewer*/, 24 /*Button C- Hose*/, 22 /*Button D- Toilet Bowl*/};

// Time to delay between changes in flow pattern
const int FLOW_DELAY_MS = 50;

// Time to delay to look at computer screen
const int SCREEN_DELAY_SEC = 5;

// Time the start button needs to be held in order to trigger the game starting
const int START_BUTTON_HOLD_TIME_MS = 1000;

// Time to hold buttons A and B to reset the game
const int RESET_HOLD_TIME_MS = 2000;

// Time it takes LEDs to flash 3 times
const int LED_FLASH_TIME_MS = 1500;

// Preset color options for LED strips
const CRGB WATER_COLOR_LOW(0, 240, 200);
const CRGB WATER_COLOR_MED(0, 120, 200);
const CRGB WATER_COLOR_HIGH(0,  0, 200);
const CRGB DEFAULT_COLORS[] = {CRGB(15, 0, 15) /*A*/, CRGB(15, 5, 0) /*B*/, CRGB(15, 15, 15) /*C*/, CRGB(0, 15, 0) /*D*/};
const CRGB NO_COLOR(0, 0, 0);

// Initialize Score lcd with digital ports
LiquidCrystal lcdScore(LCD_PORTS[0], LCD_PORTS[1], LCD_PORTS[2], LCD_PORTS[3], LCD_PORTS[4], LCD_PORTS[5]);

// Initialize led strips 
CRGB ledStrip[NUM_OUTPUTS][30]; // 30 is maximum number of LEDs in strip

int highScore = 0;

void setup() {

  // Set the backlight on the LCD

  // Initialize 16 x 2 LCD and clear previous information
  lcdScore.begin(16,2);
  lcdScore.clear();

  // Make each LED strip have access to FastLED library
  FastLED.addLeds<WS2812, LED_STRIP_A_PORT, GRB>(ledStrip[0], NUM_LEDS[0]);
  FastLED.addLeds<WS2812, LED_STRIP_B_PORT, GRB>(ledStrip[1], NUM_LEDS[1]);
  FastLED.addLeds<WS2812, LED_STRIP_C_PORT, GRB>(ledStrip[2], NUM_LEDS[2]);
  FastLED.addLeds<WS2812, LED_STRIP_D_PORT, GRB>(ledStrip[3], NUM_LEDS[3]);

  Serial.begin(9600);
}

// Initialize user score
int userScore;

void loop() {

  // Initialize whether the game has started as false
  bool isStarted = false;

  // Keep running code while button A hasn't been held to start the game
  while(!isStarted) {

    bool isPressed = false;
    // Wait while button A isn't pressed
    while(!isPressed) {
      if (digitalRead(BUTTON_PORTS[0]) == LOW) {
        isPressed = true;
      }
    }

    // Wait for time button should be held down to start
    delay(START_BUTTON_HOLD_TIME_MS);

    // If button A is still pressed after delay
    //    Set that the game has started to true to exit loop
    if (digitalRead(BUTTON_PORTS[0]) == LOW) {
      isStarted = true;
    }
    
  }

  // Clear LCD screen
  lcdScore.clear();
  
  // Reset user score for each new game
  userScore = 0;

  // Pause to give user time to release button
  delay(2 * START_BUTTON_HOLD_TIME_MS);

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Turn all LED strips to their default color to make following them easier
    for (int output = 0; output < NUM_OUTPUTS; output++) {
      for (int led = 0; led < NUM_LEDS[output]; led++) {
        ledStrip[output][led] = DEFAULT_COLORS[output];
      }
    }
    FastLED.show();

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

    Serial.println(buttonPressed);

    // Delay 50 ms to allow user time to press both A and B buttons
    delay(50);

    // Get the current time
    long curTime = millis();

    // Wait while buttons A and B are still pressed and reset time hasn't been passed
    while (digitalRead(BUTTON_PORTS[0]) == LOW && digitalRead(BUTTON_PORTS[1]) == LOW
            && millis() - curTime <= RESET_HOLD_TIME_MS) {
        // Do Nothing. Wait
    }

    // If more than the reset time has passed
    //  Exit out of the input loop to end the game
    if (millis() - curTime > RESET_HOLD_TIME_MS) {
      break;
    }

    /***************************************************************
     * FIRST FLOW OF LEDS (needed to get within index bounds of loop
     ***************************************************************/
     
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 1] = WATER_COLOR_LOW;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 2] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 1] = WATER_COLOR_MED;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 3] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 2] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 1] = WATER_COLOR_HIGH;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 4] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 3] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 2] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 1] = WATER_COLOR_MED;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 5] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 4] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 3] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 2] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS[buttonPressed] - 1] = WATER_COLOR_LOW;
    FastLED.show();
    delay(FLOW_DELAY_MS);

    /******************************************
     * MAIN FLOW PATTERN OF 5 LIGHTS AT A TIME
     ******************************************/
    
    for (int i = NUM_LEDS[buttonPressed] - 4; i > 1; i--) {
      ledStrip[buttonPressed][i] = WATER_COLOR_HIGH;
      ledStrip[buttonPressed][i - 1] = WATER_COLOR_MED;
      ledStrip[buttonPressed][i + 1] = WATER_COLOR_MED;
      ledStrip[buttonPressed][i - 2] = WATER_COLOR_LOW;
      ledStrip[buttonPressed][i + 2] = WATER_COLOR_LOW;
      ledStrip[buttonPressed][i + 3] = DEFAULT_COLORS[buttonPressed];
      FastLED.show();
      delay(FLOW_DELAY_MS);
    }

    /******************************************
     * FLOW AT END OF LEDS
     ******************************************/
    ledStrip[buttonPressed][0] = WATER_COLOR_MED;
    ledStrip[buttonPressed][1] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][2] = WATER_COLOR_MED;
    ledStrip[buttonPressed][3] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][4] = DEFAULT_COLORS[buttonPressed];
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][0] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][1] = WATER_COLOR_MED;
    ledStrip[buttonPressed][2] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][3] = DEFAULT_COLORS[buttonPressed];
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][0] = WATER_COLOR_MED;
    ledStrip[buttonPressed][1] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][2] = DEFAULT_COLORS[buttonPressed];
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][0] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][1] = DEFAULT_COLORS[buttonPressed];
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][0] = DEFAULT_COLORS[buttonPressed];
    FastLED.show();
    delay(FLOW_DELAY_MS); 

    /******END OF LED SECTION******/

    // Add the score of the input-output combination to user score
    userScore += SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]];

    // Clear the LCD screen
    lcdScore.clear();

    // Set LCD cursor to first position on screen (0, 1)
    lcdScore.setCursor(0, 0);

    // Print the user score to the LCD
    lcdScore.print("+ " + String(SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]]));

    // Set the cursor to the second line
    lcdScore.setCursor(0, 1);

    // Print the total score
    lcdScore.print("Score: " + String(userScore));

    // Delay for time to look at Matlab screen
    delay(SCREEN_DELAY_SEC * 1000);

    // Repeat blink 3 times
    for (int i = 0; i < 3; i++) {

      // Turn all LED strips off
      for (int output = 0; output < NUM_OUTPUTS; output++) {
        for (int led = 0; led < NUM_LEDS[output]; led++) {
          ledStrip[output][led] = NO_COLOR;
        }
      }
      FastLED.show();

      // Delay enough time for 3 flashes to happen
      delay(LED_FLASH_TIME_MS / 6);
      
      // Turn all LED strips to their default color
      for (int output = 0; output < NUM_OUTPUTS; output++) {
        for (int led = 0; led < NUM_LEDS[output]; led++) {
          ledStrip[output][led] = DEFAULT_COLORS[output];
        }
      }
      FastLED.show();

      // Delay enough time for 3 flashes to happen
      delay(LED_FLASH_TIME_MS / 6);

    }

  }

  if (userScore > highScore) {
    highScore = userScore;
  }

  // Clear the LCD screen
  lcdScore.clear();

  // Set LCD cursor to first position on screen (0, 0)
  lcdScore.setCursor(0, 0);

  // Print the user score to the LCD
  lcdScore.print("Final Score: " + String(userScore));

  // Set LCD cursor to beginning of second line
  lcdScore.setCursor(0, 1);

  // Print the high score to the LCD
  lcdScore.print("High Score: " + String(highScore));
    
}
