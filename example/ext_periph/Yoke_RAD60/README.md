# RAD60 mmWave Radar Example

[中文](./README_CN.md)

This example demonstrates the AT6010 (RAD60) mmWave radar sensor on a Ryzobee board using the **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** library's `Yoke_rad60` class. Communication is via UART at 115200 baud on an RS expansion port (SDA=TX, SCL=RX).

# Features

- Initialize radar on any RS port: `radar.begin(board.maker.useport("rs_portN"))`
- UART specified via constructor: `Yoke_rad60 radar(UART_NUM_1, 115200)`
- Wake ULP radar: `radar.enterCommunicationMode(true)`
- Apply default detection config (motion + breath): `radar.applyDefaultConfig()`
- Single-target output (the first target in each report): `radar.update()`
- Distance (cm), angle (degrees), velocity (cm/s), and ID for one target
- Vacancy / communication health monitoring: `isDetected()`, `isConnected()`

# Hardware

| RS Port | RAD60 Module |
|---------|-------------|
| SDA     | TX          |
| SCL     | RX          |
| VCC     | 3.3V        |
| GND     | GND         |

Rootmaker GPIO mapping:

| Port  | SDA (TX)  | SCL (RX)  |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

| Library | Description |
|---------|-------------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | Board support (Rootmaker) |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | Peripheral drivers (UART-based, no third-party libs) |

# Quick Start

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_rad60/yoke_rad60.hpp>

Ryzobee board;
Yoke_rad60 radar(UART_NUM_1, 115200);

void setup() {
    board.maker.begin();
    radar.begin(board.maker.useport("rs_port4"));
    delay(2000);                                 // startup delay
    radar.enterCommunicationMode(true);
    radar.applyDefaultConfig();
}

void loop() {
    radar.update();
    if (radar.isDetected()) {
        Serial.printf("Targets: %d, dist: %d cm\n",
                      radar.getTargetCount(),
                      radar.getTargetDistance());
    }
    delay(50);
}
```

# Notes

- Each RAD60 requires its own UART (UART_NUM_1 or UART_NUM_2)
- Must wait ~2s after power-on before sending commands
- Communication baud rate is fixed at 115200
- Protocol: 0x58 send / 0x59 response / 0x5A auto-report frames

The driver still receives `TYPE=0x0A` multi-target reports and parses 8-byte target records, caching only the first target. `getTargetCount()` returns 0 or 1. This does not change the module algorithm or guarantee the same target ID across frames. Getters accept an omitted index, such as `getTargetDistance()`; nonzero indices return 0. Incomplete reports are ignored; empty reports or timeouts clear the count.
