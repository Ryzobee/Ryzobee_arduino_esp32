# KEYW 按键模块例程

[English](./README.md)

本例程演示如何在 Ryzobee 开发板上使用 KEYW 扩展模块（按键 + LED）。本例程使用 Yoke 库的 `Yoke_keyw` 类管理 RS 端口、按键消抖和 LED 输出。

SDA → 按键输入（低电平有效，内部上拉），SCL → LED 输出（支持 PWM，`analogWrite`）。

# 功能说明

- 初始化 RS 端口：`keyw.begin(board.maker.useport("rs_portN"))`
- 轮询按键 + 30ms 软件消抖，每次循环调用 `keyw.update()`
- 每次按下切换 LED 亮灭
- 可通过 `LED_CONTROL` 宏切换数字开关 / PWM 亮度模式

# 硬件连接

| 扩展口 | KEYW 模块 |
|--------|-----------|
| SDA    | 按键输入  |
| SCL    | LED 输出  |

Rootmaker 端口 GPIO 映射：

| 端口   | SDA (按键) | SCL (LED) |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

| 名称 | 说明 |
|------|------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | 开发板支持（Rootmaker） |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | KEYW 外设驱动 |

需要安装 Yoke 库；无需额外第三方驱动库。

# 快速开始

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

# 说明

- 按键低电平有效，已启用内部上拉，无需外接上拉电阻
- PWM 模式（`LED_CONTROL=1`）使用 `analogWrite`，需确保 SCL 引脚支持 PWM
- `wasPressed()` / `wasReleased()` 读取并清除对应事件，`isPressed()` 查询消抖后的状态
- `setLED(bool)` 控制数字开关，`setLEDBrightness(0~255)` 设置 PWM 亮度
- `end()` 关闭 LED 并释放引脚和端口；端口数据须在此之前保持有效
- 请频繁调用 `update()`；两次轮询之间完成的短按可能无法捕获，未消费的同类事件会合并
