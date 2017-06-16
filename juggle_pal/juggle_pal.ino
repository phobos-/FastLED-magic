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

CRGBPalette16 currentPalette;                                 // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPalette;                                  // Also support smooth palette transitioning
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND


// Routine specific variables
uint8_t    numdots =   4;                                     // Number of dots in use.
uint8_t   thisfade =   2;                                     // How long should the trails be. Very low value = longer trails.
uint8_t   thisdiff =  16;                                     // Incremental change in hue between each dot.
uint8_t    thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   thisbeat =   5;                                     // Higher = faster movement.

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.

  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;  
}

void loop() {

  EVERY_N_MILLISECONDS(100) {                                 // AWESOME palette blending capability provided as required.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   
  }
 
  ChangeMe();
  juggle_pal();
  FastLED.show();                         // Power managed display of LED's.  
}

void juggle_pal() {                                           // Several colored dots, weaving in and out of sync with each other
  curhue = thishue;                                           // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS)] += ColorFromPalette(currentPalette, curhue , thisbright, currentBlending);    // Munge the values and pick a colour from the palette
    curhue += thisdiff;
  }  
}

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: numdots = 1; thisbeat = 20; thisdiff = 16; thisfade = 2; thishue = 0; break;                  // You can change values here, one at a time , or altogether.
      case 10: numdots = 4; thisbeat = 10; thisdiff = 16; thisfade = 8; thishue = 128; break;
      case 20: numdots = 8; thisbeat =  3; thisdiff =  0; thisfade = 8; thishue=random8(); break;           // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 30: break;
    }
  }  
}

