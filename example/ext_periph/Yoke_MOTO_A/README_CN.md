# MOTO-A 电机驱动例程 (F-BUS)

[English](./README.md)

本例程演示如何使用 **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** 库的 `Yoke_moto_a` 类驱动直流电机。与 RS 端口的 `Yoke_moto` 不同，该驱动使用 **F-BUS** 端口，固定引脚为 F_BUS17（GPIO6, IN1/A）和 F_BUS19（GPIO4, IN2/B）。使用 MCPWM TIMER_1，可与 RS 端口的 `Yoke_moto`（TIMER_0）同时工作。

# 功能说明

- 初始化：`moto.begin(board.maker.useport("f_bus"))` — 无需指定端口号
- 正转 / 反转 / 停止：`setmoto(Yoke_moto_a::DIR_FORWARD, speed)`（speed 范围 0–100）
- 独立 MCPWM 定时器（TIMER_1），与 RS 端口 MOTO（TIMER_0）共存
- 速度映射可调：`setSpeedMap(min, max)`

# 硬件连接

| F-BUS 引脚 | GPIO  | 电机驱动 |
|-----------|-------|---------|
| F_BUS17   | GPIO6 | IN1 / A |
| F_BUS19   | GPIO4 | IN2 / B |

> **注意：** Rootmaker 的 F-BUS 端口数据向量需包含全部 16 个引脚（F_BUS9 至 F_BUS24）。若 `Yoke_moto_a::begin()` 失败，请检查 `rootmaker.hpp` 中的端口定义。

# 依赖库

| 名称 | 说明 |
|------|------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | 开发板支持（Rootmaker） |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | 外设驱动 |

# 快速开始

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

# RS + F-BUS 双电机同时运行

RS 端口 `Yoke_moto`（TIMER_0）和 F-BUS `Yoke_moto_a`（TIMER_1）可独立同时工作：

```cpp
Yoke_moto  motorRS;   // RS 端口, TIMER_0
Yoke_moto_a motorFB;  // F-BUS,   TIMER_1

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

# 说明

- F-BUS 电机固定使用 F_BUS17/F_BUS19，无需选择端口号
- 速度范围 0–100，默认映射 0–100 PWM 占空比（1:1）
- MCPWM Timer 1 专用于 F-BUS；Timer 0 用于 RS 端口 MOTO
