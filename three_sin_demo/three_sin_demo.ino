#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Initialize global variables for sequences
uint8_t thisdelay = 50;                                       // A delay value for the sequence(s)

int wave1=0;                                                  // Current phase is calculated.
int wave2=0;
int wave3=0;

uint8_t inc1 = 2;                                             // Phase shift. Keep it low, as this is the speed at which the waves move.
uint8_t inc2 = 1;
uint8_t inc3 = -2;

uint8_t lvl1 = 80;                                            // Any result below this value is displayed as 0.
uint8_t lvl2 = 80;
uint8_t lvl3 = 80;

uint8_t mul1 = 20;                                            // Frequency, thus the distance between waves
uint8_t mul2 = 25;
uint8_t mul3 = 22;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.

  random16_set_seed(4832);                                    // Awesome randomizer (which we don't yet need here.)
  random16_add_entropy(analogRead(2));
}

void loop () {
  ChangeMe();
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    three_sin();                                              // Improved method of using non-blocking delay
  }
  FastLED.show();
}

void three_sin() {
  wave1 += inc1;
  wave2 += inc2;
  wave3 += inc3;
  for (int k=0; k<NUM_LEDS; k++) {
    leds[k].r = qsub8(sin8(mul1*k + wave1/128), lvl1);        // Another fixed frequency, variable phase sine wave with lowered level
    leds[k].g = qsub8(sin8(mul2*k + wave2/128), lvl2);        // A fixed frequency, variable phase sine wave with lowered level
    leds[k].b = qsub8(sin8(mul3*k + wave3/128), lvl3);        // A fixed frequency, variable phase sine wave with lowered level
  }
}

void ChangeMe() {
  uint8_t secondHand = (millis() / 1000) % 30;                // Increase this and add more options below if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  if(lastSecond != secondHand) {                              // You can change variables, but remember to set them back in the next demo, or they will stay as is.
    lastSecond = secondHand;
    switch(secondHand) {
      case 0: thisdelay = 25; mul1 = 20; mul2 = 25; mul3 = 22; lvl1 = 80; lvl2 = 80; lvl3 = 80; inc1 = 1; inc2 = 1; inc3 = -1; break;
      case 10: mul1 = 5; mul2 = 8; mul3 = 7; break;
      case 20: thisdelay = 40; lvl1 = 220; lvl2 = 220; lvl3 = 220; break;
      case 30: break;
    }
  }
}
