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
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequence.
unsigned int  ledLoc = 0;                                     // Current active LED.

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds

  currentBlending = LINEARBLEND;  
  currentPalette = PartyColors_p;
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.
}

void loop() {
 
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,5,100);                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.
    ledLoc = (ledLoc+1) % (NUM_LEDS-1);                       // A simple routine to just move the active LED UP the strip.
    leds[ledLoc] = ColorFromPalette(currentPalette, ledLoc, 255, currentBlending);     // Pick a slightly rotating colour from the Palette
  }

  fadeToBlackBy(leds, NUM_LEDS, 8);                           // Leave a nice comet trail behind.

  FastLED.show();
}
