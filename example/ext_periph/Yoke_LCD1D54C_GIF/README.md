# GIF Animation Example

[中文](./README_CN.md)

This example demonstrates how to play built-in GIF animations on a detachable LCD1D54C display connected to RootMaker and switch between them with the onboard button. It is useful for validating display output, button input, and the GIF decoding and playback pipeline.

# Features

- Initializes the display and button peripherals
- Uses the AnimatedGIF library to decode and play embedded GIF assets
- Switches to the next animation when the button is pressed
- Prints playback status and debug information to the serial monitor

# Dependencies

Install the following Arduino libraries in the Arduino IDE before building this example:

| Name | Version | Description |
|---|---|---|
| Yoke | Companion library | LCD1D54C peripheral driver |
| LovyanGFX | >= v1.2.19 | Display graphics API |
| [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF) | >= v2.2.0 | Library for decoding and playing GIF animations on embedded devices |

# Usage

- Install the required dependency
- Build and upload example/ext_periph/Yoke_LCD1D54C_GIF/Yoke_LCD1D54C_GIF.ino
- After boot, the display starts playing the embedded GIF animation automatically
- Press the button once to switch to the next GIF

# Notes

- The GIF data is compiled into the example as byte arrays, so no extra asset files are required at runtime
- This example enables the display and button modules, while the LED, sensor, and battery modules remain disabled
- Use `board.maker.begin(false, false, true, false)` to select onboard peripherals; initialize the display through a separate Yoke peripheral object.
- If playback does not behave as expected, first verify display initialization, library version compatibility, and the selected target board


The display is a detachable Yoke LCD1D54C peripheral. Install Yoke, LovyanGFX and AnimatedGIF. See the [LCD1D54C wiring](../Yoke_LCD1D54C/README.md).

The display now uses `Lcd1d54cPins` and `lcd.begin(pins)` with explicit GPIOs illustrating RootMaker wiring. This example still uses the Ryzobee onboard button and shared I2C bus; other ESP32 boards also require adapting button and bus initialization. The GPIO entry point does not reserve F_BUS; the application coordinates pin resources.
