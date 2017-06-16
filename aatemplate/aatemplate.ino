#include "FastLED.h"                                          // FastLED library.

#define qsubd(x, b) ((x>b)?b:0)                               // Clip. . . . A digital unsigned subtraction macro. if result <0, then x=0. Otherwise, x=b.
#define qsuba(x, b) ((x>b)?x-b:0)                             // Level shift. . . Unsigned subtraction macro. if result <0, then x=0. Otherwise x=x-b.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequences.
int      twinkrate = 100;                                     // The higher the value, the lower the number of twinkles.
uint8_t  thisdelay =  10;                                     // A delay value for the sequence(s).
uint8_t   thisfade =   8;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thishue =  50;                                     // The hue.
uint8_t    thissat = 255;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
bool       randhue =   1;                                     // Do we want random colours all the time? 1 = yes.

void setup() { 
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Set up APA102 leds

  currentBlending = LINEARBLEND;  
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
}

void loop () {

  ChangeMe();                                                 // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    twinkle();
  }

  EVERY_N_MILLISECONDS(5000) {                                        // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

  FastLED.show();  
}

void twinkle() {
  if (random8() < twinkrate) leds[random16(NUM_LEDS)] += ColorFromPalette(currentPalette, (randhue ? random8() : thishue), 255, currentBlending);
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
}

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 10;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case 0: thisdelay = 10; randhue = 1; thissat=255; thisfade=8; twinkrate=150; break;  // You can change values here, one at a time , or altogether.
      case 5: thisdelay = 100; randhue = 0;  thishue=random8(); thisfade=2; twinkrate=20; break;
      case 10: break;
    }
  }
}
