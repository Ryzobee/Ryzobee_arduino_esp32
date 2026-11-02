# MOTO 电机驱动例程

[English](./README.md)

本例程演示如何使用 **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** 库的 `Yoke_moto` 类在 Ryzobee 开发板上驱动直流电机。模块通过 MCPWM 控制 RS 端口两个引脚（IN1/IN2）上的 H 桥电机驱动芯片。

# 功能说明

- 初始化：`moto.begin(board.maker.useport("rs_portN"))`
- 正转 / 反转 / 停止：`setmoto(Yoke_moto::DIR_FORWARD, speed)`（speed 范围 0–100）
- 速度映射可调：`setSpeedMap(min, max)`
- `groundUnusedPorts()` 防止未用端口浮空导致 H 桥误触发

# 硬件连接

| 扩展口 | 电机驱动 |
|--------|---------|
| SDA    | IN1     |
| SCL    | IN2     |

Rootmaker 端口 GPIO 映射：

| 端口   | SDA (IN1) | SCL (IN2) |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

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

# 多端口使用

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

# 说明

- 速度范围 0–100（默认映射为 0–255 PWM 占空比）
- 未配置的 RS 端口引脚处于浮空态，建议调用 `groundUnusedPorts()` 主动拉低
- 多个 `Yoke_moto` 实例使用独立的 MCPWM 定时器，互不冲突
