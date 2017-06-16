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

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequences.
uint8_t thisbeat =  23;                                       // Beats per minute for first part of dot.
uint8_t thatbeat =  28;                                       // Combined the above with this one.
uint8_t thisfade =   32;                                       // How quickly does it fade? Lower = slower fade rate.
uint8_t  thissat = 255;                                       // The saturation, where 255 = brilliant colours.
uint8_t  thisbri = 255;                                       // Brightness of a sequence.
int        myhue =   0;

int    thisdelay = 50;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.

  currentBlending = LINEARBLEND;
}

void loop () {

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(5*1000) {                                        // Change the target palette to a random one every 5 seconds.
    //static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    sinelon();                                                // Call our sequence.
  }

  FastLED.show();
}

void sinelon() {                                              // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, thisfade);
  int pos1 = beatsin8(thisbeat,0,NUM_LEDS); //beatsin16
  int pos2 = beatsin8(thatbeat,0,NUM_LEDS); //beatsin16

  leds[(pos1+pos2)/2] += ColorFromPalette(currentPalette, myhue++, thisbri, currentBlending);
}



