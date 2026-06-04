#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);

static int8_t lastBatteryLevel = -1;
static const int INFO_X = 12;
static const int INFO_WIDTH = 216;
static const int LINE_HEIGHT = 18;
static const int BAR_X = 12;
static const int BAR_Y = 145;
static const int BAR_WIDTH = 160;
static const int BAR_HEIGHT = 20;
static const int BAR_INNER_X = 13;
static const int BAR_INNER_Y = 146;
static const int BAR_INNER_WIDTH = 158;
static const int BAR_INNER_HEIGHT = 18;
static const int VOLTAGE_Y = 76;
static const int LEVEL_Y = 100;
static const int BLINK_HINT_Y = 124;
static const int STATUS_Y = 174;
static const int KEY_WAKE_Y = 198;
static const int KEY_OFF_Y = 218;
static const int VALUE_GAP = 8;
static const int VALUE_PADDING = 6;

static int g_voltage_value_x = 0;
static int g_voltage_value_width = 0;
static int g_level_value_x = 0;
static int g_level_value_width = 0;
static int g_status_value_x = 0;
static int g_status_value_width = 0;
static int g_key_value_x = 0;
static int g_key_value_width = 0;

// Blinking control variables
static bool isBlinking = false;             // Whether blinking
static uint32_t lastBlinkTime = 0;          // Last blinking time
static bool ledOn = false;                  // LED current state (on/off)
static const uint32_t BLINK_INTERVAL = 100; // Blinking interval 100ms

// 100 color table, corresponding to 0-100% battery
// Using HSV color space from red(0°) to green(120°)
// Low battery is red, medium battery is yellow, high battery is green
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_Color_t;

static const RGB_Color_t batteryColors[101] = {
    {255, 0, 0},      // 0%  - 纯红
    {255, 6, 0},      // 1%
    {255, 12, 0},     // 2%
    {255, 18, 0},     // 3%
    {255, 24, 0},     // 4%
    {255, 31, 0},     // 5%
    {255, 37, 0},     // 6%
    {255, 43, 0},     // 7%
    {255, 49, 0},     // 8%
    {255, 55, 0},     // 9%
    {255, 61, 0},     // 10%
    {255, 67, 0},     // 11%
    {255, 73, 0},     // 12%
    {255, 80, 0},     // 13%
    {255, 86, 0},     // 14%
    {255, 92, 0},     // 15%
    {255, 98, 0},     // 16%
    {255, 104, 0},    // 17%
    {255, 110, 0},    // 18%
    {255, 116, 0},    // 19%
    {255, 122, 0},    // 20%
    {255, 129, 0},    // 21%
    {255, 135, 0},    // 22%
    {255, 141, 0},    // 23%
    {255, 147, 0},    // 24%
    {255, 153, 0},    // 25%
    {255, 159, 0},    // 26%
    {255, 165, 0},    // 27%
    {255, 171, 0},    // 28%
    {255, 178, 0},    // 29%
    {255, 184, 0},    // 30%
    {255, 190, 0},    // 31%
    {255, 196, 0},    // 32%
    {255, 202, 0},    // 33%
    {255, 208, 0},    // 34%
    {255, 214, 0},    // 35%
    {255, 220, 0},    // 36%
    {255, 227, 0},    // 37%
    {255, 233, 0},    // 38%
    {255, 239, 0},    // 39%
    {255, 245, 0},    // 40%
    {255, 251, 0},    // 41%
    {253, 255, 0},    // 42%
    {247, 255, 0},    // 43%
    {241, 255, 0},    // 44%
    {235, 255, 0},    // 45%
    {229, 255, 0},    // 46%
    {222, 255, 0},    // 47%
    {216, 255, 0},    // 48%
    {210, 255, 0},    // 49%
    {204, 255, 0},    // 50% - 黄绿色
    {198, 255, 0},    // 51%
    {192, 255, 0},    // 52%
    {186, 255, 0},    // 53%
    {180, 255, 0},    // 54%
    {173, 255, 0},    // 55%
    {167, 255, 0},    // 56%
    {161, 255, 0},    // 57%
    {155, 255, 0},    // 58%
    {149, 255, 0},    // 59%
    {143, 255, 0},    // 60%
    {137, 255, 0},    // 61%
    {131, 255, 0},    // 62%
    {124, 255, 0},    // 63%
    {118, 255, 0},    // 64%
    {112, 255, 0},    // 65%
    {106, 255, 0},    // 66%
    {100, 255, 0},    // 67%
    {94, 255, 0},     // 68%
    {88, 255, 0},     // 69%
    {82, 255, 0},     // 70%
    {75, 255, 0},     // 71%
    {69, 255, 0},     // 72%
    {63, 255, 0},     // 73%
    {57, 255, 0},     // 74%
    {51, 255, 0},     // 75%
    {45, 255, 0},     // 76%
    {39, 255, 0},     // 77%
    {33, 255, 0},     // 78%
    {26, 255, 0},     // 79%
    {20, 255, 0},     // 80%
    {14, 255, 0},     // 81%
    {8, 255, 0},      // 82%
    {2, 255, 0},      // 83%
    {0, 255, 4},      // 84%
    {0, 255, 10},     // 85%
    {0, 255, 16},     // 86%
    {0, 255, 22},     // 87%
    {0, 255, 28},     // 88%
    {0, 255, 35},     // 89%
    {0, 255, 41},     // 90%
    {0, 255, 47},     // 91%
    {0, 255, 53},     // 92%
    {0, 255, 59},     // 93%
    {0, 255, 65},     // 94%
    {0, 255, 71},     // 95%
    {0, 255, 77},     // 96%
    {0, 255, 84},     // 97%
    {0, 255, 90},     // 98%
    {0, 255, 96},     // 99%
    {0, 255, 102},    // 100% - 青绿色
};

