#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);

void setup(void)
{
    Serial.begin(115200);
  
    board.rootmaker.begin(false, false, true, false, false);
}

void loop(void)
{
  int32_t accelerometer[3];
  board.rootmaker.lis2dwtr->read_acceleration(accelerometer, 3);
  Serial.print("X:\tY:\tZ:\n");
  Serial.print(accelerometer[0]);
  Serial.print("\t");
  Serial.print(accelerometer[1]);
  Serial.print("\t");
  Serial.print(accelerometer[2]);
  Serial.println("\n");
  delay(100);
}

