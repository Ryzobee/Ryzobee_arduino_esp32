# RootMaker 开发板

[English](README.md)

本目录提供 RootMaker 开发板的板载外设和扩展端口支持。`rootmaker.hpp` 定义开发板接口，`rootmaker_pin.h` 定义 GPIO 映射，`peripherals` 存放板载外设驱动。

## 板载外设

| 对象 | 功能 |
| --- | --- |
| `board.maker.led` | 板载 RGB LED |
| `board.maker.gsensor` | LIS2DWTR 加速度传感器 |
| `board.maker.btns` | 板载按键 |
| `board.maker.ec` | 电池检测 |

定义 `BOARD_RYZOBEE_ROOTMAKER` 后包含 `Ryzobee.h`，创建 `Ryzobee board`，通过 `board.maker` 使用开发板。

`board.maker.begin()` 初始化全部板载外设。也可使用 `board.maker.begin(LEDEnable, SensorEnable, ButtonEnable, BatteryEnable)` 分别选择启用的功能；四个参数均为布尔值。全部传入 `false` 时仍会初始化开发板的共享 I2C 总线。

板载外设示例见 [example/rootmaker](../../../example/rootmaker)。

## 可接入的 Yoke 外设

LCD1D54C 屏幕、MOTO 电机模块、RGBW 灯光模块等均为可拆卸外设，由配合库 Yoke 提供驱动。它们由应用独立创建，通过 `外设.begin(board.maker.useport("端口名"))` 或屏幕的 `lcd.begin(pins)` 初始化，不作为 `board.maker` 的成员，也不随开发板自动初始化。

| 外设 | Yoke 类 | 接口 | 示例 |
| --- | --- | --- | --- |
| LCD1D54C 屏幕及触摸 | `Yoke_lcd1d54c` | GPIO (`Lcd1d54cPins`) | [触摸交互](../../../example/ext_periph/Yoke_LCD1D54C)、[GIF 播放](../../../example/ext_periph/Yoke_LCD1D54C_GIF) |
| MOTO 电机模块 | `Yoke_moto` | `rs_port1`～`rs_port4` | [Yoke_MOTO](../../../example/ext_periph/Yoke_MOTO) |
| MOTO-A 电机模块 | `Yoke_moto_a` | `f_bus` | [Yoke_MOTO_A](../../../example/ext_periph/Yoke_MOTO_A) |
| RGBW 灯光模块 | `Yoke_rgbw` | `rs_port1`～`rs_port4` | [Yoke_RGBW](../../../example/ext_periph/Yoke_RGBW) |
| KEYW 按键模块 | `Yoke_keyw` | RS 端口 | [Yoke_KEYW](../../../example/ext_periph/Yoke_KEYW) |
| RAD60 雷达模块 | `Yoke_rad60` | `rs_port1`～`rs_port4` | [Yoke_RAD60](../../../example/ext_periph/Yoke_RAD60) |
| TOUCH4 四通道触摸 | `Yoke_touch4` | RS 端口（UART） | [Yoke_TOUCH4](../../../example/ext_periph/Yoke_TOUCH4) |

先安装 Yoke 库，并在应用中包含对应的外设头文件。屏幕还需要 LovyanGFX，GIF 播放需要 AnimatedGIF；具体接线和配置见对应示例。

## 扩展端口

| 端口名 | 引脚 |
| --- | --- |
| `rs_port1` | GPIO15、GPIO16 |
| `rs_port2` | GPIO19、GPIO20 |
| `rs_port3` | GPIO13、GPIO14 |
| `rs_port4` | GPIO17、GPIO18 |
| `s_bus` | S_BUS1～S_BUS14 |
| `f_bus` | F_BUS9～F_BUS24 |

完整引脚映射见 [rootmaker_pin.h](rootmaker_pin.h)。RS 端口的通信方式由外设决定，不能仅根据端口信号名称判断为 I2C。

S_BUS、F_BUS 与部分 RS 端口及板载功能共用 GPIO，使用时需核对引脚占用。当前端口接口不支持按引脚拆分 F_BUS。屏幕仅接收 GPIO 配置，不会占用或检查 `f_bus` 端口；与 MOTO-A 等外设组合时，由应用核对实际引脚和硬件连接。

屏幕触摸与板载传感器共用 I2C。屏幕示例将 `&board.maker.getWire()` 和 `board.maker.getI2CMutex()` 传入驱动，以复用总线和互斥锁。应先初始化开发板，再初始化屏幕，并使开发板对象的生命周期长于屏幕对象。

其他 ESP32 开发板也可直接提供 `Lcd1d54cPins` GPIO 配置，通过 `lcd.begin(pins)` 使用屏幕，无需遵循 RootMaker 的 F_BUS 接线。参见 [自定义 GPIO 示例](../../../example/ext_periph/Yoke_LCD1D54C/README_CN.md)。
