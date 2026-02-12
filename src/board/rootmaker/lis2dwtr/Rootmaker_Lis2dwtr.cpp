#include "Rootmaker_Lis2dwtr.h"

Rootmaker_lis2dwtr::Rootmaker_lis2dwtr(TwoWire& i2c, SemaphoreHandle_t mutex) 
    : Accelero(nullptr), _dev_i2c(i2c), _i2c_mutex(mutex) {
}

void Rootmaker_lis2dwtr::init() {
    if (xSemaphoreTake(_i2c_mutex, portMAX_DELAY) == pdTRUE) {
        Accelero = new LIS2DW12Sensor(&_dev_i2c); 
        Accelero->begin();
        Accelero->Enable_X();
        xSemaphoreGive(_i2c_mutex);
    }
}

void Rootmaker_lis2dwtr::read_acceleration(int32_t buf[], int32_t len) {
    if (len < 3 || Accelero == nullptr) {
        return;
    }
    
    if (xSemaphoreTake(_i2c_mutex, portMAX_DELAY) == pdTRUE) {
        Accelero->Get_X_Axes(buf);
        xSemaphoreGive(_i2c_mutex);
    }
}
