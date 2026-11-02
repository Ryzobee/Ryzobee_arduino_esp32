# MOTO-A Motor Example (F-BUS)

[中文](./README_CN.md)

This example demonstrates how to drive a DC motor using the **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** library's `Yoke_moto_a` class. Unlike RS-port `Yoke_moto`, this driver uses the **F-BUS** port with dedicated pins F_BUS17 (GPIO6, IN1/A) and F_BUS19 (GPIO4, IN2/B). It runs on MCPWM TIMER_1, so it can operate simultaneously with a RS-port `Yoke_moto` on TIMER_0.

# Features

- Initialize via `moto.begin(board.maker.useport("f_bus"))` — no port number needed
- Forward / reverse / stop: `setmoto(Yoke_moto_a::DIR_FORWARD, speed)` (speed 0–100)
- Independent MCPWM timer (TIMER_1) — coexists with RS-port Yoke_moto (TIMER_0)
- Adjustable speed mapping: `setSpeedMap(min, max)`

# Hardware

| F-BUS Pin | GPIO  | Motor Driver |
|-----------|-------|-------------|
| F_BUS17   | GPIO6 | IN1 / A     |
| F_BUS19   | GPIO4 | IN2 / B     |

> **Note:** The Rootmaker F-BUS port data vector must include all 16 F-BUS pins (F_BUS9 through F_BUS24). If `Yoke_moto_a::begin()` fails, verify the port definition in `rootmaker.hpp`.

# Dependencies

| Library | Description |
|---------|-------------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | Board support (Rootmaker) |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | Peripheral drivers |

# Quick Start

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_moto_a/yoke_moto_a.hpp>

Ryzobee board;
Yoke_moto_a moto;

void setup() {
    board.maker.begin();
    moto.begin(board.maker.useport("f_bus"));
}

void loop() {
    moto.setmoto(Yoke_moto_a::DIR_FORWARD, 100);  delay(2000);
    moto.setmoto(Yoke_moto_a::DIR_FORWARD, 0);    delay(1000);
}
```

# Simultaneous RS + F-BUS Motors

RS-port `Yoke_moto` (TIMER_0) and F-BUS `Yoke_moto_a` (TIMER_1) can run independently at the same time:

```cpp
Yoke_moto  motorRS;   // RS port, TIMER_0
Yoke_moto_a motorFB;  // F-BUS,  TIMER_1

void setup() {
    board.maker.begin();
    motorRS.begin(board.maker.useport("rs_port4"));
    motorFB.begin(board.maker.useport("f_bus"));
}

void loop() {
    motorRS.setmoto(Yoke_moto::DIR_FORWARD, 80);
    motorFB.setmoto(Yoke_moto_a::DIR_REVERSE, 60);
    delay(1000);
}
```

# Notes

- No port selection parameter — F-BUS motor always uses F_BUS17/F_BUS19
- Speed range 0–100, default mapping 0–100 PWM duty (1:1)
- MCPWM Timer 1 is dedicated to F-BUS; Timer 0 is used by RS-port MOTO
