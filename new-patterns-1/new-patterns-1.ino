#include "FastLED.h"

#define NUM_LEDS 26
#define DATA_PIN 3

#define SEQUENCE_COUNT 2
#define SEQUENCE_PROPS 4

CRGB leds[NUM_LEDS];

int arr [NUM_LEDS][3]; // 3=h,s,v
int patterns [2][2]; // start,end

char sequence[SEQUENCE_COUNT][SEQUENCE_PROPS][20] = {
  {"17", "NULL", "NULL", "43"},
  {"20", "NULL", "NULL", "0"}
};


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, 60);
  Serial.begin(9600);
  patterns[0][0] = 0;
  patterns[0][1] = 13;
  patterns[1][0] = 14;
  patterns[1][1] = 26;
  

  static int count = 1;
  static int hue = 0;
  static int saturation = 255;
  static int brightness = 50;
  
  static int dindex = 0;
  
  
//  for (int i = 0; i < SEQUENCE_COUNT; i++) {
//    for (int j = 0; j < SEQUENCE_PROPS; j++) {
//      int count;
//      Serial.println(sequence[i][j]);
//      if (isdigit(sequence[i][j][0])) {
//        
//        int num = atoi(sequence[i][j]);
//        
//        Serial.print(j);
//        Serial.print(" - ");
//        Serial.print(num);
////        if (j == 0) {
////          count = atoi(sequence[i][j]);
////        } else if (j == 1) {
////          hue = atoi(sequence[i][j]);
////        } else if (j == 2) {
////          saturation = atoi(sequence[i][j]);
////        } else if (j == 3) {
////          brightness = atoi(sequence[i][j]);
////        }
//      }
//      
//      int start = 0;
//      while (start < count) {
//        arr[dindex][0] = hue;
//        arr[dindex][1] = 255;
//        arr[dindex][2] = brightness;
//        start++;
//        dindex++;
//      }
//      
//      Serial.println(arr[dindex][0]);
//    }
//  }

}

void loop() {
  static uint8_t hue = 0;
  static uint8_t saturation = 255;
  static uint8_t brightness = 50;
  static uint8_t index = 0;
  static uint8_t len = 8;


  for (int i = 0; i < NUM_LEDS; i++){
    if(i <= index && i > index - len) {
      arr[i][0] = hue;
      arr[i][1] = 255;
      arr[i][2] = brightness;
      if (index - i < 3) {
        arr[i][2] = brightness * (index - i) / 3;
      } else if (index - i > len - 3) {
        arr[i][2] = brightness * (abs(index - i - len)) / 3;
      }
    } else if (i == index - len) {
      arr[i][0] = hue;
      arr[i][1] = 255;
      arr[i][2] = 0;
    }
  }

  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(arr[i][0], arr[i][1], arr[i][2]);
  }

  
  hue += 7;
  index++;
  if (index - len > NUM_LEDS) {
    index = 0;
  }
  
  FastLED.show();
  delay(100);
}
