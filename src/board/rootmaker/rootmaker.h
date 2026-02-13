#pragma once

#include <LovyanGFX.hpp>
#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "rootmaker_pin.h"
#include "lcd/Touch_CST816T.hpp"
#include "lcd/Rootmaker_lcd.h"
#include "led/Rootmaker_led.h"
#include "button/Rootmaker_btn.h"
#include "lis2dwtr/Rootmaker_Lis2dwtr.h"
#include "battery/Rootmaker_bat.h"

#define SCREEN_WIDTH  240

class RootMaker {
  private:
    TwoWire _dev_i2c;
    SemaphoreHandle_t _i2c_mutex;
    bool _i2c_initialized;

  public:
    RootMaker(void);
    ~RootMaker(void);

    void begin(bool LCDEnable      = true, 
               bool LEDEnable      = true, 
               bool SensorEnable   = true, 
               bool ButtonEnable   = true,
               bool BatteryEnable  = true);

    Rootmaker_Lcd* lcd;
    Rootmaker_led led;
    Rootmaker_btn btn;
    Rootmaker_lis2dwtr* lis2dwtr;
    Rootmaker_bat bat;

    void lockI2C();
    void unlockI2C();
    TwoWire& getI2C() { return _dev_i2c; }
};
