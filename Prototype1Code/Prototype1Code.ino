// Greywater Input number look-up
// Instead of making each input to an object, each input is given a number to make the code loops easier
//    0 = Fresh Water
//    1 = Laundry
//    2 = Shower
//    3 = Toilet
//    4 = Dishwasher

// Button Digital Input Ports
const int[] BUTTON_PORTS = {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/};

// LED Digital Output Ports
const int[] LED_PORTS = {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/};
                               
// Scores of each input-output combination
           /* Outputs:    Sprinkler         Sewer         Sink        Hose     Inputs */
const int[][] SCORE =  {{ 5,                1,            10,         5},   // Fresh Water
                        { 10,               5,            2,          10},  // Laundry
                        { 10,               5,            2,          10},  // Shower
                        { 1,                10,           0,          1}};  // Toilet

// Each button corresponds to one output.
// The index in the score matrix where this output lies is the button score index
const int[] BUTTON_SCORE_INDEX = {2 /*Button A- Sink*/ , 1 /*Button B- Sewer*/, 3 /*Button C- Hose*/, 4 /*Button D- Sprinkler*/};

// Keep track of number of inputs and outputs
const int NUM_INPUTS = 4;
const int NUM_OUTPUS = 4;

// Number of milliseconds the LED paths should stay lit up for
const int LED_MS_LIT_UP = 1500;

// Initialize user score
int userScore;

/**
 * Setup sets up input/output modes for ports
 */
void setup() {

  // Set each button port to be an input
  for(int port : BUTTON_PORTS) {
    pinMode(port, INPUT);
  }

  // Set each LED port to be an output
  for(int port: LED_PORTS) {
    pinMode(port, OUTPUT);
  }
  
}

/**
 * Each loop is one player's game
 */
void loop() {

  // Reset user score for each new game
  userScore = 0;

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

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

    // Light up correct LED Path for LED_MS_LIT_UP milliseconds
    digitalWrite(LED_PORTS[buttonPressed], HIGH);
    delay(LED_MS_LIT_UP);
    digitalWrite(LED_PORTS[buttonPressed], LOW);

    // Add the score of the input-output combination to 
    userScore += SCORE[inputCounter][BUTTON_SCORE_INDEX[buttonPressed]];

  }

}
