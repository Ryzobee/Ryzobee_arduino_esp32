# Yoke LCD1D54C Display and Touch Example

Uses a 240×240 ST7789 display and CST816T touch controller.

## Features

- Initializes the display module and touch input
- Draws a clickable red square on the screen
- Moves the square to a random position when tapped
- Draws green pixel markers near the touch point when other areas are tapped

## Usage

Install Yoke and LovyanGFX, then build and upload `Yoke_LCD1D54C.ino`.
The example specifies actual GPIOs with `Lcd1d54cPins` and calls `lcd.begin(pins)`, which returns `YOKE_PORT_ERROR_NONE` on successful initialization.
LCD1D54C is a detachable peripheral created and initialized separately by the example, without a Ryzobee board class.
By default, the driver uses `Wire` and initializes the touch I2C bus. The table illustrates RootMaker wiring; change the GPIOs in `setup()` for other ESP32 boards. All pins must be valid and distinct; every pin except touch INT must support output.

| Signal | F_BUS Pin | RootMaker GPIO |
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

`lcd` inherits the LovyanGFX graphics interface and supports `setBrightness()`, `fillRect()`, `drawString()` and `getTouch(&x, &y)`; `lcd.end()` turns off the backlight and releases the SPI bus, keeping the shared I2C bus active.
GPIO initialization does not reserve a Ryzobee port; the application coordinates pins. To share an initialized I2C bus, pass a `TwoWire*` and a mutex handle address to the constructor, and keep both resources alive longer than lcd.

The driver accepts actual GPIO configuration through `lcd.begin(pins)` only, without F_BUS port initialization or fixed connector mappings. F_BUS pins in the table describe the physical RootMaker wiring only.
