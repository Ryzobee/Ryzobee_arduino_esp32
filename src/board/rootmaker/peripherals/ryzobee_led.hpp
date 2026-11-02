#pragma once

#include "../rootmaker_pin.h"
#include "esp32-hal-rgb-led.h"

#define LED_PIN    PIN_RGB_LED

class ryzobee_led {
    public:
        ryzobee_led(){};

        void init(){
            if (_initialized) return;
            rgbLedWriteOrdered(LED_PIN, LED_COLOR_ORDER_GRB, 0, 0, 0);
            _initialized = true;
        }

        void setColor(uint8_t r, uint8_t g, uint8_t b){
            _ensureInitialized();
            rgbLedWriteOrdered(LED_PIN, LED_COLOR_ORDER_GRB, r, g, b);
        }
        void setColor(uint32_t color){
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            setColor(r, g, b);
        }
        void setHSV(uint32_t h, uint32_t s, uint32_t v){setColor(hsv2rgb(h, s, v));};

        uint32_t hsv2rgb(uint32_t h, uint32_t s, uint32_t v) {
            h %= 360;
            uint32_t rgb_max = v * 2.55f;
            uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;
            uint32_t i = h / 60;
            uint32_t diff = h % 60;
            uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;
            uint8_t r, g, b;
            switch (i) {
            case 0: r = rgb_max; g = rgb_min + rgb_adj; b = rgb_min; break;
            case 1: r = rgb_max - rgb_adj; g = rgb_max; b = rgb_min; break;
            case 2: r = rgb_min; g = rgb_max; b = rgb_min + rgb_adj; break;
            case 3: r = rgb_min; g = rgb_max - rgb_adj; b = rgb_max; break;
            case 4: r = rgb_min + rgb_adj; g = rgb_min; b = rgb_max; break;
            default: r = rgb_max; g = rgb_min; b = rgb_max - rgb_adj; break; }
            return (r << 16) | (g << 8) | b;
        }
    private:
        void _ensureInitialized(){
            if (!_initialized) {
                init();
            }
        }
        bool _initialized = false;
};
