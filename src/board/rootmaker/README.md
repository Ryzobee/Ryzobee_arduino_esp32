# RootMaker Board

[中文](README_CN.md)

This directory provides onboard peripheral and expansion port support for RootMaker. `rootmaker.hpp` defines the board interface, `rootmaker_pin.h` defines GPIO mappings, and `peripherals` contains onboard peripheral drivers.

## Onboard peripherals

| Object | Function |
| --- | --- |
| `board.maker.led` | Onboard RGB LED |
| `board.maker.gsensor` | LIS2DWTR accelerometer |
| `board.maker.btns` | Onboard button |
| `board.maker.ec` | Battery monitoring |

Define `BOARD_RYZOBEE_ROOTMAKER` before including `Ryzobee.h`, create `Ryzobee board`, and access the board through `board.maker`.

`board.maker.begin()` initializes all onboard peripherals. Use `board.maker.begin(LEDEnable, SensorEnable, ButtonEnable, BatteryEnable)` to select individual features with four Boolean switches. Passing `false` for all four still initializes the shared board I2C bus.

See [example/rootmaker](../../../example/rootmaker) for onboard peripheral examples.

## Supported Yoke peripherals

The LCD1D54C display, MOTO motor module, RGBW lighting module, and other expansion modules are detachable peripherals with drivers provided by the companion Yoke library. Applications create these objects separately and initialize them with `peripheral.begin(board.maker.useport("port_name"))` or, for displays, `lcd.begin(pins)`. They are not members of `board.maker` and are not initialized automatically with the board.

| Peripheral | Yoke class | Port | Examples |
| --- | --- | --- | --- |
| LCD1D54C display and touch | `Yoke_lcd1d54c` | GPIO (`Lcd1d54cPins`) | [Touch interaction](../../../example/ext_periph/Yoke_LCD1D54C), [GIF playback](../../../example/ext_periph/Yoke_LCD1D54C_GIF) |
| MOTO motor module | `Yoke_moto` | `rs_port1` through `rs_port4` | [Yoke_MOTO](../../../example/ext_periph/Yoke_MOTO) |
| MOTO-A motor module | `Yoke_moto_a` | `f_bus` | [Yoke_MOTO_A](../../../example/ext_periph/Yoke_MOTO_A) |
| RGBW lighting module | `Yoke_rgbw` | `rs_port1` through `rs_port4` | [Yoke_RGBW](../../../example/ext_periph/Yoke_RGBW) |
| KEYW button module | `Yoke_keyw` | RS port | [Yoke_KEYW](../../../example/ext_periph/Yoke_KEYW) |
| RAD60 radar module | `Yoke_rad60` | `rs_port1` through `rs_port4` | [Yoke_RAD60](../../../example/ext_periph/Yoke_RAD60) |
| TOUCH4 four-channel touch | `Yoke_touch4` | RS port (UART) | [Yoke_TOUCH4](../../../example/ext_periph/Yoke_TOUCH4) |

Install Yoke and include the appropriate peripheral header in your application. Displays also require LovyanGFX; GIF playback requires AnimatedGIF. Refer to each example for wiring and configuration.

## Expansion ports

| Port name | Pins |
| --- | --- |
| `rs_port1` | GPIO15, GPIO16 |
| `rs_port2` | GPIO19, GPIO20 |
| `rs_port3` | GPIO13, GPIO14 |
| `rs_port4` | GPIO17, GPIO18 |
| `s_bus` | S_BUS1 through S_BUS14 |
| `f_bus` | F_BUS9 through F_BUS24 |

See [rootmaker_pin.h](rootmaker_pin.h) for complete GPIO mappings. The peripheral determines the communication protocol on an RS port; signal names alone do not imply I2C.

S_BUS and F_BUS share some GPIOs with RS ports and onboard functions. Check pin assignments before combining modules. The current port interface does not split F_BUS into individual pin allocations. The display accepts GPIO configuration only and does not reserve or check the `f_bus` port; applications must check actual pin assignments and hardware connections when combining it with peripherals such as MOTO-A.

Display touch shares I2C with onboard sensors. The display examples pass `&board.maker.getWire()` and `board.maker.getI2CMutex()` to the driver to reuse the bus and mutex. Initialize the board before the display and keep the board object alive longer than the display object.

Other ESP32 boards can provide a `Lcd1d54cPins` GPIO configuration and call `lcd.begin(pins)` without following the RootMaker F_BUS layout. See the [custom GPIO example](../../../example/ext_periph/Yoke_LCD1D54C/README.md).
