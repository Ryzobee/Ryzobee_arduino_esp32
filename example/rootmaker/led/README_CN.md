# LED 灯效例程

[English](./README.md)

本例程演示如何使用 Rootmaker 板载 RGB 灯带输出循环彩虹灯效，适合用于验证 LED 外设是否工作正常，并了解库中 LED 控制接口的基本用法。

# 功能说明

- 初始化 Rootmaker 板载 LED 模块
- 循环输出彩虹渐变灯效
- 演示像素颜色设置与刷新流程

# 依赖库

本例程基于本库提供的 LED 接口实现，无需额外安装第三方 Arduino 依赖库。

# 运行说明

- 编译并烧录 example/rootmaker/led/led.ino
- 上电后，板载 RGB LED 会持续播放彩虹循环灯效

# 说明

- 本例程仅启用 LED 外设，未启用屏幕、传感器、按键和电池模块
- 使用 `board.maker.begin(true, false, false, false)` 选择需要启用的板载外设；屏幕通过独立的 Yoke 外设对象初始化。
- 若灯效未正常显示，请优先检查开发板型号选择、供电状态以及依赖库是否正确安装
