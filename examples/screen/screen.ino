
#include <LovyanGFX.hpp>
#include "Ryzobee.h"
#include "Touch_CST816T.hpp"

#define SCREEN_WIDTH  240
#define CUBE_WIDTH    40

Ryzobee display(RYZOBEE_ROOTMAKER);
// RootMaker display;

long randNumber_x = (SCREEN_WIDTH - CUBE_WIDTH) / 2;
long randNumber_y = (SCREEN_WIDTH - CUBE_WIDTH) / 2;

void setup(void)
{
  Serial.begin(115200);

  randomSeed(analogRead(A0));

  display.init();

  display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);

  display.fillScreen(TFT_BLACK);

  display.fillRect(randNumber_x, randNumber_y, CUBE_WIDTH, CUBE_WIDTH, 0xf800);// color: red
  display.drawString("Please click the cube", 60, 230);
}

uint32_t count = ~0;
void loop(void)
{
  int32_t x, y;
  if (display.getTouch(&x, &y)) {

    if (x >= randNumber_x && x <= randNumber_x + 40 && y >= randNumber_y && y <= randNumber_y + 40) {
      randNumber_x = random(0, SCREEN_WIDTH - CUBE_WIDTH);
      randNumber_y = random(0, SCREEN_WIDTH - CUBE_WIDTH);
      display.clearDisplay();
      display.fillRect(randNumber_x, randNumber_y, CUBE_WIDTH, CUBE_WIDTH, 0xf800);// color: red
      display.drawString("Please click the cube", 60, 230);

    } else {
      if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_WIDTH) {
        display.fillRect(x-2, y-2, 1, 1, 0x07E0);
      }
    }
  }
}
