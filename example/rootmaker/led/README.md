# LED Effect Example

[中文](./README_CN.md)

This example demonstrates how to drive the onboard RGB LED strip on the Rootmaker board and render a continuous rainbow animation. It is useful for verifying that the LED hardware is working correctly and for learning the basic LED control APIs provided by the library.

# Features

- Initializes the onboard LED module
- Plays a looping rainbow animation
- Demonstrates pixel color updates and strip refresh flow

# Dependencies

This example uses the LED APIs provided by this library and does not require any additional third-party Arduino libraries.

# Usage

- Build and upload example/rootmaker/led/led.ino
- After boot, the onboard RGB LED will continuously display a rainbow cycle effect

# Notes

- This example only enables the LED peripheral; the display, sensor, button, and battery modules are disabled
- Use `board.maker.begin(true, false, false, false)` to select onboard peripherals; initialize the display through a separate Yoke peripheral object.
- If the LED effect does not appear as expected, first verify the selected board type, power status, and dependency installation

