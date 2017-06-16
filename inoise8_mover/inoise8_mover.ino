#include "FastLED.h"
 
// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette=LavaColors_p;
CRGBPalette16 targetPalette=OceanColors_p;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
static int16_t dist;                                          // A moving location for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.

uint8_t maxChanges = 24;                                      // Value for blending between palettes.

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
  
  dist = random16(12345);                                     // A semi-random number for our noise generator
}
 
void loop() {
  
  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
    
    inoise8_mover();                                          // Update the LED array with noise at the new location
    fadeToBlackBy(leds, NUM_LEDS, 4);     
  }

  //EVERY_N_MILLISECONDS(5*1000) {                                        // Change the target palette to a random one every 5 seconds.
  //  targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  //}
 
  FastLED.show();
}

void inoise8_mover() {

  uint8_t locn = inoise8(xscale, dist+yscale) % 255;          // Get a new pixel location from moving noise.
  uint8_t pixlen = map(locn,0,255,0,NUM_LEDS);                // Map that to the length of the strand.
  leds[pixlen] = ColorFromPalette(currentPalette, pixlen, 255, LINEARBLEND);   // Use that value for both the location as well as the palette index colour for the pixel.

  dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
}

