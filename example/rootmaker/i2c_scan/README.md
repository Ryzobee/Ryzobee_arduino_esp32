# I2C Scanner Example

[中文](./README_CN.md)

This example demonstrates how to scan all four I2C ports exposed by the Rootmaker board and print the detected device addresses for each port to the serial monitor. It is useful for debugging I2C connections and quickly identifying which bus a peripheral is attached to.

# Features

- Initializes and scans four I2C ports in sequence
- Checks the full 7-bit I2C address range on each port
- Prints detected device addresses and the total number of devices per port
- Prompts the user to reset the board to run the scan again

# Dependencies

This example uses the Arduino Wire library and the pin definitions provided by this library. No additional third-party Arduino libraries are required.

# Usage

- Build and upload examples/i2c_scan/i2c_scan.ino
- Open the serial monitor at 115200 baud
- After boot, the sketch scans PORT1 through PORT4 and prints the results
- Press the reset button on the board whenever you want to run the scan again

# Notes

- This example is intended for I2C debugging and device discovery, so it does not depend on the normal Rootmaker peripheral initialization flow
- If no devices are found, first verify module power, SDA and SCL wiring, pull-up resistors, and the expected device address