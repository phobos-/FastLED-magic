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
uint8_t thisdelay = 20;                                       // A delay value for the sequence(s)

int wave1=0;                                                  // Current phase is calculated.
int wave2=0;
int wave3=0;

uint8_t lvl1 = 80;                                            // Any result below this value is displayed as 0.
uint8_t lvl2 = 80;
uint8_t lvl3 = 80;

uint8_t mul1 = 7;                                            // Frequency, thus the distance between waves
uint8_t mul2 = 6;
uint8_t mul3 = 5;

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(PartyColors_p);

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
}

void loop () {
  
  ChangeMe();
  
  uint8_t maxChanges = 24;
 
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
    three_sin();                                              // Improved method of using non-blocking delay
  }
  
  FastLED.show();
}

void three_sin() {
  
  wave1 += beatsin8(10,-4,4);
  wave2 += beatsin8(15,-2,2);
  wave3 += beatsin8(12,-3, 3);

  for (int k=0; k<NUM_LEDS; k++) {
    uint8_t tmp = sin8(mul1*k + wave1) + sin8(mul1*k + wave2) + sin8(mul1*k + wave3);
    leds[k] = ColorFromPalette(currentPalette, tmp, 255);
  }
  
}

void ChangeMe() {
  
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB u = CHSV( HUE_BLUE, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;

    switch(secondHand) {
      case  0: targetPalette = RainbowColors_p; break;
      case  5: targetPalette = CRGBPalette16( u,u,b,b, p,p,b,b, u,u,b,b, p,p,b,b); break;
      case 10: targetPalette = OceanColors_p; break;
      case 15: targetPalette = CloudColors_p; break;
      case 20: targetPalette = LavaColors_p; break;
      case 25: targetPalette = ForestColors_p; break;
      case 30: targetPalette = PartyColors_p; break;
      //case 35: targetPalette = CRGBPalette16( b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); break;
      //case 40: targetPalette = CRGBPalette16( u,u,u,w, u,u,u,w, u,u,u,w, u,u,u,w); break;
      //case 45: targetPalette = CRGBPalette16( u,p,u,w, p,u,u,w, u,g,u,w, u,p,u,w); break;
      case 50: targetPalette = CloudColors_p; break;
      case 55: targetPalette = CRGBPalette16( u,u,u,w, u,u,p,p, u,p,p,p, u,p,p,w); break;
      case 60: break;
    }
  }
}

