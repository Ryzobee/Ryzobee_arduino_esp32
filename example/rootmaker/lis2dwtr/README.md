# LIS2DW12 Accelerometer Example

[中文](./README_CN.md)

This example demonstrates how to initialize the onboard LIS2DW12 three-axis accelerometer on the Rootmaker board and continuously print X, Y, and Z acceleration data to the serial monitor. It is useful for verifying sensor communication and learning the basic sensor-reading APIs exposed by the library.

# Features

- Initializes the LIS2DW12 sensor module
- Periodically reads three-axis acceleration data
- Prints raw X, Y, and Z measurements to the serial monitor

# Dependencies

Install the following Arduino library in the Arduino IDE before building this example:

| Name | Version | Description |
|---|---|---|
| [LIS2DW12](https://github.com/stm32duino/LIS2DW12.git) | >= v2.1.1 | Arduino library for the LIS2DW12 three-axis accelerometer |

# Usage

- Install the required dependency
- Build and upload example/rootmaker/lis2dwtr/lis2dwtr.ino
- Open the serial monitor at 115200 baud
- After boot, observe the continuous X, Y, and Z acceleration output

# Notes

- This example only enables the sensor module; the display, LED, button, and battery modules are disabled
- Use `board.maker.begin(false, true, false, false)` to select onboard peripherals; initialize the display through a separate Yoke peripheral object.
- If no data appears or the readings look invalid, first verify sensor initialization, library version compatibility, and board connection status

