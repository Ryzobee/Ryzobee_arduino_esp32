#include "rootmaker.h"

RootMaker::RootMaker(void) 
    : _dev_i2c(0), _i2c_mutex(nullptr), _i2c_initialized(false),
      lcd(nullptr), lis2dwtr(nullptr) {
    // 构造函数中不做硬件初始化
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
                      bool ButtonEnable) {
    // 初始化 I2C 互斥锁
    if (_i2c_mutex == nullptr) {
        _i2c_mutex = xSemaphoreCreateMutex();
    }

    // 首先初始化共享 I2C 总线
    // 触摸屏和加速度传感器共用同一组 I2C 引脚 (41/40)
    // 现在统一使用 Arduino Wire 作为唯一的 I2C 驱动
    if (!_i2c_initialized) {
        _dev_i2c.begin(I2C_SDA, I2C_SCL);
        _i2c_initialized = true;
    }

    // 初始化 LCD (触摸屏现在使用 Wire 而不是 lgfx::i2c)
    if (LCDEnable) {
        if (lcd == nullptr) {
            lcd = new Rootmaker_Lcd(_dev_i2c, _i2c_mutex);
        }
        lcd->init();
    }

    // 初始化 LED
    if (LEDEnable) {
        led.init();
    }

    // 初始化加速度传感器 (与触摸屏共享同一个 Wire 和互斥锁)
    if (SensorEnable) {
        if (lis2dwtr == nullptr) {
            lis2dwtr = new Rootmaker_lis2dwtr(_dev_i2c, _i2c_mutex);
        }
        lis2dwtr->init();
    }

    // 初始化按钮
    if (ButtonEnable) {
        btn.init();
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
