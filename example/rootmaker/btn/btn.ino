#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);


void setup(void)
{
    Serial.begin(115200);
  
    board.rootmaker.begin(false, false, false, true, false);
}

void loop(void)
{
    Button_event_t event = board.rootmaker.btn.update();
    
    switch (event) {
        case BTN_EVENT_SHORT_PRESS: {
            Serial.println("short press");
            break;
        }
        case BTN_EVENT_LONG_PRESS: {
            Serial.println("long press");
            break;
        }
        case BTN_EVENT_DOUBLE_CLICK: {
            Serial.println("double click");
            break;
        }
        case BTN_EVENT_LONG_PRESSING: {
            Serial.println("long pressing");
            break;
        }
        default: {
            break;
        }
    }
}
