#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

void setup(void)
{
  Serial.begin(115200);
  delay(200);

  board.maker.begin(false, true, false, false);

  Serial.println();
  Serial.println("RootMaker LIS2DWTR example");
  Serial.println("Printing acceleration and tilt values every 200ms.");
}

void loop(void)
{
  board.maker.gsensor.flush();

  Serial.printf("X: %.3fg, Y: %.3fg, Z: %.3fg, roll: %.2f deg, pitch: %.2f deg\n",
          board.maker.gsensor.x,
          board.maker.gsensor.y,
          board.maker.gsensor.z,
          board.maker.gsensor.roll,
          board.maker.gsensor.pitch);

  delay(200);
}
