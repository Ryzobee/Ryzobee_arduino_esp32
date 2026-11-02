# MOTO Module Example

[中文](./README_CN.md)

This example demonstrates how to drive a DC motor using the MOTO expansion module on a Ryzobee board. It uses the **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** library's `Yoke_moto` class, which controls an H-bridge motor driver via MCPWM on two RS port pins (IN1/IN2).

# Features

- Initialize via `moto.begin(board.maker.useport("rs_portN"))`
- Forward / reverse / stop: `setmoto(Yoke_moto::DIR_FORWARD, speed)` (speed 0–100)
- Speed ramping and adjustable PWM mapping via `setSpeedMap(min, max)`
- `groundUnusedPorts()` helper prevents floating H-bridge inputs on unused ports

# Hardware

| Expansion Port | Motor Driver |
|----------------|-------------|
| SDA            | IN1         |
| SCL            | IN2         |

Rootmaker GPIO mapping:

| Port  | SDA (IN1) | SCL (IN2) |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

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
#include <yoke_moto/yoke_moto.hpp>

Ryzobee board;
Yoke_moto moto;

void setup() {
    board.maker.begin();
    moto.begin(board.maker.useport("rs_port4"));
}

void loop() {
    moto.setmoto(Yoke_moto::DIR_FORWARD, 100);  delay(2000);
    moto.setmoto(Yoke_moto::DIR_FORWARD, 0);    delay(1000);
}
```

# Multi-Port Usage

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_moto/yoke_moto.hpp>

Ryzobee board;
Yoke_moto motorA, motorB;

void setup() {
    board.maker.begin();
    motorA.begin(board.maker.useport("rs_port2"));
    motorB.begin(board.maker.useport("rs_port3"));
}

void loop() {
    motorA.setmoto(Yoke_moto::DIR_FORWARD, 80);
    motorB.setmoto(Yoke_moto::DIR_REVERSE, 60);
    delay(1000);
}
```

# Notes

- Speed range is 0–100 (maps to 0–255 PWM duty by default)
- Unconfigured RS port pins float; use `groundUnusedPorts()` to pull them LOW
- Multiple `Yoke_moto` instances use independent MCPWM timers
