# TOUCH4 Four-Channel Touch Example

[中文](README_CN.md)

This driver is extracted from the SC92F8372/8371/8370 touch firmware in [mian.c](../mian.c). `TOUCH4` describes a four-channel touch module, not a commercial model specified by the firmware. The original file is retained as the protocol source. Only `SSI_MODE == UART` is supported; I2C is not implemented.

## Wiring and operation

Install Ryzobee, Yoke and the ESP32 board core; no additional third-party library is needed. Build and upload `Yoke_TOUCH4.ino`. The example requests the touch mask and four raw values every 100 ms. Each command waits up to 1000 ms. The firmware does not stream reports automatically; requests are required.

| Host RS_PORT4 signal | RootMaker GPIO | Module signal |
| --- | --- | --- |
| Second pin / SCL, host TX | 18 | RX |
| First pin / SDA, host RX | 17 | TX |
| GND | GND | GND |

The example swaps the two RS_PORT4 pin entries before initialization, using TX=GPIO18 and RX=GPIO17 to test the module UART wiring direction.

Power the module according to its actual specification and use ESP32-compatible UART levels; do not infer module supply voltage from the MCU name. UART is fixed at 9600 baud, 8 data bits, no parity, 1 stop bit. Each instance owns a UART; do not share it with RAD60 or a serial console. Successful `begin()` means the host UART is ready; successful `getVersion()` / `readData()` confirms a valid device reply.

## API

| Method | Function |
| --- | --- |
| `begin(board.maker.useport("rs_port4"))` | Initialize and reserve an RS port; returns `YOKE_PORT_ERROR_NONE` on success |
| `readData()` | Request and cache all four touch channels; returns true on success |
| `update()` | Drain and parse UART input without blocking or sending requests |
| `getTouchMask()` / `isTouched(channel)` | Read cached touch bits for logical channels 0-3 |
| `getRawData(channel)` | Read a cached 16-bit raw value |
| `hasData()` | Whether a valid data reply has arrived since initialization, not a freshness indication |
| `setEnabledChannels(mask)` | Set the channel enable mask, 0x00-0x0F |
| `setThresholds(data)` | Write 8 threshold bytes; firmware reinitializes touch |
| `reconfigure()` | Reinitialize touch sensing |
| `getChannelConfig(channel, data)` | Read 8 bytes from a firmware configuration channel |
| `setChannelConfig(channel, data)` | Write 8 configuration bytes; call `reconfigure()` afterwards to apply |
| `getVersion(&version)` | Query the 16-bit firmware version; the source defines 0x1001 |
| `end()` | Release UART and the RS port; keep port data valid until complete |

Failed reads preserve the previous cache; use the current reading only when `readData()` returns true. Serialize commands; concurrent tasks must not use the same instance.

Raw report order is `RawData[3]`, `RawData[2]`, `RawData[0]`, `RawData[1]`. Touch mask bits 0-3 come from `exKeyValueFlag` masks 0x0100, 0x0020, 0x0001, 0x0002 respectively. Configuration channel indices directly address `TKChannelCfg[channel]`; do not assume they use report order. The threshold command writes `TKChannelCfg[0..3][6..7]`. Configuration byte semantics are not defined by the source, so the driver preserves opaque bytes without inventing threshold units or parameter meanings. The example leaves default configuration unchanged.

## UART protocol

Frames use `AA 55 CMD LEN DATA... CRC EE`. CRC8 polynomial 0x31, initial value 0, no reflection or final XOR, covering CMD, LEN and DATA only. Raw values and versions are big-endian. Oversized lengths, invalid CRCs and invalid tails are rejected.

| Request → reply | Payload |
| --- | --- |
| 0x21 → 0x11 | Request: 1 placeholder byte; reply: 9 bytes, touch mask plus four 16-bit raw values |
| 0x22 → 0x12 | Request: enable mask; reply: applied mask |
| 0x23 → 0x13 | Request: 8 threshold bytes; reply: 1 on success |
| 0x24 → 0x14 | Request: 1 placeholder byte; reply: 1 on success |
| 0x25 → 0x15 | Request: configuration channel index; reply: 8 configuration bytes |
| 0x26 → 0x16 | Request: channel index plus 8 bytes; reply: `1 | (channel << 4)` |
| 0x2F → 0x1F | Request: 1 placeholder byte; reply: 2-byte version |

Firmware `case 0x26` lacks a `break`, potentially sending an additional version frame and overwriting an ACK still being transmitted. The driver matches replies to the current command and does not accept the extra version as a configuration ACK. If configuration acknowledgement fails, fix this transmit flow in the device firmware. The protocol has no request sequence number, so late replies after a timeout cannot be fully distinguished from replies to a new request of the same kind.

## Timeout diagnosis

Raw UART tracing is disabled by default. The example prints `Channel N pressed/released` on touch and release transitions, using display numbers 1-4 (driver indices remain 0-3). Holding a channel does not repeat messages, and raw values are not printed continuously. Persistent read failures are reported at most every 5 seconds after the first immediate warning. To diagnose communication, add `touch.setDebugStream(&Serial)` before the first query to print transmitted frames, received bytes and CRC-validated frames. TX without RX indicates the need to check power, wiring, RX/TX direction and UART firmware mode. RX without valid frames requires checking baud rate, CRC and the actual protocol. Valid frames followed by timeouts require checking the reply command and length. Disable tracing with `touch.setDebugStream(nullptr)` afterwards. Use a console separate from the device UART.

## Button-style events

After each successful read, the example compares the old and new masks and prints press/release events per channel, such as `Channel 1 pressed` and `Channel 1 released`. Channels held at startup produce a press on the first read. Polling remains at 100 ms, not a hardware interrupt; changes during communication outages or completed between polls may be missed. Fluctuating device states still generate alternating events. Recovery compares against the last valid state without repeating presses for unchanged held channels.
