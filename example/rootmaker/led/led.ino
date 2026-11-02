#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

static uint16_t hue = 0;
static uint32_t lastStepAt = 0;
static const uint32_t STEP_INTERVAL_MS = 20;

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(true, false, false, false);

    Serial.println();
    Serial.println("RootMaker LED example");
    Serial.println("Cycling the onboard RGB LED with the built-in HSV helper.");
}

void loop(void)
{
    uint32_t now = millis();
    if (now - lastStepAt < STEP_INTERVAL_MS) {
        delay(1);
        return;
    }

    lastStepAt = now;
    board.maker.led.setHSV(hue, 100, 40);
    hue = (hue + 2) % 360;
}
