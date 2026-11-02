/*
 * Yoke_TOUCH4 - SC92F837x four-channel touch module, UART 9600 8-N-1.
 * Host TX -> module RX; host RX <- module TX; common ground required.
 */
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_touch4/yoke_touch4.hpp>

Ryzobee board;
Yoke_touch4 touch(UART_NUM_1);
uint32_t lastPoll = 0;
uint32_t lastErrorPrint = 0;
uint8_t lastTouchMask = 0;
bool readFailed = false;

void setup() {
    Serial.begin(115200);
    board.maker.begin();
    auto* port = board.maker.useport("rs_port4");
    if (port == nullptr) {
        Serial.println("RS port 4 is unavailable");
        return;
    }
    // Swap the RS port pin order for this module's UART wiring.
    uint8_t originalTx = port->at(PORT_PIN_START);
    port->at(PORT_PIN_START) = port->at(PORT_PIN_START + 1);
    port->at(PORT_PIN_START + 1) = originalTx;
    Serial.printf("TOUCH4 UART1: TX=GPIO%u RX=GPIO%u, 9600 8-N-1\n",
                  port->at(PORT_PIN_START), port->at(PORT_PIN_START + 1));
    if (touch.begin(port) != YOKE_PORT_ERROR_NONE) {
        Serial.println("TOUCH4 UART initialization failed");
        return;
    }
    delay(500);
    uint16_t version = 0;
    if (touch.getVersion(&version)) {
        Serial.printf("TOUCH4 firmware: 0x%04X\n", version);
    } else {
        Serial.println("Version query failed; check wiring, power and UART firmware");
    }
    if (!touch.setEnabledChannels(0x0F)) {
        Serial.println("Channel enable request failed");
    }
}

void loop() {
    if (!touch.isInitialized()) {
        delay(100);
        return;
    }
    touch.update();
    if (millis() - lastPoll < 100) return;
    lastPoll = millis();
    if (!touch.readData()) {
        if (!readFailed || millis() - lastErrorPrint >= 5000) {
            Serial.println("TOUCH4 read timeout");
            lastErrorPrint = millis();
        }
        readFailed = true;
        return;
    }
    if (readFailed) {
        Serial.println("TOUCH4 communication restored");
        readFailed = false;
    }
    uint8_t mask = touch.getTouchMask();
    uint8_t changed = mask ^ lastTouchMask;
    for (uint8_t channel = 0; channel < Yoke_touch4::CHANNEL_COUNT; channel++) {
        uint8_t bit = 1U << channel;
        if (changed & bit) {
            Serial.printf("Channel %u %s\n", channel + 1,
                          (mask & bit) ? "pressed" : "released");
        }
    }
    lastTouchMask = mask;
}
