#include "Rootmaker_bat.h"
#include <string.h>

Rootmaker_bat::Rootmaker_bat()
    : _wire(nullptr), _mutex(nullptr), _initialized(false),
      _voltage_max(BATTERY_VOLTAGE_MAX), _voltage_min(BATTERY_VOLTAGE_MIN),
      _voltage_index(0), _voltage_count(0), _voltage_sum(0) {
    memset(_voltage_buffer, 0, sizeof(_voltage_buffer));
}

void Rootmaker_bat::init(TwoWire& wire, SemaphoreHandle_t mutex) {
    _wire = &wire;
    _mutex = mutex;
    _initialized = true;
}

void Rootmaker_bat::lockI2C(void) {
    if (_mutex != nullptr) {
        xSemaphoreTake(_mutex, portMAX_DELAY);
    }
}

void Rootmaker_bat::unlockI2C(void) {
    if (_mutex != nullptr) {
        xSemaphoreGive(_mutex);
    }
}

bool Rootmaker_bat::readRegister(uint8_t reg_addr, uint8_t* data, uint8_t len) {
    if (!_initialized || _wire == nullptr) {
        return false;
    }

    lockI2C();

    // Send register address
    _wire->beginTransmission(PY32_I2C_ADDRESS);
    _wire->write(reg_addr);
    uint8_t result = _wire->endTransmission(false);  // Send repeated start condition

    if (result != 0) {
        unlockI2C();
        return false;
    }

    // Read data
    uint8_t bytes_read = _wire->requestFrom(PY32_I2C_ADDRESS, len);
    if (bytes_read != len) {
        unlockI2C();
        return false;
    }

    for (uint8_t i = 0; i < len; i++) {
        data[i] = _wire->read();
    }

    unlockI2C();
    return true;
}

bool Rootmaker_bat::writeRegister(uint8_t reg_addr, const uint8_t* data, uint8_t len) {
    if (!_initialized || _wire == nullptr) {
        return false;
    }

    lockI2C();

    _wire->beginTransmission(PY32_I2C_ADDRESS);
    _wire->write(reg_addr);
    for (uint8_t i = 0; i < len; i++) {
        _wire->write(data[i]);
    }
    uint8_t result = _wire->endTransmission(true);

    unlockI2C();
    return (result == 0);
}

uint16_t Rootmaker_bat::getVersion(void) {
    uint8_t data[2] = {0};
    if (!readRegister(REG_VERSION_ID, data, 2)) {
        return 0;
    }
    
    return (uint16_t)(data[0] | (data[1] << 8));
}

ChargingStatus_t Rootmaker_bat::getChargingStatus(void) {
    uint8_t data = 0;
    if (!readRegister(REG_CHARGING_STATUS, &data, 1)) {
        return CHARGING_STATUS_NOT_CHARGING;
    }
    return (ChargingStatus_t)data;
}

uint32_t Rootmaker_bat::getVoltageRaw(void) {
    uint8_t data[4] = {0};
    if (!readRegister(REG_BATTERY_VOLTAGE, data, 4)) {
        return 0;
    }
    return (uint32_t)(data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
}

void Rootmaker_bat::updateVoltageFilter(void) {
    uint32_t raw_voltage = getVoltageRaw();
    if (raw_voltage == 0) {
        return;  // Read failed, don't update filter
    }

    // If buffer is full, subtract the old value about to be overwritten
    if (_voltage_count >= VOLTAGE_FILTER_SIZE) {
        _voltage_sum -= _voltage_buffer[_voltage_index];
    } else {
        _voltage_count++;
    }

    // Write new value and update sum
    _voltage_buffer[_voltage_index] = raw_voltage;
    _voltage_sum += raw_voltage;

    // Move index (circular)
    _voltage_index = (_voltage_index + 1) % VOLTAGE_FILTER_SIZE;
}

void Rootmaker_bat::resetVoltageFilter(void) {
    memset(_voltage_buffer, 0, sizeof(_voltage_buffer));
    _voltage_index = 0;
    _voltage_count = 0;
    _voltage_sum = 0;
}

uint32_t Rootmaker_bat::getVoltage(void) {
    // Auto-sample and update filter on each call
    updateVoltageFilter();

    // If no valid data, return 0
    if (_voltage_count == 0) {
        return 0;
    }

    // Return average value
    return _voltage_sum / _voltage_count;
}

bool Rootmaker_bat::shutdown(void) {
    uint8_t cmd = 0x01;
    return writeRegister(REG_SHUTDOWN, &cmd, 1);
}

uint16_t Rootmaker_bat::getKeyWakeupTime(void) {
    uint8_t data[2] = {0};
    if (!readRegister(REG_KEY_WAKEUP_TIME, data, 2)) {
        return 0;
    }
    return (uint16_t)(data[0] | (data[1] << 8));
}

bool Rootmaker_bat::setKeyWakeupTime(uint16_t time_ms) {
    uint8_t data[2];
    data[0] = (uint8_t)(time_ms & 0xFF);
    data[1] = (uint8_t)((time_ms >> 8) & 0xFF);
    return writeRegister(REG_KEY_WAKEUP_TIME, data, 2);
}

uint16_t Rootmaker_bat::getKeyShutdownTime(void) {
    uint8_t data[2] = {0};
    if (!readRegister(REG_KEY_SHUTDOWN_TIME, data, 2)) {
        return 0;
    }
    return (uint16_t)(data[0] | (data[1] << 8));
}

bool Rootmaker_bat::setKeyShutdownTime(uint16_t time_ms) {
    uint8_t data[2];
    data[0] = (uint8_t)(time_ms & 0xFF);
    data[1] = (uint8_t)((time_ms >> 8) & 0xFF);
    return writeRegister(REG_KEY_SHUTDOWN_TIME, data, 2);
}

bool Rootmaker_bat::setTimerWakeup(uint32_t seconds) {
    uint8_t data[4];
    data[0] = (uint8_t)(seconds & 0xFF);
    data[1] = (uint8_t)((seconds >> 8) & 0xFF);
    data[2] = (uint8_t)((seconds >> 16) & 0xFF);
    data[3] = (uint8_t)((seconds >> 24) & 0xFF);
    return writeRegister(REG_TIMER_WAKEUP, data, 4);
}

bool Rootmaker_bat::isConnected(void) {
    if (!_initialized || _wire == nullptr) {
        return false;
    }

    lockI2C();

    _wire->beginTransmission(PY32_I2C_ADDRESS);
    uint8_t result = _wire->endTransmission(true);

    unlockI2C();
    return (result == 0);
}

uint8_t Rootmaker_bat::getBatteryLevel(void) {
    uint32_t voltage = getVoltage();
    
    // Voltage below minimum, return 0%
    if (voltage <= _voltage_min) {
        return 0;
    }
    
    // Voltage above maximum, return 100%
    if (voltage >= _voltage_max) {
        return 100;
    }
    
    // Calculate battery percentage
    uint32_t range = _voltage_max - _voltage_min;
    uint32_t level = ((voltage - _voltage_min) * 100) / range;
    
    return (uint8_t)level;
}

void Rootmaker_bat::setBatteryThreshold(uint32_t max_mv, uint32_t min_mv) {
    _voltage_max = max_mv;
    _voltage_min = min_mv;
}
