// Greywater Input number look-up
// Instead of making each input to an object, each input is given a number to make the code loops easier
//    0 = Fresh Water
//    1 = Laundry
//    2 = Shower
//    3 = Toilet
//    4 = Dishwasher

// Button Digital Input Ports
const int BUTTON_A_PORT = 0;
const int BUTTON_B_PORT = 0;
const int BUTTON_C_PORT = 0;
const int BUTTON_D_PORT = 0;
                               
// Scores of each input-output combination
                        /*Sprinkler*/     /*Sewer*/     /*Sink*/    /*Hose*/
const int[] SCORE_0 = {   5,                1,            10,         5};   // Fresh Water
const int[] SCORE_1 = {   10,               5,            2,          10};  // Laundry
const int[] SCORE_2 = {   10,               5,            2,          10};  // Shower
const int[] SCORE_3 = {   1,                10,           0,          1};   // Toilet

// Number of greywater inputs
const int NUM_INPUTS = 4;

// Initialize user score
int userScore;

void setup() {
  

}

void loop() {

  // Loop through the greywater inputs each time the game runs
  for(int inputCounter = 0; inputCounter < NUM_INPUTS; inputCounter++) {

    // Reset user score
    userScore = 0;

    bool buttonHit = false;
    while(digitalRead(BUTTON_A_PORT) == LOW && digitalRead(BUTTON_B_PORT) == LOW && digitalRead(BUTTON_C_PORT) == LOW && digitalRead(BUTTON_A_PORT) == LOW) {
    
    }

  }

}
