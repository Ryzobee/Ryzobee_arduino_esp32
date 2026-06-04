# GIF Animation Example

[中文](./README_CN.md)

This example demonstrates how to play built-in GIF animations on the Rootmaker display and switch between them with the onboard button. It is useful for validating display output, button input, and the GIF decoding and playback pipeline.

# Features

- Initializes the display and button peripherals
- Uses the AnimatedGIF library to decode and play embedded GIF assets
- Switches to the next animation when the button is pressed
- Prints playback status and debug information to the serial monitor

# Dependencies

Install the following Arduino library in the Arduino IDE before building this example:

| Name | Version | Description |
|---|---|---|
| [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF) | >= v2.2.0 | Library for decoding and playing GIF animations on embedded devices |

# Usage

- Install the required dependency
- Build and upload examples/gif/gif.ino
- After boot, the display starts playing the embedded GIF animation automatically
- Press the button once to switch to the next GIF

# Notes

- The GIF data is compiled into the example as byte arrays, so no extra asset files are required at runtime
- This example enables the display and button modules, while the LED, sensor, and battery modules remain disabled
- If playback does not behave as expected, first verify display initialization, library version compatibility, and the selected target board

