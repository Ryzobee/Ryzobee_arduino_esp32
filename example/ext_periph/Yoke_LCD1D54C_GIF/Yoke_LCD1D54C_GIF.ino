#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <AnimatedGIF.h>
#include <ryzobee_yoke.hpp>
#include <yoke_lcd1d54c/yoke_lcd1d54c.hpp>

#include "Ryzo_20fps.h"
#include "Ryzo_25fps.h"

Ryzobee board;
Yoke_lcd1d54c lcd(&board.maker.getWire(), board.maker.getI2CMutex());

AnimatedGIF gif;

#define BUFFER_SIZE 240

static uint16_t lineBuffer[BUFFER_SIZE];
static uint8_t currentGif = 0;
static bool lastPressed = false;

static void drawGifLine(GIFDRAW* draw)
{
    if (!lcd.isInitialized()) {
        return;
    }

    uint16_t* palette = draw->pPalette;
    uint8_t* source = draw->pPixels;
    int width = draw->iWidth;
    int y = draw->iY + draw->y;

    if (y >= lcd.height() || draw->iX >= lcd.width()) {
        return;
    }

    if (draw->iX + width > lcd.width()) {
        width = lcd.width() - draw->iX;
    }
    if (width <= 0) {
        return;
    }

    if (draw->ucDisposalMethod == 2) {
        for (int index = 0; index < width; ++index) {
            if (source[index] == draw->ucTransparent) {
                source[index] = draw->ucBackground;
            }
        }
        draw->ucHasTransparency = 0;
    }

    if (draw->ucHasTransparency) {
        int start = 0;
        while (start < width) {
            while (start < width && source[start] == draw->ucTransparent) {
                ++start;
            }

            int count = 0;
            while (start + count < width && source[start + count] != draw->ucTransparent && count < BUFFER_SIZE) {
                lineBuffer[count] = palette[source[start + count]];
                ++count;
            }

            if (count > 0) {
                lcd.pushImage(draw->iX + start, y, count, 1, lineBuffer);
                start += count;
            }
        }
        return;
    }

    for (int index = 0; index < width; ++index) {
        lineBuffer[index] = palette[source[index]];
    }

    lcd.pushImage(draw->iX, y, width, 1, lineBuffer);
}

static bool openCurrentGif()
{
    const uint8_t* gifData = currentGif == 0 ? Ryzo_20fps : Ryzo_25fps;
    uint32_t gifSize = currentGif == 0 ? sizeof(Ryzo_20fps) : sizeof(Ryzo_25fps);

    if (lcd.isInitialized()) {
        lcd.fillScreen(TFT_BLACK);
    }

    if (!gif.open((uint8_t*)gifData, gifSize, drawGifLine)) {
        Serial.printf("Failed to open GIF %u\n", currentGif);
        return false;
    }

    Serial.printf("Playing GIF %u: %d x %d\n", currentGif, gif.getCanvasWidth(), gif.getCanvasHeight());
    return true;
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(false, false, true, false);

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
    lcd.fillScreen(TFT_BLACK);
    gif.begin(BIG_ENDIAN_PIXELS);
    openCurrentGif();

    Serial.println();
    Serial.println("RootMaker GIF example");
    Serial.println("Release the onboard button to switch to the next animation.");
}

void loop(void)
{
    if (!lcd.isInitialized()) {
        delay(100);
        return;
    }

    bool pressed = board.maker.btns[0].isPressed() != 0;

    if (!pressed && lastPressed) {
        gif.close();
        currentGif = (currentGif + 1) % 2;
        openCurrentGif();
    }

    lcd.startWrite();
    bool hasFrame = gif.playFrame(true, nullptr);
    lcd.endWrite();

    if (!hasFrame) {
        gif.reset();
    }

    lastPressed = pressed;
    delay(1);
}

