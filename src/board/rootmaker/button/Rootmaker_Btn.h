#pragma once

#include "Button.h"

#include "../rootmaker_pin.h"

#define BUTN_PIN    BTN_PIN
#define DEBOUNCE_MS 10

// Time parameters (milliseconds)
#define BTN_LONG_PRESS_TIME     1000    // Long press threshold
#define BTN_DOUBLE_CLICK_TIME   300     // Double click interval time
#define BTN_SHORT_PRESS_MAX     500     // Short press maximum time

enum Button_state_t {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED  = 1,
};

// Button event types
enum Button_event_t {
    BTN_EVENT_NONE          = 0,    // No event
    BTN_EVENT_SHORT_PRESS   = 1,    // Short press
    BTN_EVENT_LONG_PRESS    = 2,    // Long press
    BTN_EVENT_DOUBLE_CLICK  = 3,    // Double click
    BTN_EVENT_LONG_PRESSING = 4,    // Long press (pressed state)
};

class Rootmaker_btn {
    public:

        Button button = Button(BUTN_PIN, true, DEBOUNCE_MS);

        /**
         * @brief Constructor
         */
        Rootmaker_btn();

        /**
         * @brief Initialize button module
         */
        void init();

        /**
         * @brief Get current button state
         * @return BUTTON_PRESSED or BUTTON_RELEASED
         */
        Button_state_t getState();

        /**
         * @brief Check if button state changed
         * @return true if button was pressed or released, false otherwise
         */
        bool btn_change();
        
        /**
         * @brief Button event detection (needs to be called periodically in the main loop)
         * @return Detected button event type
         */
        Button_event_t update();
        
        /**
         * @brief Get the last event detected
         * @return Last button event type
         */
        Button_event_t getLastEvent();
        
        /**
         * @brief Clear event
         */
        void clearEvent();
        
        /**
         * @brief Check if button is in long pressing state
         * @return true if long pressing, false otherwise
         */
        bool isLongPressing();

    private:
        Button_event_t  _lastEvent = BTN_EVENT_NONE;
        uint32_t        _pressStartTime = 0;             // Press start time
        uint32_t        _lastReleaseTime = 0;            // Last release time
        uint8_t         _clickCount = 0;                 // Click count
        bool            _waitingForDoubleClick = false;  // Waiting for double click
        bool            _longPressTriggered = false;     // Long press triggered
};
