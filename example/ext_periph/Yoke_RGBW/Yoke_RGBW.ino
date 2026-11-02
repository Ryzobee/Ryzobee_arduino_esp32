/*
 * Yoke_RGBW.ino — RGBW NeoPixel LED example (3 LEDs)
 *
 * Uses the Yoke library's Yoke_rgbw class (RS port, RMT-based).
 * Drives 3 SK6812 RGBW NeoPixels with per-pixel color control.
 * Data order: LED0 → LED1 → LED2 (closest to MCU first).
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>               // Yoke library base — triggers IDE library discovery
#include <yoke_rgbw/yoke_rgbw.hpp>        // Yoke_rgbw class (src/yoke_rgbw/)

Ryzobee board;
Yoke_rgbw rgbw;

void setup(void) {
    Serial.begin(115200);
    board.maker.begin();
    rgbw.begin(board.maker.useport("rs_port4"));

    // ==== Configure for 3 NeoPixels ====
    rgbw.setNumLeds(3);
    Serial.printf("RGBW ready on PORT4, %d LEDs\n", rgbw.getNumLeds());
}

void loop(void) {
    // ==== 1. All 3 LEDs: RGBW component cycling ====
    rgbw.setColor(255, 0, 0, 0);    rgbw.show(); delay(500);  // All red
    rgbw.setColor(0, 255, 0, 0);    rgbw.show(); delay(500);  // All green
    rgbw.setColor(0, 0, 255, 0);    rgbw.show(); delay(500);  // All blue
    rgbw.setColor(0, 0, 0, 255);    rgbw.show(); delay(500);  // All white
    rgbw.setColor(0);               rgbw.show(); delay(500);  // All off

    // ==== 2. Per-pixel independent colors ====
    rgbw.setPixelColor(0, 255, 0, 0, 0);    // LED0: red
    rgbw.setPixelColor(1, 0, 255, 0, 0);    // LED1: green
    rgbw.setPixelColor(2, 0, 0, 255, 0);    // LED2: blue
    rgbw.show(); delay(1000);

    rgbw.setPixelColor(0, 0, 255, 0);       // LED0: blue
    rgbw.setPixelColor(1, 255, 0, 0);       // LED1: red
    rgbw.setPixelColor(2, 0, 255, 0);       // LED2: green
    rgbw.show(); delay(1000);

    // ==== 3. Chasing effect (single LED runs across) ====
    for (int i = 0; i < 3; i++) {
        rgbw.setColor(0);                           // all off
        rgbw.setPixelColor(i, 255, 255, 255, 0);    // white on LED[i]
        rgbw.show(); delay(200);
    }

    // ==== 4. Brightness sweep on all 3 LEDs (red channel) ====
    for (int b = 0; b <= 255; b++) {
        rgbw.setColor(b, 0, 0, 0);
        rgbw.show(); delay(3);
    }
    for (int b = 255; b >= 0; b--) {
        rgbw.setColor(b, 0, 0, 0);
        rgbw.show(); delay(3);
    }

    // ==== 5. Mixed colors per LED ====
    rgbw.setPixelColor(0, 0xFF0000);    // LED0: red (packed WRGB)
    rgbw.setPixelColor(1, 0x00FFFF00);  // LED1: yellow (W=0, R=255, G=255, B=0)
    rgbw.setPixelColor(2, 0x000000FF);  // LED2: blue
    rgbw.show(); delay(1500);

    rgbw.setColor(0);
    rgbw.show(); delay(500);
}
