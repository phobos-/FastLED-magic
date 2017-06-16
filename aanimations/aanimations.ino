#include "FastLED.h"                                          // FastLED library.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.

// have 3 independent CRGBs - 2 for the sources and one for the output
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set in Volts and miliAmps.
}

void loop() {

  animationA();                                               // render the first animation into leds2   
  animationB();                                               // render the second animation into leds3

  uint8_t ratio = beatsin8(2);                                // Alternate between 0 and 255 every minute
  
  for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
    leds[i] = blend( leds2[i], leds3[i], ratio );
  }

  FastLED.show();
}

void animationA() {                                             // running red stripe.

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 10) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
    leds2[i] = CRGB(red, 0, 0);
  }
}

void animationB() {                                               // running green stripe in opposite direction.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t green = (millis() / 5) - (i * 12);                    // speed, length
    if (green > 128) green = 0;
    leds3[i] = CRGB(0, green, 0);
  }
}
