#include "rootmaker.h"

RootMaker::RootMaker(void) 
    : _dev_i2c(0), _i2c_mutex(nullptr), _i2c_initialized(false),
      lcd(nullptr), lis2dwtr(nullptr) {
}

RootMaker::~RootMaker(void) {
    if (lcd != nullptr) {
        delete lcd;
        lcd = nullptr;
    }
    if (lis2dwtr != nullptr) {
        delete lis2dwtr;
        lis2dwtr = nullptr;
    }
    if (_i2c_mutex != nullptr) {
        vSemaphoreDelete(_i2c_mutex);
        _i2c_mutex = nullptr;
    }
}

void RootMaker::begin(bool LCDEnable, 
                      bool LEDEnable, 
                      bool SensorEnable, 
                      bool ButtonEnable,
                      bool BatteryEnable) {
    if (_i2c_mutex == nullptr) {
        _i2c_mutex = xSemaphoreCreateMutex();
    }

    // Initialize shared I2C bus (100kHz, compatible with PY32)
    if (!_i2c_initialized) {
        _dev_i2c.begin(I2C_SDA, I2C_SCL, 100000);
        _i2c_initialized = true;
    }

    // Initialize LCD
    if (LCDEnable) {
        if (lcd == nullptr) {
            lcd = new Rootmaker_Lcd(_dev_i2c, _i2c_mutex);
        }
        lcd->init();
    }

    // Initialize LED
    if (LEDEnable) {
        led.init();
    }

    // Initialize accelerometer sensor
    if (SensorEnable) {
        if (lis2dwtr == nullptr) {
            lis2dwtr = new Rootmaker_lis2dwtr(_dev_i2c, _i2c_mutex);
        }
        lis2dwtr->init();
    }

    // Initialize button
    if (ButtonEnable) {
        btn.init();
    }

    // Initialize battery management module
    if (BatteryEnable) {
        bat.init(_dev_i2c, _i2c_mutex);
    }
}

void RootMaker::lockI2C() {
    if (_i2c_mutex != nullptr) {
        xSemaphoreTake(_i2c_mutex, portMAX_DELAY);
    }
}

void RootMaker::unlockI2C() {
    if (_i2c_mutex != nullptr) {
        xSemaphoreGive(_i2c_mutex);
    }
}
