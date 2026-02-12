#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);


void setup(void)
{
    Serial.begin(115200);
  
    board.rootmaker.begin(false, true, false, false);
}

void loop(void)
{
    rainbowCycle(10);
}

void rainbowCycle(int wait) {
    for (int j = 0; j < 256 * 5; j++) {
      for (int i = 0; i < board.rootmaker.led.strip.numPixels(); i++) {
        board.rootmaker.led.strip.setPixelColor(i, Wheel(((i * 256 / board.rootmaker.led.strip.numPixels()) + j) & 255));
      }
      board.rootmaker.led.strip.show();
      delay(wait);
    }
  }
  
  uint32_t Wheel(byte wheelPos) {
    if (wheelPos < 85) {
      return board.rootmaker.led.strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if (wheelPos < 170) {
      wheelPos -= 85;
      return board.rootmaker.led.strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
      wheelPos -= 170;
      return board.rootmaker.led.strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
  }
