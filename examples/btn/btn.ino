#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);


void setup(void)
{
    Serial.begin(115200);
  
    board.rootmaker.begin(false, false, false, true);
}

void loop(void)
{
    if (board.rootmaker.btn.btn_change()) {
        if (board.rootmaker.btn.getState() == BUTTON_PRESSED) {
            Serial.println("Button pressed");
        } else {
            Serial.println("Button released");
        }
    }
}
