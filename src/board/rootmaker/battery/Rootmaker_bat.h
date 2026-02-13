#pragma once

#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "../rootmaker_pin.h"

#define PY32_I2C_ADDRESS    0x73

/* Register address definitions */
#define REG_VERSION_ID          0x00    // Firmware version (uint16_t, read-only)
#define REG_CHARGING_STATUS     0x02    // Charging status (uint8_t, read-only)
#define REG_BATTERY_VOLTAGE     0x03    // Battery voltage (uint32_t, read-only, unit: mV)
#define REG_SHUTDOWN            0x07    // Shutdown command (uint8_t, write-only)
#define REG_KEY_WAKEUP_TIME     0x08    // Key wakeup duration (uint16_t, read-write, unit: ms)
#define REG_KEY_SHUTDOWN_TIME   0x0A    // Key shutdown duration (uint16_t, read-write, unit: ms)
#define REG_TIMER_WAKEUP        0x0C    // Timer wakeup (uint32_t, write-only, unit: seconds)

/* Battery voltage threshold defaults (unit: mV) */
#define BATTERY_VOLTAGE_MAX     4200    // Full charge voltage (Li-ion 4.2V)
#define BATTERY_VOLTAGE_MIN     3300    // Empty battery voltage (Li-ion 3.3V)

/* Voltage filter */
#define VOLTAGE_FILTER_SIZE     64

/* Charging status */
typedef enum {
    CHARGING_STATUS_NOT_CHARGING    = 0,    // Not charging
    CHARGING_STATUS_CHARGING        = 1,    // Charging
    CHARGING_STATUS_COMPLETE        = 2,    // Charge complete
} ChargingStatus_t;

class Rootmaker_bat {
public:
    Rootmaker_bat();
    
    /**
     * @brief Initialize battery management module
     * @param wire I2C bus reference
     * @param mutex I2C mutex
     */
    void init(TwoWire& wire, SemaphoreHandle_t mutex);

    /**
     * @brief Read firmware version
     * @return Version number (format: 0xMMmm, e.g. 0x0100 means v1.0)
     */
    uint16_t getVersion(void);

    /**
     * @brief Read charging status
     * @return Charging status (0=not charging, 1=charging, 2=charge complete)
     */
    ChargingStatus_t getChargingStatus(void);

    /**
     * @brief Read battery voltage (raw value, no filtering)
     * @return Battery voltage (unit: mV)
     */
    uint32_t getVoltageRaw(void);

    /**
     * @brief Read battery voltage (filtered smooth value)
     * @return Battery voltage (unit: mV)
     */
    uint32_t getVoltage(void);

    /**
     * @brief Update voltage filter (sample a new value)
     * @note Must be called periodically to update the filter buffer
     */
    void updateVoltageFilter(void);

    /**
     * @brief Reset voltage filter
     * @note Clears historical samples, next read will refill the buffer
     */
    void resetVoltageFilter(void);

    /**
     * @brief Send immediate shutdown command
     * @return true: success, false: failure
     */
    bool shutdown(void);

    /**
     * @brief Read key wakeup duration
     * @return Key wakeup duration (unit: ms)
     */
    uint16_t getKeyWakeupTime(void);

    /**
     * @brief Set key wakeup duration
     * @param time_ms Key wakeup duration (unit: ms)
     * @return true: success, false: failure
     */
    bool setKeyWakeupTime(uint16_t time_ms);

    /**
     * @brief Read key shutdown duration
     * @return Key shutdown duration (unit: ms)
     */
    uint16_t getKeyShutdownTime(void);

    /**
     * @brief Set key shutdown duration
     * @param time_ms Key shutdown duration (unit: ms)
     * @return true: success, false: failure
     */
    bool setKeyShutdownTime(uint16_t time_ms);

    /**
     * @brief Set timer wakeup (shuts down immediately after write, auto powers on at time)
     * @param seconds Wakeup time (unit: seconds)
     * @return true: success, false: failure
     */
    bool setTimerWakeup(uint32_t seconds);

    /**
     * @brief Check if PY32 is online
     * @return true: online, false: offline
     */
    bool isConnected(void);

    /**
     * @brief Get remaining battery level percentage
     * @return Battery percentage (0-100)
     */
    uint8_t getBatteryLevel(void);

    /**
     * @brief Set battery voltage thresholds
     * @param max_mv Full charge voltage (unit: mV, default 4200)
     * @param min_mv Empty battery voltage (unit: mV, default 3300)
     */
    void setBatteryThreshold(uint32_t max_mv, uint32_t min_mv);

private:
    TwoWire* _wire;
    SemaphoreHandle_t _mutex;
    bool _initialized;
    uint32_t _voltage_max;  // Full charge voltage (mV)
    uint32_t _voltage_min;  // Empty battery voltage (mV)

    // Voltage filter
    uint32_t _voltage_buffer[VOLTAGE_FILTER_SIZE]; 
    uint8_t _voltage_index;                        
    uint8_t _voltage_count;                        
    uint32_t _voltage_sum;                         

    /**
     * @brief Read register
     * @param reg_addr Register address
     * @param data Data buffer
     * @param len Data length
     * @return true: success, false: failure
     */
    bool readRegister(uint8_t reg_addr, uint8_t* data, uint8_t len);

    /**
     * @brief Write register
     * @param reg_addr Register address
     * @param data Data buffer
     * @param len Data length
     * @return true: success, false: failure
     */
    bool writeRegister(uint8_t reg_addr, const uint8_t* data, uint8_t len);

    /**
     * @brief Acquire I2C mutex
     */
    void lockI2C(void);

    /**
     * @brief Release I2C mutex
     */
    void unlockI2C(void);
};
