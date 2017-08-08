#include "FastLED.h"

// How many leds in your strip?
//#define NUM_LEDS 182
//
//#define LED_PIN 3
//#define CLOCK_PIN 13
//
//
//// Define the array of leds
//CRGB leds[NUM_LEDS];
//
//int hue = 100;
//int limit = 5;
//int lastTime = 0;
//int timeDelay = 200;
//
//void setup() {
//  Serial.begin(57600);
//  //  Serial.println("resetting");
//  LEDS.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
//  LEDS.setBrightness(50);
//}
//
//
//void loop()                     
//{
//  int currentTime = millis();
//  if (currentTime - timeDelay > lastTime) {
//    lastTime = currentTime;
//    int val;
//    val = analogRead(0);
//    Serial.print("analog 0 is: ");
//    Serial.println(val);
//    Serial.print("Time: ");
//    Serial.println(millis());
//  
//    limit = val * NUM_LEDS;
//    Serial.println(limit);
//    limit = (limit / 300) + 3;
//    Serial.println(limit);
//    
//    for (int i = 0; i < NUM_LEDS; i++) {
//      if (i < limit) {
//        leds[i] = CHSV(hue, 255, 100);
//      } else {
//        leds[i] = CHSV(hue, 255, 0);
//      }
//      hue++;
//    }
//    FastLED.show();
//  }
//}





#define msg7RESET 8
#define msg7Strobe 7
#define msg7DCout 6
#define LED_PIN 3
#define NUM_LEDS 182
#define LEDS_STRIP 26

//CRGB leds[NUM_LEDS];
//
//int hue = 0;
//int lastRun = 0;
//int loop1 = 50;
//
//void setup() {
//  LEDS.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
//  LEDS.setBrightness(50);
//  
//  Serial.begin(115200);
//  Serial.println("i");
//
//  // initialize the digital pin as an output.
//  // Pin 13 has an LED connected on most Arduino boards:
////  for (int x=0; x<7; x++) {
////      pinMode(LEDpins[x], OUTPUT);
////  }
////  pinMode(msg7RESET, OUTPUT);
////  pinMode(msg7Strobe, OUTPUT);
//
//  for (int i = 0; i < NUM_LEDS; i++) {
////    Serial.println(i);
//    if (i <= 25) {
//      leds[i] = CHSV(0, 255, 30);
//    } else {
//      leds[i] = CHSV(0, 255, 0);
//    }
//  }
//  FastLED.show();
//}
//
//void loop() {
//    int currentTime = millis();
//    if (currentTime < lastRun + loop1) {
//      return;  
//    } else {
//      lastRun = currentTime;
//    }
//  
//    digitalWrite(msg7RESET, HIGH);          // reset the MSGEQ7's counter
//    delay(5);
//    digitalWrite(msg7RESET, LOW);
//
//    int audio2 = analogRead(0);
//    Serial.println(audio2);
//
//    for (int x = 0; x < 7; x++){
//        digitalWrite(msg7Strobe, LOW);      // output each DC value for each freq band
//        delayMicroseconds(35); // to allow the output to settle
//        int spectrumRead = analogRead(msg7DCout);
//
//        Serial.print(spectrumRead);
//        Serial.print(" | ");
//
//        // a
//        spectrumRead = spectrumRead - 10;
//
//        int PWMvalue = map(spectrumRead, 0, 1024, 0, 255); // scale analogRead's value to Write's 255 max
//        if (PWMvalue < 50)
//            PWMvalue = PWMvalue / 2;   // bit of a noise filter, so the LEDs turn off at low levels
//
//        Serial.print(x);
//        Serial.print(" | ");
//        for (int i = 0; i < PWMvalue / 10; i++) {
//          Serial.print("x");
//        }
//        Serial.println("");
//
//        int startingLed = x * LEDS_STRIP;
//        for (int i = startingLed; i < NUM_LEDS; i++) {
//          int limit = PWMvalue / 3;
//          if (i - startingLed <= limit / 2 || (i - startingLed >= LEDS_STRIP - (limit / 2) && i - startingLed < LEDS_STRIP) ) {
//            leds[i] = CHSV(hue, 255, 50);
//          } else {
//            leds[i] = CHSV(hue, 255, 0);
//          }
//          
//        }
//        FastLED.show();
//
//        digitalWrite(msg7Strobe, HIGH);
//    }
//
////    hue++;
//    Serial.println("");
//    Serial.println("");
//    Serial.println("");
//    Serial.println("");
//    Serial.println("");
//    Serial.println("");
//
//}



/*
  Copyright (c) 2014-2016 NicoHood
  See the readme for credit to other people.
  MSGEQ7 FastLED example
  Output via Led strip and FastLED library
  Reads MSGEQ7 IC with 7 different frequencies from range 0-255
  63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLED


#include "FastLED.h"

#define CLOCK_PIN 13
#define LED_PINS 3
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

void setup() {
  Serial.begin(115200);
  
  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PINS, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(LED_DITHER);
  FastLED.show(); // needed to reset leds to zero

  // This will set the IC ready for reading
  MSGEQ7.begin();
}

void loop() {
  int hue = 0;

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
      
  //    fill_solid(leds, val, color); // NUM_LEDS
  
      int startingLed = x * LEDS_STRIP;
      for (int i = startingLed; i < NUM_LEDS; i++) {
        int limit = val / 4;
        if (i - startingLed <= limit / 2 || (i - startingLed >= LEDS_STRIP - (limit / 2) && i - startingLed < LEDS_STRIP) ) {
          leds[i] = CHSV(hue, 255, 50);
        } else {
          leds[i] = CHSV(hue, 255, 0);
        }
        hue++;
      }

     
    }
    
    // Update Leds
    FastLED.show();
  }
}
