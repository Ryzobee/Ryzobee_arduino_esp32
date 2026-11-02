/*
 * Yoke_MOTO.ino — Motor driver module example
 *
 * Uses the Yoke library's Yoke_moto class (RS port, MCPWM-based DC motor).
 * Speed range: 0–100 (maps to PWM duty 0–255 by default).
 *
 * NOTE: Unconfigured RS port pins float (high-impedance input).
 * If a motor driver is connected to an uninitialized port, the floating
 * inputs can partially turn on the H-bridge, making the motor spin/twitch.
 * Always ground unused ports via groundUnusedPorts().
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>               // Yoke library base — triggers IDE library discovery
#include <yoke_moto/yoke_moto.hpp>        // Yoke_moto class (src/yoke_moto/)

Ryzobee board;
Yoke_moto moto;

// ==== Ground all RS port pins except the one in use ====
// Each RS port has 2 pins: SDA (key) + SCL (LED/motor).
// Without this, floating inputs on unused ports can cause
// H-bridge motor drivers to partially conduct.
void groundUnusedPorts(const char* usedPort) {
    const char* allPorts[] = {"rs_port1", "rs_port2", "rs_port3", "rs_port4"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(allPorts[i], usedPort) == 0) continue;
        auto* p = board.maker.useport(allPorts[i]);
        if (p == nullptr || p->empty()) continue;
        int sda = p->at(PORT_PIN_START);
        int scl = p->at(PORT_PIN_START + 1);
        pinMode(sda, OUTPUT);
        pinMode(scl, OUTPUT);
        digitalWrite(sda, LOW);
        digitalWrite(scl, LOW);
        Serial.printf("Grounded %s: GPIO%d GPIO%d\n", allPorts[i], sda, scl);
    }
}

void setup() {
    Serial.begin(115200);
    board.maker.begin();

    // Ground unused RS ports to prevent floating H-bridge inputs
    groundUnusedPorts("rs_port4");

    // Diagnostic: verify which pins are being used
    auto* port = board.maker.useport("rs_port4");
    Serial.printf("PORT4 pins: SDA=GPIO%d SCL=GPIO%d\n",
                  port->at(PORT_PIN_START), port->at(PORT_PIN_START + 1));

    moto.begin(port);
    Serial.println("MOTO ready on PORT4");
}

void loop() {
    // Forward 2s → stop 1s → backward 2s → stop 1s
    moto.setmoto(Yoke_moto::DIR_FORWARD, 100);  delay(2000);
    moto.setmoto(Yoke_moto::DIR_FORWARD, 0);    delay(1000);
    moto.setmoto(Yoke_moto::DIR_REVERSE, 100);  delay(2000);
    moto.setmoto(Yoke_moto::DIR_REVERSE, 0);    delay(1000);

    // Speed ramp forward (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto::DIR_FORWARD, i);
        delay(10);
    }

    // Speed ramp reverse (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto::DIR_REVERSE, i);
        delay(10);
    }

    moto.setmoto(Yoke_moto::DIR_FORWARD, 0);  // stop
    delay(1000);
}
