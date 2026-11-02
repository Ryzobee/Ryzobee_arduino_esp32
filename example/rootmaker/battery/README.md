# Battery Monitor Example

[中文](./README_CN.md)

This example shows how to communicate with the PY32 battery management module over I2C and print battery information to the serial monitor while using the onboard RGB LED as a status indicator. It is intended as a quick reference for validating hardware connections, checking communication with the battery manager, and learning the basic battery-related APIs provided by the library.

# Features

- Read battery voltage in mV
- Read battery percentage
- Read charging status: Not Charging, Charging, or Charge Complete
- Read the battery manager firmware version
- Read and configure key wakeup and shutdown timing
- Configure timer wakeup
- Send a shutdown command

# Hardware Connection

| ESP32S3 | PY32 |
|---------|------|
| GPIO41 (SDA) | PB4 (I2C_SDA) |
| GPIO40 (SCL) | PB3 (I2C_SCL) |

# I2C Address

- PY32 slave address: `0x73`

# Battery Level Notes

The battery percentage shown in this example is not derived from a simple linear formula. Instead, it is estimated from a fitted discharge curve for a typical single-cell Li-ion battery.

- 100% corresponds to approximately 4200 mV
- 0% corresponds to approximately 3450 mV
- Values between 3450 mV and 4200 mV are estimated using a piecewise curve-fitting model

In other words, the reported voltage is the measured battery voltage, while the percentage is an estimated value derived from that voltage. Because of load conditions, sampling noise, and filtering, the reported voltage and percentage will not always map one-to-one at every moment.

# How It Works

- On startup, the example initializes the Rootmaker LED, button, and battery peripherals and the PY32 battery management module
- If I2C communication succeeds, the serial monitor shows the firmware version, battery voltage, battery level, charging state, and key timing configuration
- When the battery level changes, the RGB LED updates its color and starts blinking as a visual notification
- Pressing the button stops the blinking notification

# Notes

- This example requires a working PY32 battery management module on the hardware
- Use `board.maker.begin(true, false, true, true)` to select onboard peripherals; initialize the display through a separate Yoke peripheral object.
- If no valid battery report appears, first check power, I2C wiring, and the slave address
- If you want the printed voltage and percentage calculation to use the exact same data source, you can modify the example to use the filtered voltage API for reporting as well











