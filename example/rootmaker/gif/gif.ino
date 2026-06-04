#include "Ryzobee.h"
#include <AnimatedGIF.h>

#include "Ryzo_20fps.h"
#include "Ryzo_25fps.h"

Ryzobee board(RYZOBEE_ROOTMAKER);

AnimatedGIF gif;

// Buffer for pixel rendering (256 pixels wide should be enough for 240px screen)
#define BUFFER_SIZE 256
uint16_t usTemp[BUFFER_SIZE];

uint8_t currentGif = 0;
bool switchToNextGif = false;

void GIFDraw(GIFDRAW *pDraw);

void setup(void)
{
    Serial.begin(115200);
    Serial.println("GIF Animation Demo Starting...");

    // Parameters: LCDEnable, LEDEnable, SensorEnable, ButtonEnable, BatteryEnable
    board.rootmaker.begin(true, false, false, true, false);
    board.rootmaker.lcd->setBrightness(255);
    board.rootmaker.lcd->fillScreen(TFT_BLACK);
    
    gif.begin(BIG_ENDIAN_PIXELS);
    
    Serial.println("Setup complete. Starting GIF playback...");
    Serial.println("Press button to switch to next GIF.");
}

void loop(void)
{
    // Select which GIF to play
    const uint8_t *gifData;
    uint32_t gifSize;
    
    if (currentGif == 0) {
        gifData = Ryzo_20fps;
        gifSize = sizeof(Ryzo_20fps);
        Serial.println("Playing Ryzo_20fps...");
    } else {
        gifData = Ryzo_25fps;
        gifSize = sizeof(Ryzo_25fps);
        Serial.println("Playing Ryzo_25fps...");
    }
    
    // Reset switch flag
    switchToNextGif = false;
    
    if (gif.open((uint8_t *)gifData, gifSize, GIFDraw)) {
        Serial.printf("GIF opened successfully. Canvas size: %d x %d\n", 
                      gif.getCanvasWidth(), gif.getCanvasHeight());
        
        while (!switchToNextGif) {
            board.rootmaker.lcd->startWrite();
            while (gif.playFrame(true, NULL)) {
                Button_event_t event = board.rootmaker.btn.update();
                if (event == BTN_EVENT_SHORT_PRESS) {
                    Serial.println("Button pressed! Switching to next GIF...");
                    switchToNextGif = true;
                    break;
                }
                yield(); // Allow background tasks to run
            }
            board.rootmaker.lcd->endWrite();
            
            if (!switchToNextGif) {
                gif.reset();
            }
        }
        
        // Close GIF after playback
        gif.close();
        
    } else {
        Serial.println("Failed to open GIF!");
        delay(1000);
    }
    
    // Switch to the next GIF
    currentGif = (currentGif + 1) % 2;

    board.rootmaker.lcd->fillScreen(TFT_BLACK);
}

/**
 * @brief Helper function to draw pixels to LCD
 * 
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 * @param pixels Pointer to RGB565 pixel buffer
 */
void drawPixels(int x, int y, int w, int h, uint16_t *pixels)
{
    board.rootmaker.lcd->pushImage(x, y, w, h, pixels);
}

/**
 * @brief GIF draw callback - called for each scan line
 * @param pDraw Pointer to GIFDRAW structure containing line data
 */
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette;
    int x, y, iWidth;
    
    // Get display dimensions
    int displayWidth = board.rootmaker.lcd->width();
    int displayHeight = board.rootmaker.lcd->height();
    
    // Calculate visible width (clip to display bounds)
    iWidth = pDraw->iWidth;
    if (iWidth + pDraw->iX > displayWidth) {
        iWidth = displayWidth - pDraw->iX;
    }
    
    // Get palette
    usPalette = pDraw->pPalette;
    
    // Calculate current Y position
    y = pDraw->iY + pDraw->y;
    
    // Bounds check
    if (y >= displayHeight || pDraw->iX >= displayWidth || iWidth < 1) {
        return;
    }
    
    // Get source pixel pointer
    s = pDraw->pPixels;
    
    // Handle disposal method 2: restore to background color
    if (pDraw->ucDisposalMethod == 2) {
        for (x = 0; x < iWidth; x++) {
            if (s[x] == pDraw->ucTransparent) {
                s[x] = pDraw->ucBackground;
            }
        }
        pDraw->ucHasTransparency = 0;
    }
    
    // Draw the line with or without transparency
    if (pDraw->ucHasTransparency) {
        // Handle transparent pixels
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        int iCount;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0;
        
        while (x < iWidth) {
            c = ucTransparent - 1;
            d = &usTemp[0];
            
            // Collect opaque pixels
            while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE) {
                c = *s++;
                if (c == ucTransparent) {
                    s--; // Back up to treat as transparent
                } else {
                    *d++ = usPalette[c];
                    iCount++;
                }
            }
            
            // Draw collected opaque pixels
            if (iCount) {
                drawPixels(pDraw->iX + x, y, iCount, 1, usTemp);
                x += iCount;
                iCount = 0;
            }
            
            // Skip transparent pixels
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd) {
                c = *s++;
                if (c == ucTransparent) {
                    x++;
                } else {
                    s--;
                }
            }
        }
    } else {
        // No transparency - draw entire line
        s = pDraw->pPixels;
        
        // Convert palette-indexed pixels to RGB565
        for (x = 0; x < iWidth; x++) {
            usTemp[x] = usPalette[*s++];
        }
        
        // Draw the line
        drawPixels(pDraw->iX, y, iWidth, 1, usTemp);
    }
}

