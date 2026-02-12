#pragma once

#include <LIS2DW12Sensor.h>

#include "../rootmaker_pin.h"

#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define I2C_SDA SDA
#define I2C_SCL SCL

#define I2C_NUM 0

// extern TwoWire dev_i2c;  // 使用 I2C 总线 0

class Rootmaker_lis2dwtr {
    public:

        LIS2DW12Sensor* Accelero; 

        Rootmaker_lis2dwtr(TwoWire& i2c, SemaphoreHandle_t mutex);
        void init();
        void read_acceleration(int32_t buf[], int32_t len);
    
    private:
        TwoWire& _dev_i2c;
        SemaphoreHandle_t _i2c_mutex;
};