// Set LED color by battery level
void setLedByBatteryLevel(uint8_t level)
{
    // Limit range to 0-100
    if (level > 100) {
        level = 100;
    }
    
    const RGB_Color_t *color = &batteryColors[level];
    board.rootmaker.led.strip.setPixelColor(0, color->r, color->g, color->b);
    board.rootmaker.led.strip.show();
}

void handleBlinking(uint8_t level)
{
    if (!isBlinking) {
        return;
    }
    
    uint32_t currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        lastBlinkTime = currentTime;
        ledOn = !ledOn;
        
        if (ledOn) {
            const RGB_Color_t *color = &batteryColors[level];
            board.rootmaker.led.strip.setPixelColor(0, color->r, color->g, color->b);
        } else {
            board.rootmaker.led.strip.setPixelColor(0, 0, 0, 0);
        }
        board.rootmaker.led.strip.show();
    }
}

void stopBlinking(uint8_t level)
{
    isBlinking = false;
    ledOn = true;
    setLedByBatteryLevel(level);
    Serial.println("Blinking stopped by button press");
}

void startBlinking(void)
{
    isBlinking = true;
    lastBlinkTime = millis();
    ledOn = false;
    Serial.println("Blinking started due to battery level change");
}

void updateLedOnBatteryChange(uint8_t currentLevel)
{
    if (currentLevel > 100) {
        currentLevel = 100;
    }
    
    if (lastBatteryLevel < 0 || lastBatteryLevel != (int8_t)currentLevel) {
        const RGB_Color_t *color = &batteryColors[currentLevel];
        
        if (lastBatteryLevel >= 0) {
            Serial.printf("Battery level changed: %d%% -> %d%%\n", lastBatteryLevel, currentLevel);
            startBlinking();
        } else {
            setLedByBatteryLevel(currentLevel);
        }
        
        lastBatteryLevel = (int8_t)currentLevel;
        
        Serial.printf("LED color for %d%% is RGB(%d, %d, %d)\n", 
                      currentLevel, color->r, color->g, color->b);
    }
}

static void clearInfoLine(int y)
{
    board.rootmaker.lcd->fillRect(INFO_X, y, INFO_WIDTH, LINE_HEIGHT, TFT_BLACK);
}

static void drawInfoLine(int y, const char* text, uint16_t color = TFT_WHITE)
{
    clearInfoLine(y);
    board.rootmaker.lcd->setTextColor(color, TFT_BLACK);
    board.rootmaker.lcd->drawString(text, INFO_X, y);
}

static void drawVoltageValue(const char* text)
{
    board.rootmaker.lcd->fillRect(g_voltage_value_x, VOLTAGE_Y, g_voltage_value_width, LINE_HEIGHT, TFT_BLACK);
    board.rootmaker.lcd->setTextColor(TFT_WHITE, TFT_BLACK);
    board.rootmaker.lcd->drawString(text, g_voltage_value_x, VOLTAGE_Y);
}

