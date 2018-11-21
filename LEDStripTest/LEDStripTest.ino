#include <FastLED.h>
#include <LiquidCrystal.h>

const int NUM_LEDS = 29;
const int LED_PIN = 53;
const int LED_PIN2 = 52;
const int LED_PIN1 = 50;

// Button Digital Input Ports
const int BUTTON_PORTS[] = {A2 /*A*/, A3 /*B*/, A4 /*C*/, A5 /*D*/};

// LCD Ports
const int LCD_PORTS[] = {8, 9, 10, 11, 12, 13};

// Port of the LCD Backlight
const int LCD_BACKLIGHT_PORT = 6;

// Keep track of number of inputs and outputs
const int NUM_INPUTS = 4;
const int NUM_OUTPUTS = 4;

// Scores of each input-output combination
                            /* Outputs:     Sprinkler         Sewer         Sink        Hose     Inputs */
const int SCORE[NUM_INPUTS][NUM_OUTPUTS] ={{5,                0,            10,         5},   // Fresh Water
                                          { 10,               5,            1,          10},  // Laundry
                                          { 10,               5,            1,          10},  // Shower
                                          { 0,                10,           0,          0}};  // Toilet

// Each button corresponds to one output.
// The index in the score matrix where this output lies is the button score index
const int BUTTON_SCORE_INDEX[] = {2 /*Button A- Sink*/ , 1 /*Button B- Sewer*/, 3 /*Button C- Hose*/, 0 /*Button D- Sprinkler*/};

// The number of LEDs in each LED Strip
const int NUM_LEDS[] = {29 /*Button A- Sink*/, 29 /*Button B- Sewer*/, 0 /*Button C- Hose*/, 29 /*Button D- Sprinkler*/};

// Ananlog value for LCD Backlight
const int LCD_BACKLIGHT_VALUE = 100;

const int FLOW_DELAY_MS = 50;

CRGB ledStrip[NUM_OUTPUTS][29];

// Initialize Score lcd with digital ports
LiquidCrystal lcdScore(LCD_PORTS[0], LCD_PORTS[1], LCD_PORTS[2], LCD_PORTS[3], LCD_PORTS[4], LCD_PORTS[5]);

// Preset color options for LED strips
const CRGB WATER_COLOR_LOW(0, 240, 200);
const CRGB WATER_COLOR_MED(0, 120, 200);
const CRGB WATER_COLOR_HIGH(0,  0, 200);
const CRGB DEFAULT_COLORS[] = {CRGB(15, 0, 15) /*A*/, CRGB(15, 15, 0) /*B*/, CRGB(15, 15, 15) /*C*/, CRGB(0, 15, 0) /*D*/};
const CRGB NO_COLOR(0, 0, 0);

void setup() {

  // Set the backlight on the LCD
  analogWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VALUE);

  // Initialize 16 x 2 LCD and clear previous information
  lcdScore.begin(16,2);
  lcdScore.clear();

  FastLED.addLeds<WS2812, LED_PIN, GRB>(ledStrip[0], NUM_LEDS[0]);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(ledStrip[1], NUM_LEDS[1]);
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(ledStrip[2], NUM_LEDS[3]);

}

// Initialize user score
int userScore;

void loop() {

  // Reset user score for each new game
  userScore = 0;

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Turn all LED strips to their default color to make following them easier
    for (int output = 0; output < 4; output++) {
      for (int led = 0; led < NUM_LEDS[output]; led++) {
        ledStrip[output][led] = DEFAULT_COLORS[output];
      }
    }
    FastLED.show();
  
    // Flow the LEDs from the LCD screen to the letter labels
    for (int led = NUM_LEDS[0] - 2; led >= NUM_LEDS[0]; led--) {
      ledStrip[0][led] = WATER_COLOR_HIGH;
      ledStrip[0][led + 1] = NO_COLOR;
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
      for (int button = 0; button < 4; button++) {
        if (digitalRead(BUTTON_PORTS[button]) == LOW) {
          isButtonHit = true;
          buttonPressed = button;
        }
      }
      
    }
  
    // Turn off LED on top
    ledStrip[0][NUM_LEDS[0]] = NO_COLOR;
  
    ledStrip[buttonPressed][NUM_LEDS - 1] = WATER_COLOR_LOW;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS - 2] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS - 1] = WATER_COLOR_MED;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS - 3] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS - 2] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS - 1] = WATER_COLOR_HIGH;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS - 4] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS - 3] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS - 2] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][NUM_LEDS - 1] = WATER_COLOR_MED;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    ledStrip[buttonPressed][NUM_LEDS - 5] = WATER_COLOR_LOW;
    ledStrip[buttonPressed][NUM_LEDS - 4] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS - 3] = WATER_COLOR_HIGH;
    ledStrip[buttonPressed][NUM_LEDS - 2] = WATER_COLOR_MED;
    ledStrip[buttonPressed][NUM_LEDS - 1] = WATER_COLOR_LOW;
    FastLED.show();
    delay(FLOW_DELAY_MS);
  
    /******************************************
     * MAIN FLOW PATTERN OF 5 LIGHTS AT A TIME
     ******************************************/
    
    for (int i = NUM_LEDS - 4; i > 1; i--) {
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

    // Add the score of the input-output combination to user score
    userScore += SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]];

    // Clear the LCD screen
    lcdScore.clear();

    // Set LCD cursor to first position on screen (0, 1)
    lcdScore.setCursor(0, 0);

    // Print the user score to the LCD
    lcdScore.print("Score: " + String(SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]]));

    // Set the cursor to the second line
    lcdScore.setCursor(0, 1);

    // Print the total score
    lcdScore.print("Total: " + String(userScore));
    
  }

  // Clear the LCD screen
  lcdScore.clear();

  // Set LCD cursor to first position on screen (0, 1)
  lcdScore.setCursor(0, 0);

  // Print the user score to the LCD
  lcdScore.print("Final Score: " + String(userScore));

}
