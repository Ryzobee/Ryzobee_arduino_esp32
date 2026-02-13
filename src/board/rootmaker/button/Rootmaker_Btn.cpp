#include "Rootmaker_btn.h"



Rootmaker_btn::Rootmaker_btn() {
    
}

Button_state_t Rootmaker_btn::getState() {
    return button.isPressed() ? BUTTON_PRESSED : BUTTON_RELEASED;
}

void Rootmaker_btn::init() {
    _lastEvent = BTN_EVENT_NONE;
    _pressStartTime = 0;
    _lastReleaseTime = 0;
    _clickCount = 0;
    _waitingForDoubleClick = false;
    _longPressTriggered = false;
}

bool Rootmaker_btn::btn_change() {
    button.read();
    return button.wasPressed() || button.wasReleased();
}

Button_event_t Rootmaker_btn::update() {
    button.read();
    uint32_t now = millis();
    
    // Button was pressed
    if (button.wasPressed()) {
        _pressStartTime = now;
        _longPressTriggered = false;
    }
    
    if (button.isPressed()) {
        if (!_longPressTriggered && (now - _pressStartTime >= BTN_LONG_PRESS_TIME)) {
            _longPressTriggered = true;
            _waitingForDoubleClick = false;
            _clickCount = 0;
            _lastEvent = BTN_EVENT_LONG_PRESS;
            return BTN_EVENT_LONG_PRESS;
        }
        
        if (_longPressTriggered) {
            return BTN_EVENT_LONG_PRESSING;
        }
    }
    
    if (button.wasReleased()) {
        if (_longPressTriggered) {
            _longPressTriggered = false;
            return BTN_EVENT_NONE;
        }
        
        uint32_t pressDuration = now - _pressStartTime;
        
        if (pressDuration < BTN_SHORT_PRESS_MAX) {
            if (_waitingForDoubleClick && (now - _lastReleaseTime < BTN_DOUBLE_CLICK_TIME)) {
                _waitingForDoubleClick = false;
                _clickCount = 0;
                _lastEvent = BTN_EVENT_DOUBLE_CLICK;
                return BTN_EVENT_DOUBLE_CLICK;
            } else {
                _waitingForDoubleClick = true;
                _lastReleaseTime = now;
                _clickCount = 1;
            }
        }
    }
    
    if (_waitingForDoubleClick && (now - _lastReleaseTime >= BTN_DOUBLE_CLICK_TIME)) {
        _waitingForDoubleClick = false;
        _clickCount = 0;
        _lastEvent = BTN_EVENT_SHORT_PRESS;
        return BTN_EVENT_SHORT_PRESS;
    }
    
    return BTN_EVENT_NONE;
}

Button_event_t Rootmaker_btn::getLastEvent() {
    return _lastEvent;
}

void Rootmaker_btn::clearEvent() {
    _lastEvent = BTN_EVENT_NONE;
}

bool Rootmaker_btn::isLongPressing() {
    return button.isPressed() && _longPressTriggered;
}
