/*  OctoWS2811 Rainbow.ino - Rainbow Shifting Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
    pin 1 - Output indicating CPU usage, monitor with an oscilloscope,
            logic analyzer or even an LED (brighter = CPU busier)
*/

#include <OctoWS2811.h>
//#include <stdio.h>

const int ledsPerStrip = 180; // 180
const int totalColors = 180;
const int stripCount = 8; // 8
const int totalLeds = 1440;
const int hoopCount = 24;
const int ledsPerHoop = 60;
const int lightnessLimit = 5;

DMAMEM int displayMemory[ledsPerStrip * stripCount];
int drawingMemory[ledsPerStrip * stripCount];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int rainbowColors[totalColors * (lightnessLimit + 1)]; // 1 is for 0 color.


// SETUP
void setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  delay(2000);
  Serial.begin(9600);
  Serial.println("Setup");
  int index = 0;
  for (int j = lightnessLimit; j >= 0; j--) {
    for (int i = 0; i < totalColors; i++) {
      int hue = i * 2;
      int saturation = 100;
      int lightness = j;
      index = (abs(lightnessLimit - j) * 180) + i;
      // pre-compute the 180 rainbow colors
      rainbowColors[index] = makeColor(hue, saturation, lightness);
    }
  }
  digitalWrite(1, LOW);
  leds.begin();
}

// LOOP
void loop() {
  Serial.println("Loop");
  //  sparklingInRandomly(5500);
  //  rainbowRingsGoingOutFade(5500);
  //  rainbow(5500000, 180);

//    rainbowRingsGoingOut(5500);
//    raibowSyncCircleAround(5500);

//    int i = random(3);
//    shuffle(i);

//  hRings(8, 12000);
//  vRings(8, 20000);
  sRandom(8, 20000);
    
  ///rainbowRingsGoingOutFade2(5500, 1);
}

void shuffle(int n) {
  switch(n) {
    case 0:
      rainbowRingsGoingOut(5500);
      break;
    case 1:
      raibowSyncCircleAround(5500);
      break;
    case 2:
      solidRainbow(180, 5500);
      break;
    default:
      rainbowRingsGoingOut(5500);
  }
}
////////////


// FUNCTIONS

void hRings(int duration, int delay1) {
  int counter = 0;
  int color = 0;
  digitalWrite(1, HIGH);
  while (counter < (duration * 1000)) {
    for (int y = 0; y < ledsPerHoop; y++) {
      for (int x = 0; x < hoopCount; x++) {
        leds.setPixel((x * 60) + y, rainbowColors[color]);
      }
      color = (color + 5) % totalColors;
      if (color > 160) {
        color += 180 * 4;
      }
      leds.show();
      delayMicroseconds(delay1);
      counter += (delay1 / 1000); // offset
    }
  }
  digitalWrite(1, LOW);
}

void vRings(int duration, int delay1) {
  int counter = 0;
  int color = 0;
  digitalWrite(1, HIGH);
  while (counter < (duration * 1000)) {
    for (int x = 0; x < totalLeds; x++) {
      leds.setPixel(x, rainbowColors[color]);
      if (x % ledsPerHoop == 0) {
        color = (color + 9) % totalColors;
        leds.show();
        delayMicroseconds(delay1);
        counter += (delay1 / 1000); // offset
      }
    }
  }
  digitalWrite(1, LOW);
}

void sRandom(int duration, int delay1) {
  int counter = 0;
  int color = 0;
  digitalWrite(1, HIGH);
  while (counter < (duration * 1000)) {
      leds.setPixel(random(totalLeds), rainbowColors[color]);
      color = (color + 9) % totalColors;
      if (color % 5 == 0) {
        leds.show();
      }
//      delayMicroseconds(delay1);
      counter += (delay1 / 1000); // offset
  }
  digitalWrite(1, LOW);
}












//////// FRESH LINE

int rainbowRingsGoingAroundColor = 0;
void rainbowRingsGoingAround(int cycleTime) { // TOO SIMILAR...
  int color, x, y, z, wait;
  wait = cycleTime; // cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  z = 0;
  digitalWrite(1, HIGH);

  int toggle = 0;
  while (x < 180 * 2) {
    for (y = 0; y < 60; y++) {
      z = (60 * x) + y;
      z %= 1440;
      leds.setPixel(z, rainbowColors[rainbowRingsGoingAroundColor]);
    }
    if (toggle == 0) {
      rainbowRingsGoingAroundColor++;
    } else {
      rainbowRingsGoingAroundColor--;
    }
    if (rainbowRingsGoingAroundColor >= 180) {
      toggle = 1;
    }
    leds.show();
    delayMicroseconds(10000); // 10000
    x++;
  }

  digitalWrite(1, LOW);
}



int rainbowRingsGoingOutFadeColor = 0;
void rainbowRingsGoingOutFade(int cycleTime) { // GOOD
  int color, x, y, z, wait;
  wait = cycleTime; // cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  digitalWrite(1, HIGH);

  for (y = 0; y < ledsPerHoop; y++) {
    for (x = 0; x < hoopCount; x++) {
      z = (x * ledsPerHoop) + y;
      //      rainbowRingsGoingOutFadeColor = y;
      //      if (y < 2) {
      //        rainbowRingsGoingOutFadeColor -= 180 * 2;
      //      } else if (y > 56) {
      //        rainbowRingsGoingOutFadeColor += 180;
      //      }
      rainbowRingsGoingOutFadeColor += 17;
      rainbowRingsGoingOutFadeColor %= 180;
      //      Serial.println("color: %d", rainbowRingsGoingOutFadeColor);
      Serial.println(rainbowRingsGoingOutFadeColor);
      leds.setPixel(z, rainbowColors[rainbowRingsGoingOutFadeColor]);
    }
    leds.show();
    delayMicroseconds(cycleTime * 10);
  }

  digitalWrite(1, LOW);
}


