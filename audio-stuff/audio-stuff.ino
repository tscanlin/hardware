#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>

int analogPin = A4; 
int val = 0;
       
//
//void setup()
//{
//// note that you should not call pinMode for the analog pins
//Serial.begin(9600); // initialise serial port for 9600 baud
//}
//
//void loop()
//{
//
//  val = analogRead(analogPin);   // read the input pin
//  Serial.println(val,DEC);
//  delay(250); // delay 1/4 second between readings
//}

#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 26

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600); // initialise serial port for 9600 baud
  // Uncomment/edit one of the following lines for your leds arrangement.
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() { 
  static uint8_t hue = 0;
  val = analogRead(analogPin);   // read the input pin
  Serial.println(val,DEC);
  FastLED.showColor(CHSV(hue++, 255, 50)); 
  
  delay(10);
  
  // Turn the LED on, then pause
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Green;
//  }
//  FastLED.show();
//  delay(500);  
//  // Now turn the LED off, then pause
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Black;
//  }
//  FastLED.show();
//  delay(500);

}















/*
fft_adc.pde
guest openmusiclabs.com 8.18.12
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

//#define LOG_OUT 1 // use the log output function
//#define FFT_N 256 // set to 256 point fft
//
//#include <FFT.h> // include the library
//
//void setup() {
//  Serial.begin(115200); // use the serial port
//  TIMSK0 = 0; // turn off timer0 for lower jitter
//  ADCSRA = 0xe5; // set the adc to free running mode
//  ADMUX = 0x40; // use adc0
//  DIDR0 = 0x01; // turn off the digital input for adc0
//}
//
//void loop() {
//  while(1) { // reduces jitter
//    cli();  // UDRE interrupt slows this way down on arduino1.0
//    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
//      while(!(ADCSRA & 0x10)); // wait for adc to be ready
//      ADCSRA = 0xf5; // restart adc
//      byte m = ADCL; // fetch adc data
//      byte j = ADCH;
//      int k = (j << 8) | m; // form into an int
//      k -= 0x0200; // form into a signed int
//      k <<= 6; // form into a 16b signed int
//      fft_input[i] = k; // put real data into even bins
//      fft_input[i+1] = 0; // set odd bins to 0
//    }
//    fft_window(); // window the data for better frequency response
//    fft_reorder(); // reorder the data before doing the fft
//    fft_run(); // process the data in the fft
//    fft_mag_log(); // take the output of the fft
//    sei();
//    Serial.write(255); // send a start byte
//    Serial.write(fft_log_out, 128); // send out the data
//  }
//}

