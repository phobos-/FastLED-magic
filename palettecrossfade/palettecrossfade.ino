#include "FastLED.h"                                          // FastLED library.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.
uint8_t thisdelay = 20;

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(PartyColors_p );
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.

  currentBlending = LINEARBLEND;
}

void loop() {
  
  ChangePalettePeriodically();
 
  // nblendPaletteTowardPalette() will crossfade current palette slowly toward the target palette.
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }

 EVERY_N_MILLISECONDS(thisdelay) {
  static uint8_t startIndex = 0;
  startIndex += 1;                                 // motion speed
  FillLEDsFromPaletteColors(startIndex);
 }

  FastLED.show();
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex + sin8(i*16), 255);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically() {
  
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV(HUE_PURPLE, 255, 255);
    CRGB g = CHSV(HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = CRGBPalette16(g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
    if(secondHand == 20)  { targetPalette = CRGBPalette16(b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
    if(secondHand == 30)  { targetPalette = LavaColors_p; }
    if(secondHand == 40)  { targetPalette = CloudColors_p; }
    if(secondHand == 50)  { targetPalette = PartyColors_p; }
  }
}
