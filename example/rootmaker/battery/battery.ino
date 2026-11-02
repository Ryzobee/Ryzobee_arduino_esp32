#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

static uint32_t lastReportAt = 0;
static bool ledEnabled = true;
static bool lastButtonPressed = false;
static uint32_t pressStartedAt = 0;

static const uint32_t REPORT_INTERVAL_MS = 1000;
static const uint32_t LONG_PRESS_MS = 1200;

void updateBatteryLed(uint8_t level, ChargingStatus_t status)
{
    if (!ledEnabled) {
        board.maker.led.setColor(0, 0, 0);
        return;
    }

    if (status == CHARGING_STATUS_CHARGING) {
        board.maker.led.setColor(255, 180, 0);
        return;
    }

    if (status == CHARGING_STATUS_COMPLETE) {
        board.maker.led.setColor(0, 255, 64);
        return;
    }

    uint8_t red = 255 - ((uint16_t)level * 255 / 100);
    uint8_t green = (uint16_t)level * 255 / 100;
    board.maker.led.setColor(red, green, 0);
}

const char* chargingStatusText(ChargingStatus_t status)
{
    switch (status) {
        case CHARGING_STATUS_NOT_CHARGING:
            return "not charging";
        case CHARGING_STATUS_CHARGING:
            return "charging";
        case CHARGING_STATUS_COMPLETE:
            return "charge complete";
        default:
            return "unknown";
    }
}

void printBatteryReport()
{
    uint32_t voltage = board.maker.ec.getVoltage();
    uint8_t level = board.maker.ec.getBatteryLevel();
    ChargingStatus_t status = board.maker.ec.getChargingStatus();

    Serial.printf("Battery: %lumV, %u%%, %s\n", voltage, level, chargingStatusText(status));
    Serial.printf("Wake time: %ums, shutdown time: %ums\n",
                  board.maker.ec.getKeyWakeupTime(),
                  board.maker.ec.getKeyShutdownTime());

    updateBatteryLed(level, status);
}

void handleButton()
{
    board.maker.btns.update(0);
    btn_unit& button = board.maker.btns[0];
    bool pressed = button.isPressed() != 0;
    uint32_t now = millis();

    if (pressed && !lastButtonPressed) {
        pressStartedAt = now;
    }

    if (!pressed && lastButtonPressed) {
        uint32_t pressDuration = now - pressStartedAt;
        if (pressDuration >= LONG_PRESS_MS) {
            ledEnabled = true;
            Serial.println("Long press: LED indication restored.");
        } else {
            ledEnabled = !ledEnabled;
            Serial.printf("Short press: LED indication %s.\n", ledEnabled ? "enabled" : "disabled");
        }

        printBatteryReport();
    }

    lastButtonPressed = pressed;
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(true, false, true, true);

    Serial.println();
    Serial.println("RootMaker battery monitor");
    Serial.printf("EC firmware version: 0x%04X\n", board.maker.ec.getVersion());
    Serial.println("Short press the button to toggle the LED indicator.");
    Serial.println("Long press the button to re-enable the LED indicator.");

    printBatteryReport();
}

void loop(void)
{
    handleButton();

    uint32_t now = millis();
    if (now - lastReportAt >= REPORT_INTERVAL_MS) {
        lastReportAt = now;
        printBatteryReport();
    }

    delay(20);
}
