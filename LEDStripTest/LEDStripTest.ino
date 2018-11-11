#include <FastLED.h>

const int NUM_LEDS = 60;
const int LED_PIN = 7;

const int DELAY = 50;

CRGB ledStrip[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812, LED_PIN, GRB>(ledStrip, NUM_LEDS);

}

void loop() {

  ledStrip[0] = CRGB(0, 70, 200);
  FastLED.show();
  delay(DELAY);

  ledStrip[1] = CRGB(0, 70, 200);
  ledStrip[0] = CRGB(0, 140, 200);
  FastLED.show();
  delay(DELAY);

  ledStrip[2] = CRGB(0, 70, 200);
  ledStrip[1] = CRGB(0, 140, 200);
  ledStrip[0] = CRGB(0, 210, 200);
  FastLED.show();
  delay(DELAY);

  ledStrip[3] = CRGB(0, 70, 200);
  ledStrip[2] = CRGB(0, 140, 200);
  ledStrip[1] = CRGB(0, 210, 200);
  ledStrip[0] = CRGB(0, 140, 200);
  FastLED.show();
  delay(DELAY);

  ledStrip[4] = CRGB(0, 70, 200);
  ledStrip[3] = CRGB(0, 140, 200);
  ledStrip[2] = CRGB(0, 210, 200);
  ledStrip[1] = CRGB(0, 140, 200);
  ledStrip[0] = CRGB(0, 70, 200);
  FastLED.show();
  delay(DELAY);

  for (int i = 3; i < NUM_LEDS - 2; i++) {
    ledStrip[i] = CRGB(0, 210, 200);
    ledStrip[i - 1] = CRGB(0, 140, 200);
    ledStrip[i + 1] = CRGB(0, 140, 200);
    ledStrip[i - 2] = CRGB(0, 70, 200);
    ledStrip[i + 2] = CRGB(0, 70, 200);
    ledStrip[i - 3] = CRGB(0, 0, 0);
    FastLED.show();
    delay(DELAY);
  }

  ledStrip[NUM_LEDS - 1] = CRGB(0, 140, 200);
  ledStrip[NUM_LEDS - 2] = CRGB(0, 210, 200);
  ledStrip[NUM_LEDS - 3] = CRGB(0, 140, 200);
  ledStrip[NUM_LEDS - 4] = CRGB(0, 70, 200);
  ledStrip[NUM_LEDS - 5] = CRGB(0, 0, 0);
  FastLED.show();
  delay(DELAY);

  ledStrip[NUM_LEDS - 1] = CRGB(0, 210, 200);
  ledStrip[NUM_LEDS - 2] = CRGB(0, 140, 200);
  ledStrip[NUM_LEDS - 3] = CRGB(0, 70, 200);
  ledStrip[NUM_LEDS - 4] = CRGB(0, 0, 0);
  FastLED.show();
  delay(DELAY);

  ledStrip[NUM_LEDS - 1] = CRGB(0, 140, 200);
  ledStrip[NUM_LEDS - 2] = CRGB(0, 70, 200);
  ledStrip[NUM_LEDS - 3] = CRGB(0, 0, 0);
  FastLED.show();
  delay(DELAY);

  ledStrip[NUM_LEDS - 1] = CRGB(0, 70, 200);
  ledStrip[NUM_LEDS - 2] = CRGB(0, 0, 0);
  FastLED.show();
  delay(DELAY);

  ledStrip[NUM_LEDS - 1] = CRGB(0, 0, 0);
  FastLED.show();
  delay(DELAY);

}