static void drawLevelValue(const char* text)
{
    board.rootmaker.lcd->fillRect(g_level_value_x, LEVEL_Y, g_level_value_width, LINE_HEIGHT, TFT_BLACK);
    board.rootmaker.lcd->setTextColor(TFT_WHITE, TFT_BLACK);
    board.rootmaker.lcd->drawString(text, g_level_value_x, LEVEL_Y);
}

static void drawStatusValue(const char* text, uint16_t color)
{
    board.rootmaker.lcd->fillRect(g_status_value_x, STATUS_Y, g_status_value_width, LINE_HEIGHT, TFT_BLACK);
    board.rootmaker.lcd->setTextColor(color, TFT_BLACK);
    board.rootmaker.lcd->drawString(text, g_status_value_x, STATUS_Y);
}

static void drawKeyValue(int y, const char* text)
{
    board.rootmaker.lcd->fillRect(g_key_value_x, y, g_key_value_width, LINE_HEIGHT, TFT_BLACK);
    board.rootmaker.lcd->setTextColor(TFT_WHITE, TFT_BLACK);
    board.rootmaker.lcd->drawString(text, g_key_value_x, y);
}

static int computeValueX(const char* label)
{
    return INFO_X + board.rootmaker.lcd->textWidth(label) + VALUE_GAP;
}

static int computeValueWidth(int value_x, const char* max_text)
{
    int available = board.rootmaker.lcd->width() - value_x - VALUE_PADDING;
    int required = board.rootmaker.lcd->textWidth(max_text) + VALUE_PADDING;
    return (required < available) ? required : available;
}

void setup(void)
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("Battery Monitor Example with 100 LED Colors");
  
    board.rootmaker.begin(true, true, false, true, true);

    board.rootmaker.led.strip.setPixelColor(0, 0, 0, 0);
    board.rootmaker.led.strip.show();

    board.rootmaker.lcd->setBrightness(255);
    board.rootmaker.lcd->setTextSize(2);
    board.rootmaker.lcd->fillScreen(TFT_BLACK);
    board.rootmaker.lcd->setTextColor(TFT_WHITE, TFT_BLACK);

    g_voltage_value_x = computeValueX("Voltage:");
    g_voltage_value_width = computeValueWidth(g_voltage_value_x, "8888 mV");
    g_level_value_x = computeValueX("Level:");
    g_level_value_width = computeValueWidth(g_level_value_x, "100%");
    g_status_value_x = computeValueX("Status:");
    g_status_value_width = computeValueWidth(g_status_value_x, "Charge Complete");
    g_key_value_x = computeValueX("Wake:");
    g_key_value_width = computeValueWidth(g_key_value_x, "88888ms");

    board.rootmaker.lcd->drawRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, TFT_WHITE);
    board.rootmaker.lcd->drawString("Voltage:", INFO_X, VOLTAGE_Y);
    board.rootmaker.lcd->drawString("Level:", INFO_X, LEVEL_Y);
    board.rootmaker.lcd->drawString("Status:", INFO_X, STATUS_Y);
    board.rootmaker.lcd->drawString("Wake:", INFO_X, KEY_WAKE_Y);
    board.rootmaker.lcd->drawString("Off:", INFO_X, KEY_OFF_Y);

    if (board.rootmaker.bat.isConnected()) {
        Serial.println("PY32 battery manager connected!");
        board.rootmaker.lcd->drawString("PY32 Connected!", 20, 10);
        
        uint16_t version = board.rootmaker.bat.getVersion();
        Serial.printf("Firmware Version: v%d.%d\n", version >> 8, version & 0xFF);
        
        char verStr[32];
        snprintf(verStr, sizeof(verStr), "FW: v%d.%d", version >> 8, version & 0xFF);
        board.rootmaker.lcd->drawString(verStr, 20, 40);
    } else {
        Serial.println("PY32 battery manager not found!");
        board.rootmaker.lcd->setTextColor(TFT_RED, TFT_BLACK);
        board.rootmaker.lcd->drawString("PY32 Not Found!", 20, 10);
    }
}

static uint32_t lastDisplayUpdate = 0;
static const uint32_t DISPLAY_UPDATE_INTERVAL = 2000;

