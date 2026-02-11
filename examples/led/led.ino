#include <Adafruit_NeoPixel.h>

#include "Ryzobee.h"
#define PIN        45
#define NUM_LEDS   1

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(100);
  strip.show();
}

void loop() {
  rainbowCycle(10);
}

void rainbowCycle(int wait) {
  for (int j = 0; j < 256 * 5; j++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte wheelPos) {
  if (wheelPos < 85) {
    return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  } else if (wheelPos < 170) {
    wheelPos -= 85;
    return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  } else {
    wheelPos -= 170;
    return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
}
