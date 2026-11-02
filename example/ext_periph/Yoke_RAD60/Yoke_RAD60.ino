/*
 * Yoke_RAD60.ino — AT6010 mmWave radar example
 *
 * Uses the Yoke library's Yoke_rad60 class (RS port, UART-based).
 * SDA → TX, SCL → RX. Baud rate fixed at 115200.
 * Reports the first target only: distance, angle, velocity, and ID.
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>                   // Yoke library base — triggers IDE discovery
#include <yoke_rad60/yoke_rad60.hpp>          // Yoke_rad60 class

#define PRINT_INTERVAL_MS  200

Ryzobee board;
Yoke_rad60 radar(UART_NUM_1, 115200);        // UART1, 115200 baud

uint32_t last_print_ms = 0;
bool     last_detected  = false;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=== RAD60 mmWave radar ===");

    board.maker.begin();

    // Init radar on RS port 4 (SDA=TX=GPIO17, SCL=RX=GPIO18)
    radar.begin(board.maker.useport("rs_port4"));

    Serial.println("[RAD] Waiting for radar startup (2s)...");
    delay(2000);

    Serial.println("[RAD] Trying communication mode...");
    radar.enterCommunicationMode(true);
    delay(200);

    Serial.println("[RAD] Testing communication...");
    uint8_t state = 0xFF;
    if (radar.getRadarState(&state)) {
        Serial.printf("[RAD] OK (state: 0x%02X)\n", state);
    } else {
        Serial.println("[FATAL] No response — check wiring/power!");
        while (1) { delay(1000); }
    }

    Serial.println("[RAD] Applying default config...");
    radar.applyDefaultConfig();

    Serial.println("[SYS] Data acquisition started...");
    Serial.println("----------------------------------------");
}

void loop() {
    radar.update();

    bool detected = radar.isDetected();
    if (detected != last_detected) {
        last_detected = detected;
        Serial.println(detected ? "[EVENT] >>> Target entered <<<"
                                : "[EVENT] >>> Target left <<<");
    }

    if (millis() - last_print_ms >= PRINT_INTERVAL_MS) {
        last_print_ms = millis();
        if (!radar.isDetected()) {
            static uint32_t n = 0;
            if (++n % 10 == 0) Serial.println("[RAD] Monitoring... (vacant)");
            return;
        }
        Serial.println("--- Target detected ---");
        Serial.printf("  dist=%u cm angle=%d deg vel=%d cm/s id=%u\n",
                      radar.getTargetDistance(),
                      radar.getTargetAngle(),
                      radar.getTargetVelocity(),
                      radar.getTargetID());
        Serial.println("-----------------------");
    }

    if (!radar.isConnected()) {
        Serial.println("[WARN] Comm error!");
        delay(1000);
    }
}