int rainbowRingsGoingOutDarkerColor = 0;
void rainbowRingsGoingOutDarker(int cycleTime) { // GOOD
  int color, x, y, z, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  digitalWrite(1, HIGH);

  while (x < totalLeds) {
    leds.setPixel(y, rainbowColors[rainbowRingsGoingOutDarkerColor]);
    x++;
    y += 60;
    if (x % 24 == 0) {
      leds.show();
      delayMicroseconds(cycleTime * 5);
      y++;
      rainbowRingsGoingOutDarkerColor += 7; // z++;
    }
    y %= totalLeds;
    rainbowRingsGoingOutDarkerColor %= 1080; // 180
  }
  digitalWrite(1, LOW);
}


void raibowSyncCircleAround(int cycleTime) { // GOOD
  int color, x, y, z, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  z = 0;
  digitalWrite(1, HIGH);

  while (x < totalLeds) {
    leds.setPixel(x, rainbowColors[y]);
    x++;
    if (x % 60 == 0) {
      y += 2; //6
      leds.show();
      delayMicroseconds(cycleTime * 10);
    }
//    x %= totalLeds;
    y %= 180;
  }
  digitalWrite(1, LOW);
}


void raibowRingLineAround(int cycleTime) {
  int color, x, y, z, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  z = 0;
  digitalWrite(1, HIGH);

  while (x < 14400) {
    leds.setPixel(x, rainbowColors[y]);
    leds.show();
    x++;
    if (x % 120 == 0) {
      y += 12;
    }
    x %= totalLeds;
    y %= 180;
  }
  digitalWrite(1, LOW);
}


int rainbowRingsGoingOutColor = 0;
void rainbowRingsGoingOut(int cycleTime) { // GOOD
  int color, x, y, z, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  digitalWrite(1, HIGH);

  while (x < totalLeds) {
    leds.setPixel(y, rainbowColors[rainbowRingsGoingOutColor]);
    x++;
    y += 60;
    if (x % 24 == 0) {
      leds.show();
      delayMicroseconds(cycleTime * 5);
      y++;
      rainbowRingsGoingOutColor += 7; // z++;
    }
    y %= totalLeds;
    rainbowRingsGoingOutColor %= 180;
  }
  digitalWrite(1, LOW);
}


void solidColorRingsGoingOut(int cycleTime) {
  int color, x, y, z, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  z = 0;
  digitalWrite(1, HIGH);
  while (z < 180) {
    z %= 180;
    x = 0;
    while (x < totalLeds) {
      y += 60;
      y %= totalLeds;
      if (x % 24 == 0) {
        y++;
      }
      leds.setPixel(y, rainbowColors[z]);
      leds.show();
      x++;
    }
    z += 45;
  }
  digitalWrite(1, LOW);
}


void sparklingInRandomly(int cycleTime) {
  int color, x, y, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  x = 0;
  y = 0;
  digitalWrite(1, HIGH);
  while (x < 1440) {
    //    y = y + (x * 60);
    //    if (y > 1440) {
    //      y++;
    //      y %= 1440;
    //    }
    y = random(1440);
    leds.setPixel(y, rainbowColors[random(180)]);
    leds.show();
    x++;
  }
  digitalWrite(1, LOW);
}


//int currentColor = 0;
//void raibowLine(int cycleTime) {
//  int color, x, y, wait;
//  wait = cycleTime * 1000 / ledsPerStrip;
//  x = 0;
//  y = 0;
//  digitalWrite(1, HIGH);
//  while (x <= 1440) {
//    x++;
//    if (x % 60 == 0) {
//      currentColor = random(180);
//    }
//    leds.setPixel(x, rainbowColors[currentColor]);
//    leds.show();
//  }
////  currentColor += 30 % 180;
//  digitalWrite(1, LOW);
//}


// First one. lol
void solidRainbow(int phaseShift, int cycleTime) {
  int color, x, y, wait;
  wait = cycleTime * 1000 / ledsPerStrip;
  for (color = 0; color < 180; color++) {
    digitalWrite(1, HIGH);
    for (x = 0; x < ledsPerStrip; x++) {
      for (y = 0; y < stripCount; y++) {
        int index = color; // (color + x + y*phaseShift/2) % 180;
        leds.setPixel(x + y * ledsPerStrip, rainbowColors[index]);
      }
    }
    leds.show();
    digitalWrite(1, LOW);
    delayMicroseconds(wait * 2);
  }
}


// phaseShift is the shift between each row.  phaseShift=0
// causes all rows to show the same colors moving together.
// phaseShift=180 causes each row to be the opposite colors
// as the previous.
//
// cycleTime is the number of milliseconds to shift through
// the entire 360 degrees of the color wheel:
// Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
//
void rainbow(int phaseShift, int cycleTime) {
  int color, x, y, wait;

  wait = cycleTime * 1000 / ledsPerStrip;
  for (color = 0; color < 180; color++) {
    digitalWrite(1, HIGH);
    for (x = 0; x < ledsPerStrip; x++) {
      for (y = 0; y < stripCount; y++) {
        int index = (color + x + y * phaseShift / 2) % 180;
        leds.setPixel(x + y * ledsPerStrip, rainbowColors[index]);
      }
    }
    leds.show();
    digitalWrite(1, LOW);
    delayMicroseconds(wait);
  }
}


