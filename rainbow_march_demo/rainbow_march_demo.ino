#include "FastLED.h"                                          // FastLED library.

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
uint8_t thisdelay = 40;                                       // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
int8_t thisrot = 1;                                           // Hue rotation speed. Includes direction.
uint8_t deltahue = 1;                                         // Hue change between pixels.
bool thisdir = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
}

void loop () {
  
  ChangeMe();
  
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbow_march();
  }

  FastLED.show();
}

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.
  if (thisdir == 0) thishue += thisrot; else thishue-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // I don't change deltahue on the fly as it's too fast near the end of the strip.
}

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisrot=1; deltahue=5; break;
      case  5: thisdir=-1; deltahue=10; break;
      case 10: thisrot=5; break;
      case 15: thisrot=5; thisdir=-1; deltahue=20; break;
      case 20: deltahue=30; break;
      case 25: deltahue=2; thisrot=5; break;
      case 30: break;
    }
  }
}
