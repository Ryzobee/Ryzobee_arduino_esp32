/*
 * Yoke_MOTO_A.ino — F-BUS motor driver example
 *
 * Uses the Yoke library's Yoke_moto_a class (F-BUS port, MCPWM-based).
 * F_BUS17 (GPIO6) = IN1/A, F_BUS19 (GPIO4) = IN2/B.
 * Uses MCPWM TIMER_1 — can run simultaneously with RS-port Yoke_moto (TIMER_0).
 * Speed range: 0–100 (maps to PWM duty 0–100 by default).
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>                   // Yoke library base — triggers IDE discovery
#include <yoke_moto_a/yoke_moto_a.hpp>        // Yoke_moto_a class (src/yoke_moto_a/)

Ryzobee board;
Yoke_moto_a moto;

void setup() {
    Serial.begin(115200);
    board.maker.begin();

    moto.begin(board.maker.useport("f_bus"));
    if (!moto.isInitialized()) {
        Serial.println("[FATAL] MOTO-A init failed — check F-BUS port data in rootmaker.hpp");
        while (1) { delay(1000); }
    }

    Serial.println("MOTO-A ready on F_BUS17/F_BUS19 (GPIO6/GPIO4)");
}

void loop() {
    // Forward 2s → stop 1s → backward 2s → stop 1s
    moto.setmoto(Yoke_moto_a::DIR_FORWARD, 100);  delay(2000);
    moto.setmoto(Yoke_moto_a::DIR_FORWARD, 0);    delay(1000);
    moto.setmoto(Yoke_moto_a::DIR_REVERSE, 100);  delay(2000);
    moto.setmoto(Yoke_moto_a::DIR_REVERSE, 0);    delay(1000);

    // Speed ramp forward (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto_a::DIR_FORWARD, i);
        delay(10);
    }

    // Speed ramp reverse (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto_a::DIR_REVERSE, i);
        delay(10);
    }

    moto.setmoto(Yoke_moto_a::DIR_FORWARD, 0);  // stop
    delay(1000);
}
