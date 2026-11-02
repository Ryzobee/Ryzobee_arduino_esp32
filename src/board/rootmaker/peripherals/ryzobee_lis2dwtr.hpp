#pragma once

//#include <LIS2DW12Sensor.h>

#include "../rootmaker_pin.h"
#include "LIS2DW12Sensor.h"
#include <math.h>
#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class ryzobee_lis2dwtr {
    public:
        /**
         * @brief Constructor
         * @param i2c I2C bus reference
         * @param mutex I2C mutex for thread-safe access
         */
        ryzobee_lis2dwtr(TwoWire* i2c, SemaphoreHandle_t* mutex):_dev_i2c(i2c), _i2c_mutex(mutex) {
            _accelero = new LIS2DW12Sensor(_dev_i2c, LIS2DW12_I2C_ADD_H); 
            _x_filter = 0.0f;
            _y_filter = 0.0f;
            _z_filter = 0.0f;
        };
        void init(bool enable_filter = true, float alpha = 0.2f){
            if (_initialized) return;
            set_enable_filter(enable_filter, alpha);
            if (xSemaphoreTake(*_i2c_mutex,portMAX_DELAY) == pdTRUE) {
                _accelero->begin();
                _accelero->Enable_X();
                xSemaphoreGive(*_i2c_mutex);
            }
            else {
                ESP_LOGE("ryzobee_lis2dwtr", "Failed to take I2C mutex");
            }
            xSemaphoreGive(*_i2c_mutex);
            _initialized = true;
        };
        LIS2DW12Sensor* get_accelero(){ return _accelero; }

        void set_enable_filter(bool enable, float alpha = 0.1f){
            _enable_filter = enable;
            _filter_alpha = alpha;
        };
        void set_filter_alpha(float alpha){ _filter_alpha = alpha; }
        float get_filter_alpha(){ return _filter_alpha; }

        void flush(){
            int32_t acc[3];
            if (xSemaphoreTake(*_i2c_mutex,portMAX_DELAY) == pdTRUE) {
                _accelero->Get_X_Axes(acc);
                xSemaphoreGive(*_i2c_mutex);
            }
            else {
                ESP_LOGE("ryzobee_lis2dwtr", "Failed to take I2C mutex");
            }
            if (_enable_filter) {
                x =_x_filter = _filter_alpha * _x_filter + (1 - _filter_alpha) * ((float)acc[0] / 1000.0f);
                y = _y_filter = _filter_alpha * _y_filter + (1 - _filter_alpha) * ((float)acc[1] / 1000.0f);
                z = _z_filter = _filter_alpha * _z_filter + (1 - _filter_alpha) * ((float)acc[2] / 1000.0f);
            }
            else {
                x = (float)acc[0] / 1000.0f;
                y = (float)acc[1] / 1000.0f;
                z = (float)acc[2] / 1000.0f;
            }
            roll = atan2((x), z) * 180 / M_PI;
            pitch = atan2((0-y),z) * 180 / M_PI;
        };

    public:
        float x, y, z;
        float roll, pitch;

    private:
        TwoWire* _dev_i2c = nullptr;
        SemaphoreHandle_t* _i2c_mutex = nullptr;
        LIS2DW12Sensor* _accelero = nullptr;
        bool _enable_filter = false;
        float _filter_alpha = 0.1f;
        float _x_filter, _y_filter, _z_filter;
        bool _initialized = false;
};
