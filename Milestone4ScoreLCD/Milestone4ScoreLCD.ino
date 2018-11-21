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
//    5 = Bathroom Sink
//    6 = Kitchen Sink
//    7 = Gutter

// Button Digital Input Ports
const int BUTTON_PORTS[] = {A2 /*A*/, A3 /*B*/, A4 /*C*/, A5 /*D*/};

// LCD Ports
const int LCD_PORTS[] = {8, 9, 10, 11, 12, 13};

// Port of the LCD Backlight
const int LCD_BACKLIGHT_PORT = 6;

// Ports of the LED Strips
const int LED_STRIP_A_PORT = 53;
const int LED_STRIP_B_PORT = 52;
const int LED_STRIP_C_PORT = 0;
const int LED_STRIP_D_PORT = 50;

// LED_STRIP_PORTS index for top LEDs
const int TOP_LEDS_INDEX = 2;

// Keep track of number of inputs and outputs
const int NUM_INPUTS = 8;
const int NUM_OUTPUTS = 4;

// Scores of each input-output combination
                            /* Outputs:     Sprinkler         Sewer         Sink        Hose     Inputs */
const int SCORE[NUM_INPUTS][NUM_OUTPUTS] ={{5,                0,            10,         5 },  // Fresh Water
                                          { 10,               5,            1,          10},  // Laundry
                                          { 10,               5,            1,          10},  // Shower
                                          { 0,                10,           0,          0 },  // Toilet
                                          { 10,               5,            1,          10},  // Dishwasher
                                          { 10,               5,            1,          10},  // Bathroom Sink
                                          { 3,                7,            0,          3 },  // Kitchen Sink
                                          { 10,               3,            0,          10}}; // Gutter

// Each button corresponds to one output.
// The index in the score matrix where this output lies is the button score index
const int BUTTON_SCORE_INDEX[] = {2 /*Button A- Sink*/ , 1 /*Button B- Sewer*/, 3 /*Button C- Hose*/, 0 /*Button D- Sprinkler*/};

// Ananlog value for LCD Backlight
const int LCD_BACKLIGHT_VALUE = 100;

// The number of LEDs in each LED Strip
const int NUM_LEDS[] = {29 /*Button A- Sink*/, 29 /*Button B- Sewer*/, 0 /*Button C- Hose*/, 29 /*Button D- Sprinkler*/};

// The number of LEDs on the TOP_LED_PORT above the normal number of LEDs
const int NUM_TOP_LEDS = 0;

// Time to delay between changes in flow pattern
const int FLOW_DELAY_MS = 50;

// Time to delay to look at computer screen
const int SCREEN_DELAY_SEC = 5;

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
  analogWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VALUE);

  // Initialize 16 x 2 LCD and clear previous information
  lcdScore.begin(16,2);
  lcdScore.clear();

  // Make each LED strip have access to FastLED library
  FastLED.addLeds<WS2812, LED_STRIP_A_PORT, GRB>(ledStrip[0], NUM_LEDS[0]);
  FastLED.addLeds<WS2812, LED_STRIP_B_PORT, GRB>(ledStrip[1], NUM_LEDS[1]);
  //FastLED.addLeds<WS2812, LED_STRIP_C_PORT, GRB>(ledStrip[2], NUM_LEDS[2] + NUM_TOP_LEDS);
  FastLED.addLeds<WS2812, LED_STRIP_D_PORT, GRB>(ledStrip[3], NUM_LEDS[3]);
}

// Initialize user score
int userScore;

void loop() {
  
  // Reset user score for each new game
  userScore = 0;

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Turn all LED strips to their default color to make following them easier
    for (int output = 0; output < NUM_OUTPUTS; output++) {
      for (int led = 0; led < NUM_LEDS[output]; led++) {
        ledStrip[output][led] = DEFAULT_COLORS[output];
      }
    }
    FastLED.show();

    // Flow the LEDs from the LCD screen to the letter labels
    for (int led = NUM_TOP_LEDS + NUM_LEDS[TOP_LEDS_INDEX] - 2; led >= NUM_LEDS[TOP_LEDS_INDEX]; led--) {
      ledStrip[TOP_LEDS_INDEX][led] = WATER_COLOR_HIGH;
      ledStrip[TOP_LEDS_INDEX][led + 1] = NO_COLOR;
      FastLED.show();
      delay(FLOW_DELAY_MS);
    }

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

    // Turn off LED on top
    ledStrip[TOP_LEDS_INDEX][NUM_LEDS[TOP_LEDS_INDEX]] = NO_COLOR;

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
  ldcScore.setCursor(0, 1);

  // Print the high score to the LCD
  lcdScore.print("High Score: " + String(highScore));

  // Wait while button A isn't pressed
  while(digitalRead(BUTTON_PORTS[0] == HIGH) {
    // Do nothing
  }
    
}
