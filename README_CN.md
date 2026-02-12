# Ryzobee Arduino ESP32

[English](./readme.md)

本仓库提供对 Ryzobee 系列开发板的 Arduino 支持库, 使开发过程更加迅速和高效.

## 快速开始

支持的开发板已预配置完成, 只需包含头文件即可开始使用:

```cpp
#include "Ryzobee.h"

// 创建开发板实例
Ryzobee board(RYZOBEE_ROOTMAKER);

void setup(void)
{
    Serial.begin(115200);
    
    // 初始化开发板 (LCD, 触摸, LED, 加速度计)
    board.rootmaker.begin(true, true, true, true);
}

void loop(void)
{
    // 你的代码
}
```

## 支持的开发板

| 名称 | 型号 | LCD接口 | 触摸芯片 | 其它外设 |
|:---:|:---:|:---:|:---:|:---:|
| RYZOBEE_ROOTMAKER | RootMaker | SPI / ST7789 | CST816T | LIS2DWTR / WS2812B / TP4054 |

## Arduino IDE 配置

在 Arduino IDE 中选择开发板时, 请按以下参数配置:

| 名称 | 型号 | Flash Mode | Flash Size | PSRAM |
|:---:|:---:|:---:|:---:|:---:|
| RYZOBEE_ROOTMAKER | RootMaker | QIO 80MHz | 16MB | QSPI PSRAM |

## 依赖库

使用前请确保已安装以下依赖库:

| 名称 | 最低版本 | 说明 |
|:---:|:---:|:---|
| [arduino-esp32](https://github.com/espressif/arduino-esp32.git) | v3.0.7 | ESP32 系列 SoC 的 Arduino 核心库 |
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | v1.12.3 | 用于控制基于单线协议的 LED 像素和灯带 |
| [LIS2DW12](https://github.com/stm32duino/LIS2DW12.git) | v2.1.1 | 支持 LIS2DW12 3D 加速度计的驱动库 |

## 许可证

本项目采用 MIT 许可证.
