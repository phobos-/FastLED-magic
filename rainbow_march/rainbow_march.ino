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
uint8_t thisdelay = 5;                                        // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
}

void loop () {
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking routine to update/display the sequence.
    rainbow_march();
  }
  FastLED.show(); 
}

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
  thishue++;                                                  // Increment the starting hue.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
}
