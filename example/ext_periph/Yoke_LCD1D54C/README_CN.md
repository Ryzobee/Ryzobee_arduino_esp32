# Yoke LCD1D54C 屏幕与触摸示例

使用 240×240 ST7789 屏幕和 CST816T 触摸。

## 功能说明

- 初始化屏幕显示模块和触控功能
- 在屏幕上绘制可点击的红色方块
- 点击方块后随机刷新方块位置
- 点击其他区域时，在触摸点附近绘制绿色像素标记

## 使用说明

安装 Yoke 和 LovyanGFX 后，编译并烧录 `Yoke_LCD1D54C.ino`。
示例通过 `Lcd1d54cPins` 指定实际 GPIO，调用 `lcd.begin(pins)`；初始化成功返回 `YOKE_PORT_ERROR_NONE`。
LCD1D54C 是可拆卸外设，由示例独立创建和初始化，不依赖 Ryzobee 开发板类。
默认使用 `Wire`，驱动初始化触摸 I2C 总线。下表为 RootMaker 接线演示，其他 ESP32 开发板请修改 `setup()` 中的 GPIO。所有引脚均须有效且不重复；除触摸 INT 外，其余引脚须支持输出。

| 信号 | F_BUS 引脚 | RootMaker GPIO |
| --- | --- | --- |
| LCD SCK | 11 | 12 |
| LCD RST | 13 | 10 |
| LCD SDA/MOSI | 14 | 9 |
| LCD BL | 16 | 7 |
| LCD CS | 18 | 5 |
| LCD RS/DC | 20 | 3 |
| TP RST | 21 | 2 |
| TP INT | 22 | 1 |
| TP SDA | 23 | 41 |
| TP SCL | 24 | 40 |

`lcd` 继承 LovyanGFX 绘图接口，支持 `setBrightness()`、`fillRect()`、`drawString()` 和 `getTouch(&x, &y)`；`lcd.end()` 关闭背光并释放 SPI 总线，保留共享 I2C 总线。
GPIO 初始化不参与 Ryzobee 端口占用管理，由应用协调引脚。共享已初始化的 I2C 总线时，可在构造函数中传入 `TwoWire*` 和互斥锁地址，并保证它们比 lcd 对象存活更久。

驱动仅通过 `lcd.begin(pins)` 接收实际 GPIO 配置，不提供 F_BUS 端口初始化或固定针脚映射。表中的 F_BUS 针脚仅用于说明 RootMaker 的物理接线。
