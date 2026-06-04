# Display Touch Example

[中文](./README_CN.md)

This example demonstrates how to initialize the onboard display and touch input on the Rootmaker board and build a simple touch interaction around a movable square. It is useful for verifying display and touch functionality and for learning the basic drawing and touch-reading APIs provided by the library.

# Features

- Initializes the display and touch interface
- Draws a clickable red square on the screen
- Randomly moves the square after it is tapped
- Draws a small green marker near other touch points

# Dependencies

This example uses the display and touch APIs provided by this library and does not require any additional third-party Arduino libraries.

# Usage

- Build and upload examples/screen/screen.ino
- After boot, the screen displays a red square and a short prompt
- Tap the red square to move it to a new random position
- Tap elsewhere on the screen to leave a small green touch marker

# Notes

- This example only enables the display module; the LED, sensor, button, and battery modules are disabled
- If touch input does not respond or the display looks incorrect, first verify display initialization, touch hardware status, and board configuration