# Ryzobee Arduino ESP32

[中文版本](./README_CN.md)

This repository provides Arduino support library for Ryzobee series development boards, making the development process faster and more efficient.

## Quick Start

Supported boards are pre-configured, just include the header file to get started:

```cpp
#include "Ryzobee.h"

// Create board instance
Ryzobee board(RYZOBEE_ROOTMAKER);

void setup(void)
{
    Serial.begin(115200);
    
    // Initialize board (LCD, Touch, LED, Accelerometer)
    board.rootmaker.begin(true, true, true, true);
}

void loop(void)
{
    // Your code here
}
```

## Supported Boards

| Name | Model | LCD Interface | Touch Chip | Other Peripherals |
|:---:|:---:|:---:|:---:|:---:|
| RYZOBEE_ROOTMAKER | RootMaker | SPI / ST7789 | CST816T | LIS2DWTR / WS2812B / TP4054 |

## Arduino IDE Configuration

When selecting the board in Arduino IDE, please configure with the following parameters:

| Name | Model | Flash Mode | Flash Size | PSRAM |
|:---:|:---:|:---:|:---:|:---:|
| RYZOBEE_ROOTMAKER | RootMaker | QIO 80MHz | 16MB | QSPI PSRAM |

## Dependencies

Please ensure the following libraries are installed before use:

| Name | Min Version | Description |
|:---:|:---:|:---|
| [arduino-esp32](https://github.com/espressif/arduino-esp32.git) | v3.0.7 | Arduino core for ESP32 series SoC |
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | v1.12.3 | Arduino library for controlling single-wire LED pixels and strips |
| [LIS2DW12](https://github.com/stm32duino/LIS2DW12.git) | v2.1.1 | Arduino library for LIS2DW12 3D accelerometer |

## License

This project is licensed under the MIT License.
