# RGBW LED Example

[中文](./README_CN.md)

This example demonstrates how to control SK6812 RGBW NeoPixel LEDs using the **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** library's `Yoke_rgbw` class. It uses the ESP32 RMT peripheral (new driver, ESP-IDF v5.x) to drive up to N LEDs on a single RS port data pin (SDA).

# Features

- Configure LED count at runtime: `rgbw.setNumLeds(N)`
- Per-pixel color: `rgbw.setPixelColor(index, r, g, b, w)` or packed 32-bit `setPixelColor(index, 0xWWRRGGBB)`
- All-pixel fill: `rgbw.setColor(r, g, b, w)` (backward compatible)
- Explicit transmit: `rgbw.show()` — call after setting colors to push data to the strip
- Chasing effects, brightness sweeps, mixed colors

# Hardware

| Port | NeoPixel Strip |
|------|----------------|
| SDA  | Data line (DIN)|
| SCL  | Unused         |

Rootmaker GPIO mapping:

| Port  | SDA (DIN) |
|-------|-----------|
| PORT1 | GPIO15    |
| PORT2 | GPIO19    |
| PORT3 | GPIO13    |
| PORT4 | GPIO17    |

# Dependencies

| Library | Description |
|---------|-------------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | Board support (Rootmaker) |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | Peripheral drivers (RMT-based, no Adafruit_NeoPixel needed) |

# Quick Start

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_rgbw/yoke_rgbw.hpp>

Ryzobee board;
Yoke_rgbw rgbw;

void setup() {
    board.maker.begin();
    rgbw.begin(board.maker.useport("rs_port4"));
    rgbw.setNumLeds(3);                          // 3 NeoPixels
}

void loop() {
    rgbw.setPixelColor(0, 255, 0, 0, 0);        // LED0: red
    rgbw.setPixelColor(1, 0, 255, 0, 0);        // LED1: green
    rgbw.setPixelColor(2, 0, 0, 255, 0);        // LED2: blue
    rgbw.show();
    delay(1000);

    rgbw.setColor(255, 255, 255, 0);             // all white
    rgbw.show();
    delay(500);
    rgbw.setColor(0); rgbw.show();               // all off
}
```

# Notes

- `Yoke_rgbw` uses the ESP-IDF v5.x new RMT driver (`driver/rmt_tx.h`), compatible with Arduino ESP32 3.x
- No third-party libraries required (replaces Adafruit_NeoPixel dependency)
- Timing: 20MHz RMT resolution (50ns/tick), tuned for SK6812 RGBW spec
- Data order: GRBW, MSB first — matches SK6812 RGBW NeoPixels
- LED 0 is closest to the MCU; data propagates from LED0 → LED1 → ...
