#pragma once

#include <LIS2DW12Sensor.h>

#include "../rootmaker_pin.h"

#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define I2C_SDA SDA
#define I2C_SCL SCL

#define I2C_NUM 0

class Rootmaker_lis2dwtr {
    public:

        LIS2DW12Sensor* Accelero; 

        /**
         * @brief Constructor
         * @param i2c I2C bus reference
         * @param mutex I2C mutex for thread-safe access
         */
        Rootmaker_lis2dwtr(TwoWire& i2c, SemaphoreHandle_t mutex);

        /**
         * @brief Initialize accelerometer module
         */
        void init();

        /**
         * @brief Read acceleration data from sensor
         * @param buf Buffer to store acceleration data (X, Y, Z)
         * @param len Buffer length
         */
        void read_acceleration(int32_t buf[], int32_t len);
    
    private:
        TwoWire& _dev_i2c;
        SemaphoreHandle_t _i2c_mutex;
};
