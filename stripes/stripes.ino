#include <FastLED.h>

/*
 * NOTE: This code is experimental, based on this pull request: https://github.com/FastLED/FastLED/pull/202
 * Merge it yourself to get this working...
 */

#define LED_DT      0
#define LED_CK      1
#define NUM_LEDS    28
#define BRIGHTNESS  64
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
uint16_t motionSpeedSmooth;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;

  // Use a "stripe palette" 4-bit palette interpolation.
  currentPalette = RainbowStripeColors_p;

  // With smooth interpolation we can achieve much slower animation motion
  // but retain high framerate.
  motionSpeedSmooth = 128;

  // FastLED Power management set at 5V, 500mA.
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void loop()
{
  static uint16_t startIndexSmooth = 0;

  EVERY_N_MILLISECONDS(25) {
    startIndexSmooth = startIndexSmooth + motionSpeedSmooth;
    stripes(startIndexSmooth);
  }

  FastLED.show();
}

void stripes( uint16_t colorIndex)
{
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPaletteExtended( currentPalette, colorIndex, BRIGHTNESS, currentBlending);

    // this line determines how much of the palette is rendered into the LEDs
    // with 25 LEDs this implies a width of 19200/65536, or about 1/3 of the palette.
    colorIndex += 1024;
  }
}

