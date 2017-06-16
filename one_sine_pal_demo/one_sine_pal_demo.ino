#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Initialize changeable global variables. Play around with these!!!
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave.
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 128;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 0;                                        // Brightness of background colour
uint8_t bgclrinc = 0;
int startIndex = 0;

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
}

void loop () {

  ChangeMe();

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    startIndex += thisrot;                                    // Motion speed
    one_sine_pal(startIndex);
  }

  FastLED.show();
}

void one_sine_pal(uint8_t colorIndex) {                                                         // This is the heart of this program. Sure is short.
  
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  
   for (int k=0; k<NUM_LEDS; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
    leds[k] += ColorFromPalette(currentPalette, colorIndex + k, thisbright, currentBlending);
    colorIndex += thisrot;
  }
  bgclr += bgclrinc;                                                                    // You can change the background colour or remove this and leave it fixed.
}

void ChangeMe() {
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: break;
      case  5: targetPalette=RainbowColors_p; bgclr=0; bgbright=32; bgclrinc=1; thisrot=1; thisdelay=10; thisspeed=-4; allfreq=16; thiscutoff=128; currentBlending=NOBLEND; break;
      case 10: targetPalette=ForestColors_p; thisspeed = 12; thisrot = 0; thisspeed = 12; thisdelay = 10; thiscutoff=128; allfreq=64; bgclr = 50; bgbright=15; currentBlending=LINEARBLEND; break;
      //case 15: SetupRandomColorPalette(); thisrot = 2; thisspeed = 16; break;
      case 20: targetPalette=LavaColors_p; thisspeed = 4; break; allfreq = 16; bgclr=50; break;
      case 25: thiscutoff = 96; thisspeed = -4; thiscutoff = 224; bgclr=20; bgbright=8; break;
      //case 30: SetupRandomColorPalette(); thiscutoff = 96; thisrot = 1; break;
      case 35: targetPalette = OceanColors_p; break;
      //case 40: SetupSimilarColorPalette(); thiscutoff = 128; break;
      //case 45: SetupSimilarColorPalette(); thisspeed = 3; break;
      case 50: targetPalette = PartyColors_p; break;
      case 55: break;
      case 60: break;
    }
  }
}

//void SetupRandomColorPalette() {
//    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
//}

//void SetupSimilarColorPalette() {
//    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line, i.e. baseC+random8(0,5) for the hue.
//    targetPalette = CRGBPalette16(CHSV(baseC+random8(0,10), 255, random8(128,255)), CHSV(baseC+random8(0,10), 255, random8(128,255)), CHSV(baseC+random8(0,10), 192, random8(128,255)), CHSV(baseC+random8(0,10), 255, random8(128,255)));
//}
