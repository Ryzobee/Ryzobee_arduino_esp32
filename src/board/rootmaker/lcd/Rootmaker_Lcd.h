#pragma once

#include <LovyanGFX.hpp>
#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "Touch_CST816T.hpp"

#define LCD_WRITE_FREQ 40000000
#define LCD_READ_FREQ  16000000
#define LCD_BL_FREQ    44100
#define LCD_BL_CHANNEL 7

#define SCREEN_WIDTH  240

class Rootmaker_Lcd : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789  _panel_instance;
  
    lgfx::Bus_SPI       _bus_instance;
  
    lgfx::Light_PWM     _light_instance;
  
    lgfx::Touch_CST816T _touch_instance;

    TwoWire* _dev_i2c;
    SemaphoreHandle_t _i2c_mutex;
  
  public:
        Rootmaker_Lcd(TwoWire& i2c, SemaphoreHandle_t mutex);
        void configure();  // 配置屏幕参数 (在构造函数中自动调用)
        void init();       // 初始化硬件 (需要在 setup() 中手动调用)
        void clear();
        lgfx::Panel_Device* panel() { return &_panel_instance; }

        // 带锁的触摸读取
        bool getTouch(uint16_t* x, uint16_t* y);
};
