# KEYW Module Example

[中文](./README_CN.md)

This example demonstrates how to use the KEYW expansion module (key + LED) on a Ryzobee board. It uses the Yoke library's `Yoke_keyw` class for RS port management, key debounce, and LED output.

SDA → key input (active LOW, internal pull-up), SCL → LED output (PWM-capable via `analogWrite`).

# Features

- Initialize an RS port: `keyw.begin(board.maker.useport("rs_portN"))`
- Polled input with 30 ms software debounce; call `keyw.update()` each loop
- Toggle LED on each key press
- Switchable digital on/off or PWM brightness mode (`LED_CONTROL` define)

# Hardware

| Expansion Port | KEYW Module |
|----------------|-------------|
| SDA            | Key input   |
| SCL            | LED output  |

Rootmaker GPIO mapping:

| Port  | SDA (Key) | SCL (LED) |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

| Library | Description |
|---------|-------------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | Board support (Rootmaker) |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | KEYW peripheral driver |

Install the Yoke library; no additional third-party driver library is required.

# Quick Start

```cpp
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
```

# Notes

- Key is active LOW with internal pull-up — no external pull-up needed
- PWM mode (`LED_CONTROL=1`) uses `analogWrite`; ensure the SCL pin supports PWM
- `wasPressed()` / `wasReleased()` consume the corresponding event; `isPressed()` reads the debounced state
- `setLED(bool)` controls digital output; `setLEDBrightness(0~255)` controls PWM brightness
- `end()` turns off the LED and releases the pins and port; keep the port data valid until then
- Call `update()` frequently; presses completed between polls may be missed, and unread events of the same kind coalesce
