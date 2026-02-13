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
        /**
         * @brief Constructor
         * @param i2c I2C bus reference for touch controller
         * @param mutex I2C mutex for thread-safe access
         */
        Rootmaker_Lcd(TwoWire& i2c, SemaphoreHandle_t mutex);

        /**
         * @brief Configure screen parameters
         */
        void configure();

        /**
         * @brief Initialize hardware (needs to be called manually in setup())
         */
        void init();

        /**
         * @brief Clear the screen
         */
        void clear();

        /**
         * @brief Get panel device instance
         * @return Pointer to panel device
         */
        lgfx::Panel_Device* panel() { return &_panel_instance; }

        /**
         * @brief Read touch position with I2C lock
         * @param x Pointer to store X coordinate
         * @param y Pointer to store Y coordinate
         * @return true if touch detected, false otherwise
         */
        bool getTouch(uint16_t* x, uint16_t* y);
};
