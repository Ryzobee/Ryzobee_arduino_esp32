# Button Input Example

[中文](./README_CN.md)

This example demonstrates how to use the onboard button on the Rootmaker board to detect different button events and print them to the serial monitor. It is useful for verifying button input behavior and learning the basic button event APIs provided by the library.

# Features

- Initializes the onboard button module
- Polls button event state in the main loop
- Detects and prints short press, long press, double click, and long pressing events

# Dependencies

This example uses the button APIs provided by this library and does not require any additional third-party Arduino libraries.

# Usage

- Build and upload examples/btn/btn.ino
- Open the serial monitor at 115200 baud
- Press the onboard button and observe the reported event types in the serial output

# Notes

- This example only enables the button module; the display, LED, sensor, and battery modules are disabled
- If no output appears, first verify the selected board type, serial baud rate, and button hardware status