# LED Effect Example

[中文](./README_CN.md)

This example demonstrates how to drive the onboard RGB LED strip on the Rootmaker board and render a continuous rainbow animation. It is useful for verifying that the LED hardware is working correctly and for learning the basic LED control APIs provided by the library.

# Features

- Initializes the onboard LED module
- Plays a looping rainbow animation
- Demonstrates pixel color updates and strip refresh flow

# Dependencies

Install the following Arduino library in the Arduino IDE before building this example:

| Name | Version | Description |
|---|---|---|
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | >= v1.15.3 | Arduino library for controlling single-wire RGB LEDs and LED strips |

# Usage

- Install the required dependency
- Build and upload examples/led/led.ino
- After boot, the onboard RGB LED will continuously display a rainbow cycle effect

# Notes

- This example only enables the LED peripheral; the display, sensor, button, and battery modules are disabled
- If the LED effect does not appear as expected, first verify the selected board type, power status, and dependency installation

