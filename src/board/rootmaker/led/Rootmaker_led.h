#pragma once

#include <Adafruit_NeoPixel.h>

#include "../rootmaker_pin.h"

#define LED_PIN    PIN_RGB_LED
#define NUM_LEDS   1

class Rootmaker_led {
    public:

        Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);;
        uint8_t brightness;
        
        /**
         * @brief Constructor
         */
        Rootmaker_led();

        /**
         * @brief Initialize LED module
         */
        void init();

        // void setColor(uint8_t r, uint8_t g, uint8_t b);
        // void setColor(uint32_t color);
        // void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
        // void setColor(uint32_t color, uint8_t w);
};
