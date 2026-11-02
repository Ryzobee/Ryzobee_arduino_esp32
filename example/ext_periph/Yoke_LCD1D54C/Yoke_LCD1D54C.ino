#include <ryzobee_yoke.hpp>
#include <yoke_lcd1d54c/yoke_lcd1d54c.hpp>

Yoke_lcd1d54c lcd;

#define CUBE_WIDTH 40

static long cubeX = (YOKE_LCD1D54C_SIZE - CUBE_WIDTH) / 2;
static long cubeY = (YOKE_LCD1D54C_SIZE - CUBE_WIDTH) / 2;

/**
 * @brief Draw the red square and tap prompt.
 */
static void drawScene()
{
    lcd.clear();
    lcd.fillRect(cubeX, cubeY, CUBE_WIDTH, CUBE_WIDTH, TFT_RED);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.drawString("Please click the cube", 60, 220);
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    randomSeed(micros());

    // These GPIOs illustrate RootMaker wiring; adjust them to the actual wiring on other ESP32 boards.
    Lcd1d54cPins pins;
    pins.sclk = 12;
    pins.mosi = 9;
    pins.dc = 3;
    pins.cs = 5;
    pins.rst = 10;
    pins.bl = 7;
    pins.touch_sda = 41;
    pins.touch_scl = 40;
    pins.touch_rst = 2;
    pins.touch_int = 1;

    if (lcd.begin(pins) != YOKE_PORT_ERROR_NONE) {
        Serial.println("LCD is not available");
        return;
    }

    lcd.setBrightness(255);
    lcd.setTextSize((std::max(lcd.width(), lcd.height()) + 255) >> 8);
    drawScene();

    Serial.println("Yoke LCD1D54C touch example");
}

void loop(void)
{
    if (!lcd.isInitialized()) {
        delay(100);
        return;
    }

    uint16_t x = 0;
    uint16_t y = 0;
    if (lcd.getTouch(&x, &y)) {
        if (x >= cubeX && x < cubeX + CUBE_WIDTH && y >= cubeY && y < cubeY + CUBE_WIDTH) {
            cubeX = random(0, YOKE_LCD1D54C_SIZE - CUBE_WIDTH + 1);
            cubeY = random(0, YOKE_LCD1D54C_SIZE - CUBE_WIDTH + 1);
            drawScene();
        } else if (x < YOKE_LCD1D54C_SIZE && y < YOKE_LCD1D54C_SIZE) {
            lcd.fillRect(x, y, 3, 3, TFT_GREEN);
        }
    }

    delay(10);
}
