// LED Audio Spectrum Analyzer Display
//
// Creates an impressive LED light show to music input
//   using Teensy 3.1 with the OctoWS2811 adaptor board
//   http://www.pjrc.com/store/teensy31.html
//   http://www.pjrc.com/store/octo28_adaptor.html
//
// Line Level Audio Input connects to analog pin A3
//   Recommended input circuit:
//   http://www.pjrc.com/teensy/gui/?info=AudioInputAnalog
//
// This example code is in the public domain.


#include <OctoWS2811.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

// The display size and color to use
const unsigned int matrix_width = 60;
const unsigned int matrix_height = 24;
const unsigned int myColor = 0x000700;    // 1/16 Power Green

// These parameters adjust the vertical thresholds
const float maxLevel = 0.4;      // 1.0 = max, lower is more "sensitive" Oraignal .5 then .3
const float dynamicRange = 40.0; // total range to display, in decibels
const float linearBlend = 0.3;   // useful range is 0 to 0.7


AudioControlSGTL5000 audioShield;

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=217,220
AudioMixer4              mixer1;         //xy=369,319
AudioAnalyzeFFT1024      fft;            //xy=509,319  fft1024_1
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, fft);
AudioControlSGTL5000     sgtl5000_1;     //xy=341,406


// OctoWS2811 objects
const int ledsPerPin = matrix_width * matrix_height / 8;
DMAMEM int displayMemory[ledsPerPin * 6];
int drawingMemory[ledsPerPin * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerPin, displayMemory, drawingMemory, config);

// This array holds the volume level (0 to 1.0) for each
// vertical pixel to turn on.  Computed in setup() using
// the 3 parameters above.
float thresholdVertical[matrix_height];

// This array specifies how many of the FFT frequency bin
// to use for each horizontal pixel.  Because humans hear
// in octaves and FFT bins are linear, the low frequencies
// use a small number of bins, higher frequencies use more.
int frequencyBinsHorizontal[matrix_width] = {
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,
  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,
  5,  5,  6,  6,  6,  7,  7,  7,  8,  8,
  9,  9, 10, 10, 11, 12, 12, 13, 14, 15,
  15, 16, 17, 18, 19, 20, 22, 23, 24, 25
};



// Run setup once
void setup() {
  // the audio library needs to be given memory to start working
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.lineInLevel(6);


  // compute the vertical thresholds before starting
  computeVerticalLevels();

  //fft.windowFunction(AudioWindowTukey1024);
  //fft.windowFunction(AudioWindowBlackmanHarris1024);

  // turn on the display
  leds.begin();
  leds.show();
}

// A simple xy() function to turn display matrix coordinates
// into the index numbers OctoWS2811 requires.  If your LEDs
// are arranged differently, edit this code...
unsigned int xy(unsigned int x, unsigned int y) {
  if ((y & 1) == 0) {
    // even numbered rows (0, 2, 4...) are left to right
    return y * matrix_width + x;
  } else {
    // odd numbered rows (1, 3, 5...) are right to left
    return y * matrix_width + matrix_width - 1 - x;
  }
}
//unsigned int xy(unsigned int x, unsigned int y) {
//    return y * matrix_width + x; //All rows Left to Right
//}

// Run repetitively
void loop() {
  unsigned int x, y, freqBin;
  float level;

  if (fft.available()) {
    // freqBin counts which FFT frequency data has been used,
    // starting at low frequency
    freqBin = 0;

    for (x = 0; x < matrix_width; x++) {
      // get the volume for each horizontal pixel position
      level = fft.read(freqBin, freqBin + frequencyBinsHorizontal[x] - 1);

      // uncomment to see the spectrum in Arduino's Serial Monitor
      // Serial.print(level);
      // Serial.print("  ");

      for (y = 0; y < matrix_height; y++) {
        // for each vertical pixel, check if above the threshold
        // and turn the LED on or off
        if (level >= thresholdVertical[y]) {
          leds.setPixel(xy(x, y), myColor);
        } else {
          leds.setPixel(xy(x, y), 0x000000);
        }
      }
      // increment the frequency bin count, so we display
      // low to higher frequency from left to right
      freqBin = freqBin + frequencyBinsHorizontal[x];
    }
    // after all pixels set, show them all at the same instant
    leds.show();
    // Serial.println();
  }
}


// Run once from setup, the compute the vertical levels
void computeVerticalLevels() {
  unsigned int y;
  float n, logLevel, linearLevel;

  for (y = 0; y < matrix_height; y++) {
    n = (float)y / (float)(matrix_height - 1);
    logLevel = pow10f(n * -1.0 * (dynamicRange / 20.0));
    linearLevel = 1.0 - n;
    linearLevel = linearLevel * linearBlend;
    logLevel = logLevel * (1.0 - linearBlend);
    thresholdVertical[y] = (logLevel + linearLevel) * maxLevel;
  }
}
