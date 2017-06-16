#include "FastLED.h"                                          // FastLED library.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.
CRGB leds[NUM_LEDS];

CRGB targetColor;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set in Volts and miliAmps.

  calib();
}

void loop() {
  ChangeMe();
  fill_solid(leds, NUM_LEDS, targetColor);
  FastLED.show();
}

void calib() {
  fill_solid(leds    , 1, CRGB::Red);
  fill_solid(leds + 1, 2, CRGB::Green);
  fill_solid(leds + 3, 3, CRGB::Blue);
  FastLED.show();
  delay(5000);
}

void ChangeMe() {
  
  uint8_t secondHand = (millis() / 1000) % 9;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                            // Static variable, means it's only defined once. This is our 'debounce' variable.  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case 0: targetColor = CRGB::Red; break;
      case 3: targetColor = CRGB::Green; break;
      case 6: targetColor = CRGB::Blue; break;
    }
  }

}
