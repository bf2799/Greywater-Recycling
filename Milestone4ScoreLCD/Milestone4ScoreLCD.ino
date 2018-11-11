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

// Button Digital Input Ports
const int BUTTON_PORTS[] = {A5 /*A*/, A4 /*B*/, A3 /*C*/, A2 /*D*/};

// LCD Ports
const int LCD_PORTS[] = {7, 6, 5, 4, 3, 2};

// Port of the LCD Backlight
const int LCD_BACKLIGHT_PORT = 9;

// Ports of the LED Strips
const int LED_STRIP_A_PORT = 13;
const int LED_STRIP_B_PORT = 12;
const int LED_STRIP_C_PORT = 11;
const int LED_STRIP_D_PORT = 10;

// LED_STRIP_PORTS index for top LEDs
const int TOP_LEDS_INDEX = 2;

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

// The number of LEDs in each LED Strip
const int NUM_LEDS[] = {30 /*A*/, 30 /*B*/, 30 /*C*/, 30 /*D*/};

// The number of LEDs on the TOP_LED_PORT above the normal number of LEDs
const int NUM_TOP_LEDS = 5;

// Time to delay between changes in flow pattern
const int FLOW_DELAY_MS = 50;

// Preset color options for LED strips
const CRGB WATER_COLOR_LOW(0, 240, 200);
const CRGB WATER_COLOR_MED(0, 120, 200);
const CRGB WATER_COLOR_HIGH(0,  0, 200);
const CRGB DEFAULT_COLORS[] = {CRGB(50, 0, 50) /*A*/, CRGB(50, 50, 0) /*B*/, CRGB(50, 50, 50) /*C*/, CRGB(0, 50, 0) /*D*/};
const CRGB NO_COLOR(0, 0, 0);

// Initialize Score lcd with digital ports
LiquidCrystal lcdScore(LCD_PORTS[0], LCD_PORTS[1], LCD_PORTS[2], LCD_PORTS[3], LCD_PORTS[4], LCD_PORTS[5]);

// Initialize led strips 
CRGB ledStrip[NUM_OUTPUTS][30]; // 30 is maximum number of LEDs in strip

void setup() {

  // Set the backlight on the LCD
  analogWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VALUE);

  // Initialize 16 x 2 LCD and clear previous information
  lcdScore.begin(16,2);
  lcdScore.clear();

  // Make each LED strip have access to FastLED library
  FastLED.addLeds<WS2812, LED_STRIP_A_PORT, GRB>(ledStrip[0], NUM_LEDS[0]);
  FastLED.addLeds<WS2812, LED_STRIP_B_PORT, GRB>(ledStrip[1], NUM_LEDS[1]);
  FastLED.addLeds<WS2812, LED_STRIP_C_PORT, GRB>(ledStrip[2], NUM_LEDS[2] + NUM_TOP_LEDS);
  FastLED.addLeds<WS2812, LED_STRIP_D_PORT, GRB>(ledStrip[3], NUM_LEDS[3]);

  // Set each LED strip pin to be an output
  pinMode(LED_STRIP_A_PORT, OUTPUT);
  pinMode(LED_STRIP_B_PORT, OUTPUT);
  pinMode(LED_STRIP_C_PORT, OUTPUT);
  pinMode(LED_STRIP_D_PORT, OUTPUT);

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


    ledStrip[TOP_LEDS_INDEX][NUM_LEDS[TOP_LEDS_INDEX]] = NO_COLOR;
    /************************
    TODO: Light up correct LED Path
    ************************/

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

    // Delay one second to prevent button press from registering twice in a row
    delay(1000);

  }

  // Clear the LCD screen
  lcdScore.clear();

  // Set LCD cursor to first position on screen (0, 1)
  lcdScore.setCursor(0, 0);

  // Print the user score to the LCD
  lcdScore.print("Final Score: " + String(userScore));
    
}
