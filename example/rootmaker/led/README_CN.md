# LED 灯效例程

[English](./README.md)

本例程演示如何使用 Rootmaker 板载 RGB 灯带输出循环彩虹灯效，适合用于验证 LED 外设是否工作正常，并了解库中 LED 控制接口的基本用法。

# 功能说明

- 初始化 Rootmaker 板载 LED 模块
- 循环输出彩虹渐变灯效
- 演示像素颜色设置与刷新流程

# 依赖库

本例程依赖以下 Arduino 库，请先在 Arduino IDE 中安装：

| 名称 | 版本 | 说明 |
|---|---|---|
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | >= v1.15.3 | 用于控制单总线 RGB LED 像素和灯带 |

# 运行说明

- 下载并安装所需依赖库
- 编译并烧录 examples/led/led.ino
- 上电后，板载 RGB LED 会持续播放彩虹循环灯效

# 说明

- 本例程仅启用 LED 外设，未启用屏幕、传感器、按键和电池模块
- 若灯效未正常显示，请优先检查开发板型号选择、供电状态以及依赖库是否正确安装
