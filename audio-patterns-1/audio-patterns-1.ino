#include "FastLED.h"

#define NUM_LEDS 182
#define LEDS_STRIP 26
#define CLOCK_PIN 13
#define LED_PIN 3
#define DATA_PIN 3
#define COLOR_ORDER GRB
#define CHIPSET     WS2811 // WS2811 LPD8806
#define FREQ_COUNT 7

#define BRIGHTNESS  255  // reduce power consumption
#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

CRGB leds[NUM_LEDS]; // Define the array of leds

// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 8
#define pinStrobe 7
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH 191

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

int hue = 0;

void setup() {
  Serial.begin(115200);

  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(LED_DITHER);
  FastLED.show(); // needed to reset leds to zero

  // This will set the IC ready for reading
  MSGEQ7.begin();
}

void loop() {
  // Analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    for (int x = 0; x < FREQ_COUNT; x++) {
      uint8_t val = MSGEQ7.get(x);

      // Reduce noise
      val = mapNoise(val);

      // Visualize leds to the beat
      CRGB color = CRGB::Blue;
      color.nscale8_video(val);

      Serial.println(val);

      int startingLed = x * LEDS_STRIP;
      for (int i = startingLed; i < NUM_LEDS; i++) {
        int limit = val / 4;
        if (i - startingLed <= limit / 2 || (i - startingLed >= LEDS_STRIP - (limit / 2) && i - startingLed < LEDS_STRIP) ) {
          leds[i] = CHSV(hue, 255, 50);
        } else {
          leds[i] = CHSV(hue, 255, 0);
        }
        
      }
      hue++;
    }

    // Update Leds
    FastLED.show();
  }
}
