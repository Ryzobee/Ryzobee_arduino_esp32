/*
 * Yoke_KEYW.ino - KEYW key and LED example on an RS expansion port.
 * Poll the debounced key and toggle the LED on each press.
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_keyw/yoke_keyw.hpp>

#define LED_CONTROL (1)  // 0 = digital on/off, 1 = PWM brightness

Ryzobee board;
Yoke_keyw keyw(30);  // Debounce interval in milliseconds.
bool ledState = false;

void setup() {
    Serial.begin(115200);
    board.maker.begin();

    if (keyw.begin(board.maker.useport("rs_port4")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("KEYW initialization failed");
        return;
    }
    Serial.println("KEYW ready on RS port 4");
}

void loop() {
    if (!keyw.isInitialized()) {
        delay(100);
        return;
    }
    keyw.update();
    if (keyw.wasPressed()) {
        ledState = !ledState;
#if LED_CONTROL
        keyw.setLEDBrightness(ledState ? 255 : 0);
#else
        keyw.setLED(ledState);
#endif
    }
    delay(1);
}
