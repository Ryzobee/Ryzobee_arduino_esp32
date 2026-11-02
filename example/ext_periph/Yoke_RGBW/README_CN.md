# RGBW LED 例程

[English](./README.md)

本例程演示如何使用 **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** 库的 `Yoke_rgbw` 类控制 SK6812 RGBW NeoPixel 灯珠。基于 ESP32 RMT 外设（新版驱动，ESP-IDF v5.x），通过 RS 端口的 SDA 引脚驱动多颗灯珠。

# 功能说明

- 运行时设置灯珠数量：`rgbw.setNumLeds(N)`
- 逐像素设色：`rgbw.setPixelColor(索引, R, G, B, W)` 或 32 位合成色 `setPixelColor(索引, 0xWWRRGGBB)`
- 全灯同色：`rgbw.setColor(r, g, b, w)`（向后兼容旧 API）
- 显式发送：`rgbw.show()` — 调色后调用一次，把所有灯珠数据发出
- 追逐效果、亮度渐变、混合色

# 硬件连接

| 扩展口 | NeoPixel 灯带 |
|--------|--------------|
| SDA    | 数据线 (DIN) |
| SCL    | 未使用       |

Rootmaker 端口 GPIO 映射：

| 端口   | SDA (DIN) |
|--------|-----------|
| PORT1  | GPIO15    |
| PORT2  | GPIO19    |
| PORT3  | GPIO13    |
| PORT4  | GPIO17    |

# 依赖库

| 名称 | 说明 |
|------|------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | 开发板支持（Rootmaker） |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | 外设驱动（基于 RMT，无需 Adafruit_NeoPixel） |

# 快速开始

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
    rgbw.setNumLeds(3);                          // 3 颗灯珠
}

void loop() {
    rgbw.setPixelColor(0, 255, 0, 0, 0);        // LED0: 红色
    rgbw.setPixelColor(1, 0, 255, 0, 0);        // LED1: 绿色
    rgbw.setPixelColor(2, 0, 0, 255, 0);        // LED2: 蓝色
    rgbw.show();
    delay(1000);

    rgbw.setColor(255, 255, 255, 0);             // 全白
    rgbw.show();
    delay(500);
    rgbw.setColor(0); rgbw.show();               // 全灭
}
```

# 说明

- 使用 ESP-IDF v5.x 新版 RMT 驱动（`driver/rmt_tx.h`），兼容 Arduino ESP32 3.x
- 无需第三方 LED 库（替代 Adafruit_NeoPixel 依赖）
- 时序：20MHz RMT 分辨率（50ns/tick），已针对 SK6812 RGBW 规范调优
- 数据顺序：GRBW，高位先发 — 匹配 SK6812 RGBW 灯珠
- LED0 离 MCU 最近，数据依次向后传递
