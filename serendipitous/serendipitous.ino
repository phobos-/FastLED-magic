/* serendipitous
 *  
 *  This is from the Serendipitous Circles article in the August 1977 and April 1978 issues of Byte Magazine. This is a poorly rendered 1D version and I really should get around to converting it to 2D and buying a 32x32 matrix to display it. Some day . . 
 *  
 *  Check the magazine article out online, as this 1D routine doesn't do it justice.
 *  
 *  
 *  Base Routine is:
 *  
 *  Choose X, Y
 *  Top: Display point
 *  Xn = X-Y/2
 *  Yn = Y+Xn/2
 *  X = Xn
 *  Y = Yn
  *  Goto top
 *  
 *  Other maths are:
 *  Xn = X-Y/2,   Yn = Y+Xn/2
 *  Xn = X-(Y/2), Yn = Y+(X/2.1)
 *  Xn = X-(Y/3), Yn = Y+(X/1.5)
 *  Xn = X-(2*Y), Yn = Y+(X/1.1)
 *    
 */

#include "FastLED.h"                                          // FastLED library. Please use the latest development version.

// Fixed definitions cannot change on the fly.
#define LED_DT 0                                              // Data pin to connect to the strip.
#define LED_CK 1                                              // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 28                                           // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.

CRGB leds[NUM_LEDS];

uint8_t maxChanges = 24;      // Value for blending between palettes.
 
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 

uint16_t Xorig = 0x012;
uint16_t Yorig = 0x015;
uint16_t X;
uint16_t Y;
uint16_t Xn;
uint16_t Yn;
uint8_t index;

void setup () {
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS); // Set up APA102 leds
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                      // FastLED Power management set at 5V, 500mA.

  currentBlending = LINEARBLEND;

  //targetPalette = RainbowColors_p;

  X=Xorig;
  Y=Yorig;
}

void loop () {

  EVERY_N_MILLISECONDS(60) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
  }

  EVERY_N_MILLISECONDS(50) {
    serendipitous();     
  }

  FastLED.show();
}

void serendipitous () {

  EVERY_N_MILLISECONDS(5*1000) {
    //uint8_t baseC = random8();
    //targetPalette = CRGBPalette16(CHSV(baseC-3, 255, random8(192,255)), CHSV(baseC+2, 255, random8(192,255)), CHSV(baseC+5, 192, random8(192,255)), CHSV(random8(), 255, random8(192,255)));

    X = Xorig;
    Y = Yorig;    
  }

//  Xn = X-(Y/2); Yn = Y+(Xn/2);
//  Xn = X-Y/2;   Yn = Y+Xn/2;
//  Xn = X-(Y/2); Yn = Y+(X/2.1);
  Xn = X-(Y/3); Yn = Y+(X/1.5);
//  Xn = X-(2*Y); Yn = Y+(X/1.1);
  
  X = Xn;
  Y = Yn;

  index=(sin8(X)+cos8(Y))/2;                            // Guarantees maximum value of 255

  CRGB newcolor = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);
  
  nblend(leds[X%NUM_LEDS-1], newcolor, 224);          // Try and smooth it out a bit. Higher # means less smoothing.
//  nblend(leds[map(X,0,65535,0,NUM_LEDS)], newcolor, 224); // Try and smooth it out a bit. Higher # means less smoothing.
  
  fadeToBlackBy(leds, NUM_LEDS, 16);                    // 8 bit, 1 = slow, 255 = fast
}

