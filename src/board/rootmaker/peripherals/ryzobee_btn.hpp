#pragma once
#include <vector>
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

#define UPDATA_CECK(FUN,FLAG) if(!FLAG){FUN();}

class btn_unit {
public:
    btn_unit(uint8_t pin, uint8_t invert, uint32_t dbTime){
        _pin = pin;
        _invert = invert;
        _dbTime = dbTime;
        pinMode(_pin, INPUT_PULLUP);
        _state = digitalRead(_pin);
        if (_invert != 0) _state = !_state;
        _time = millis();
        _lastState = _state;
        _changed = 0;
        _hold_time = -1;
        _lastTime = _time;
        _lastChange = _time;
        _pressTime = _time;
        _task_running = false;
    }
    uint8_t isPressed(void) { return _state == 0 ? 0 : 1;}
    uint8_t isReleased(void) { return _state == 0 ? 1 : 0;}
    
    // TODO Test these two features.
    // uint8_t wasPressed(void) { return _state && _changed; }
    // uint8_t wasReleased(void) { return !_state && _changed && millis() - _pressTime < _hold_time;}

    uint8_t pressedFor(uint32_t ms) { return (_state == 1 && _time - _lastChange >= ms) ? 1 : 0;}
    uint8_t releasedFor(uint32_t ms) { return (_state == 0 && _time - _lastChange >= ms) ? 1 : 0;}
    uint8_t wasReleasefor(uint32_t ms) { return _changed && millis() - _pressTime >= ms;}
    uint32_t lastChange(void) { return _lastChange;}
    uint8_t  update(void) {
        static uint32_t ms;
        static uint8_t pinVal;
        ms = millis();
        pinVal = digitalRead(_pin);
        if (_invert != 0) pinVal = !pinVal;
        if (ms - _lastChange < _dbTime) {
          _lastTime = _time;_time = ms; _changed = 0;
          return _state;
        }
        else {
          _lastTime = _time; _time = ms; _lastState = _state; _state = pinVal;
          if (_state != _lastState) {
            _lastChange = ms; _changed = 1;
            if (_state) { _pressTime = _time; }
          }
          else { _changed = 0;}
          return _state;
        }
        return _state;
    }
    bool waitForRelease(uint32_t ms) {
        uint32_t start = millis();
        UPDATA_CECK(update, _task_running);
        while (!isReleased()) {
            if (millis() - start >= ms) {
                return false;
            }
            UPDATA_CECK(update, _task_running);
            delay(1);
        }
        return true;
    }
    bool waitForPress(uint32_t ms) {
        uint32_t start = millis();
        UPDATA_CECK(update, _task_running);
        while (!isPressed()) {
            if (millis() - start >= ms) {
                return false;
            }
        }
    }

    uint8_t _pin;           //arduino pin number
    uint8_t _puEnable;      //internal pullup resistor enabled
    uint8_t _invert;        //if 0, interpret high state as pressed, else interpret low state as pressed
    uint8_t _state;         //current button state
    uint8_t _lastState;     //previous button state
    uint8_t _changed;       //state changed since last read
    uint32_t _time;         //time of current state (all times are in ms)
    uint32_t _lastTime;     //time of previous state
    uint32_t _lastChange;   //time of last state change
    uint32_t _dbTime;       //debounce time
    uint32_t _pressTime;    //press time
    uint32_t _hold_time;    //hold time call wasreleasefor
    uint16_t _index;        //index of the button
    bool _task_running;     //task running
};

class ryzobee_btn {

    public:
    ryzobee_btn(){_task_handle = NULL;}
    ~ryzobee_btn(){
        if (_task_handle != NULL){ vTaskDelete(_task_handle);}
        for (auto &btn : buttons){ delete btn;}
    }
    void init(bool use_task = false){
        if (_task_handle != NULL){ vTaskDelete(_task_handle);}
        if (use_task){
            xTaskCreate(_task, "ryzobee_btn", 1024, (void*)this, 5, &_task_handle);
        }
        else{
            _task(this);
            _task_handle = NULL;
        }
    }

    uint16_t add(uint8_t pin, uint8_t invert, uint32_t dbTime){
        btn_unit* btn_t = new btn_unit(pin, invert, dbTime);
        btn_t->_index = buttons.size();
        buttons.push_back(btn_t);
        return btn_t->_index;
    }
    void updateAll(){for (auto &btn : buttons){ btn->update();}}
    uint8_t update(uint16_t index){ return buttons.at(index)->update();}
    uint16_t size(){ return buttons.size();}
    btn_unit& operator[](uint16_t index){ return *buttons.at(index);}
    const btn_unit& operator[](uint16_t index) const
    {
        return *buttons.at(index);
    }
    private:
        static void _task(void *arg){
            ryzobee_btn *self = (ryzobee_btn*)arg;
            while (1){
                self->updateAll();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }
    TaskHandle_t _task_handle;
    std::vector<btn_unit*> buttons;
};