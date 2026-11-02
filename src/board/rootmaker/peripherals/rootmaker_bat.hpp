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

class rootmaker_ec{
    public:
        rootmaker_ec(TwoWire* wire, SemaphoreHandle_t* mutex):_wire(wire), _mutex(mutex){};
        void init(){
            if (!_isConnected()) {
                ESP_LOGE("rootmaker_ec", "PY32 is not connected");
                return;
            }
            _initialized = true;
        }

    uint16_t getVersion(void){ return _read_16bit_register(REG_VERSION_ID);}
    uint32_t getVoltage(void){ return _read_32bit_register(REG_BATTERY_VOLTAGE);}

    ChargingStatus_t getChargingStatus(void){ return (ChargingStatus_t)_read_8bit_register(REG_CHARGING_STATUS);}
    bool shutdown(void){return _write_8bit_register(REG_SHUTDOWN, 0x01);}
    uint16_t getKeyWakeupTime(void){return _read_16bit_register(REG_KEY_WAKEUP_TIME);}
    bool setKeyWakeupTime(uint16_t time_ms){return _write_16bit_register(REG_KEY_WAKEUP_TIME, time_ms);}
    uint16_t getKeyShutdownTime(void){return _read_16bit_register(REG_KEY_SHUTDOWN_TIME);}
    bool setKeyShutdownTime(uint16_t time_ms){return _write_16bit_register(REG_KEY_SHUTDOWN_TIME, time_ms);}
    bool setTimerWakeup(uint32_t seconds){return _write_32bit_register(REG_TIMER_WAKEUP, seconds);}
    uint8_t getBatteryLevel(void){return _voltToPercent(getVoltage());}
    bool isCharging(void){return getChargingStatus() == CHARGING_STATUS_CHARGING;}
    bool isChargeFull(void){return getChargingStatus() == CHARGING_STATUS_COMPLETE;}

    private:
        inline void _lockWire(void){xSemaphoreTake(*_mutex, portMAX_DELAY);}
        inline void _lockWire(uint32_t timeout_ms){xSemaphoreTake(*_mutex, timeout_ms);}
        inline void _unlockWire(void){xSemaphoreGive(*_mutex);}
        bool _read_register(uint8_t reg_addr, uint8_t* data, int len) {
            if (!_initialized || _wire == nullptr|| _mutex == nullptr) { return false;}
            _lockWire();
            _wire->beginTransmission(PY32_I2C_ADDRESS);
            _wire->write(reg_addr);
            uint8_t result = _wire->endTransmission(false);  // Send repeated start condition
        
            if (result != 0) { _unlockWire(); return false;}
            uint8_t bytes_read = _wire->requestFrom(PY32_I2C_ADDRESS, len);
            if (bytes_read != len) {_unlockWire(); return false;}
            for (int i = 0; i < len; i++) { data[i] = _wire->read();}
            _unlockWire();
            return true;
        }
        
        bool _write_register(uint8_t reg_addr, const uint8_t* data, int len) {
            if (!_initialized || _wire == nullptr|| _mutex == nullptr) {return false;}
            _lockWire();
            _wire->beginTransmission(PY32_I2C_ADDRESS);
            _wire->write(reg_addr);
            for (int i = 0; i < len; i++) {_wire->write(data[i]);}
            uint8_t result = _wire->endTransmission(true);
            _unlockWire();
            return (result == 0);
        }

        bool _isConnected(void){
            _lockWire();
            _wire->beginTransmission(PY32_I2C_ADDRESS);
            uint8_t result = _wire->endTransmission(true);
            _unlockWire();
            return (result == 0);
        }

        bool _write_8bit_register(uint8_t reg_addr, uint8_t data) { return _write_register(reg_addr, &data, 1);}
        bool _write_16bit_register(uint8_t reg_addr, uint16_t data) { return _write_register(reg_addr, (uint8_t*)&data, 2);}
        bool _write_32bit_register(uint8_t reg_addr, uint32_t data) { return _write_register(reg_addr, (uint8_t*)&data, 4);}

        uint8_t _read_8bit_register(uint8_t reg_addr) { uint8_t data = 0; if (!_read_register(reg_addr, &data, 1)) {return 0;} return data;}
        uint16_t _read_16bit_register(uint8_t reg_addr) { uint16_t data = 0; if (!_read_register(reg_addr, (uint8_t*)&data, 2)) {return 0;} return data;}
        uint32_t _read_32bit_register(uint8_t reg_addr) { uint32_t data = 0; if (!_read_register(reg_addr, (uint8_t*)&data, 4)) {return 0;} return data;}
    
        uint8_t _voltToPercent(uint32_t volt_mv) {
            double y;
            double x = (double)volt_mv / 1000.0;
            if (x < 3.45) { y = 0;} 
            else if (x >= 3.4 && x <= 3.59) { y = 0.0882 * x * x - 0.4418 * x + 0.5476;} 
            else if (x > 3.59 && x <= 4.049) { y = -5.5508 * x * x * x + 64.741 * x * x - 249.57 * x + 318.5;} 
            else if (x > 4.049 && x <= 4.2) { y = -2.8846 * x * x + 24.51 * x - 51.049; } 
            else { y = 1;}
            
            uint8_t percent = ( y >= 0 ) ? (uint8_t)(y * 100) : 0;
            return percent > 100 ? 100 : percent;
        }

        TwoWire* _wire;
        SemaphoreHandle_t* _mutex;
        bool _initialized = false;
};