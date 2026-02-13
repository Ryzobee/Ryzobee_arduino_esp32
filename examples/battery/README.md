# Battery Monitor Example

[中文](./README_CN.md)

This example demonstrates how to use the PY32 battery management module via I2C communication.

# Features

- Read battery voltage (mV)
- Read charging status (Not Charging / Charging / Complete)
- Read firmware version
- Read/Write key wakeup/shutdown time settings
- Timer wakeup control
- Shutdown command

# Hardware Connection

| ESP32S3 | PY32 |
|---------|------|
| GPIO41 (SDA) | PB4 (I2C_SDA) |
| GPIO40 (SCL) | PB3 (I2C_SCL) |

# I2C Address

- PY32 slave address: `0x73`