void loop(void)
{
    uint8_t currentLevel = lastBatteryLevel >= 0 ? (uint8_t)lastBatteryLevel : 0;
    
    Button_event_t event = board.rootmaker.btn.update();
    if (event == BTN_EVENT_SHORT_PRESS || event == BTN_EVENT_LONG_PRESS || 
        event == BTN_EVENT_DOUBLE_CLICK) {
        if (isBlinking) {
            stopBlinking(currentLevel);
        }
    }
    
    handleBlinking(currentLevel);
    
    if (!board.rootmaker.bat.isConnected()) {
        Serial.println("PY32 disconnected!");
        board.rootmaker.lcd->setTextColor(TFT_RED, TFT_BLACK);
        board.rootmaker.lcd->drawString("Disconnected!   ", INFO_X, VOLTAGE_Y);
        delay(100);
        return;
    }

    uint32_t currentTime = millis();
    if (currentTime - lastDisplayUpdate < DISPLAY_UPDATE_INTERVAL) {
        return;
    }
    lastDisplayUpdate = currentTime;

    uint32_t voltage = board.rootmaker.bat.getVoltage();
    Serial.printf("Battery Voltage: %ld mV\n", voltage);
    
    char voltStr[20];
    snprintf(voltStr, sizeof(voltStr), "%ld mV", voltage);

    // Read remaining battery percentage
    uint8_t level = board.rootmaker.bat.getBatteryLevel();
    Serial.printf("Battery Level: %d%%\n", level);
    
    // Update LED color by battery level (if battery level changes, start blinking)
    updateLedOnBatteryChange(level);
    
    char levelStr[16];
    snprintf(levelStr, sizeof(levelStr), "%d%%", level);
    int barWidth = (int)(level * 1.5);  // Maximum 150 pixels

    // Read charging status
    ChargingStatus_t status = board.rootmaker.bat.getChargingStatus();
    const char* statusStr = "";
    switch (status) {
        case CHARGING_STATUS_NOT_CHARGING: {
            statusStr = "Not Charging";
            Serial.println("Charging Status: Not Charging");
            board.rootmaker.lcd->setTextColor(TFT_YELLOW, TFT_BLACK);
            break;
        }
        case CHARGING_STATUS_CHARGING: {
            statusStr = "Charging...";
            Serial.println("Charging Status: Charging");
            board.rootmaker.lcd->setTextColor(TFT_CYAN, TFT_BLACK);
            break;
        }
        case CHARGING_STATUS_COMPLETE: {
            statusStr = "Charge Complete";
            Serial.println("Charging Status: Complete");
            board.rootmaker.lcd->setTextColor(TFT_GREEN, TFT_BLACK);
            break;
        }
    }

    // Read key duration configuration
    uint16_t wakeup_time = board.rootmaker.bat.getKeyWakeupTime();
    uint16_t shutdown_time = board.rootmaker.bat.getKeyShutdownTime();
    Serial.printf("Key Wakeup Time: %d ms\n", wakeup_time);
    Serial.printf("Key Shutdown Time: %d ms\n", shutdown_time);
    
    char keyStr[48];
    board.rootmaker.lcd->startWrite();

    drawVoltageValue(voltStr);
    drawLevelValue(levelStr);

    if (isBlinking) {
        drawInfoLine(BLINK_HINT_Y, "Press BTN to stop", TFT_MAGENTA);
    } else {
        clearInfoLine(BLINK_HINT_Y);
    }

    board.rootmaker.lcd->fillRect(BAR_INNER_X, BAR_INNER_Y, BAR_INNER_WIDTH, BAR_INNER_HEIGHT, TFT_BLACK);
    if (barWidth > 0) {
        uint16_t barColor = (level > 20) ? TFT_GREEN : TFT_RED;
        board.rootmaker.lcd->fillRect(BAR_INNER_X, BAR_INNER_Y, barWidth, BAR_INNER_HEIGHT, barColor);
    }

    drawStatusValue(statusStr, status == CHARGING_STATUS_NOT_CHARGING ? TFT_YELLOW :
                              status == CHARGING_STATUS_CHARGING ? TFT_CYAN : TFT_GREEN);

    snprintf(keyStr, sizeof(keyStr), "%dms", wakeup_time);
    drawKeyValue(KEY_WAKE_Y, keyStr);
    snprintf(keyStr, sizeof(keyStr), "%dms", shutdown_time);
    drawKeyValue(KEY_OFF_Y, keyStr);

    board.rootmaker.lcd->endWrite();

    Serial.println("--------------------");
}
