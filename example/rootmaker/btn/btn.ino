#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

static bool lastPressed = false;
static bool longPressReported = false;
static uint32_t pressedAt = 0;
static uint32_t lastReleasedAt = 0;
static uint8_t clickCount = 0;

static const uint32_t LONG_PRESS_MS = 1000;
static const uint32_t DOUBLE_CLICK_MS = 300;

void flushClicks(uint32_t now)
{
    if (clickCount == 1 && now - lastReleasedAt > DOUBLE_CLICK_MS) {
        Serial.println("event: short press");
        clickCount = 0;
    }
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(false, false, true, false);

    Serial.println();
    Serial.println("RootMaker button example");
    Serial.println("Press the onboard button to observe state changes.");
}

void loop(void)
{
    board.maker.btns.update(0);
    btn_unit& button = board.maker.btns[0];
    bool pressed = button.isPressed() != 0;
    uint32_t now = millis();

    if (pressed && !lastPressed) {
        pressedAt = now;
        longPressReported = false;
        Serial.println("state: pressed");
    }

    if (pressed && !longPressReported && now - pressedAt >= LONG_PRESS_MS) {
        longPressReported = true;
        Serial.println("event: long press");
    }

    if (!pressed && lastPressed) {
        uint32_t heldFor = now - pressedAt;
        Serial.printf("state: released after %lums\n", heldFor);

        if (heldFor < LONG_PRESS_MS) {
            clickCount++;
            if (clickCount == 2 && now - lastReleasedAt <= DOUBLE_CLICK_MS) {
                Serial.println("event: double click");
                clickCount = 0;
            }
            lastReleasedAt = now;
        } else {
            clickCount = 0;
        }
    }

    flushClicks(now);
    lastPressed = pressed;
    delay(10);
}
