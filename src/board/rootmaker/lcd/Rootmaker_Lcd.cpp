#include "Rootmaker_Lcd.h"

#include "../rootmaker_pin.h"
#include "Touch_CST816T.hpp"

Rootmaker_Lcd::Rootmaker_Lcd(TwoWire& i2c, SemaphoreHandle_t mutex) 
    : _dev_i2c(&i2c), _i2c_mutex(mutex) {
    configure();
}

void Rootmaker_Lcd::configure(void) {
    {
        auto cfg = _bus_instance.config();    // Get the structure for bus configuration

        // SPI bus configuration
        cfg.spi_mode = 0;                  // Set SPI communication mode (0~3)
        cfg.freq_write = LCD_WRITE_FREQ;   // SPI clock for transmission (max 80MHz, rounded to integer divisor of 80MHz)
        cfg.freq_read  = LCD_READ_FREQ;    // SPI clock for reception
        cfg.spi_3wire  = true;             // Set to true if using MOSI pin for reception
        cfg.use_lock   = true;             // Set to true if using transaction lock
        cfg.dma_channel = SPI_DMA_CH_AUTO; // Set DMA channel to use (0=DMA disabled/1=1ch/2=2ch/SPI_DMA_CH_AUTO=auto)
        // With ESP-IDF version upgrade, SPI_DMA_CH_AUTO (auto) is recommended for DMA channel. Specifying 1ch or 2ch is no longer recommended
        cfg.pin_sclk = LCD_SCK;            // Set SPI SCLK pin number
        cfg.pin_mosi = LCD_SDA;            // Set SPI MOSI pin number
        cfg.pin_miso = -1;                 // Set SPI MISO pin number (-1=disabled)
        cfg.pin_dc   = LCD_RS;             // Set SPI D/C pin number (-1=disabled)

        _bus_instance.config(cfg);             // Apply settings to the bus
        _panel_instance.setBus(&_bus_instance);// Set the bus to the panel
    }

    { // Configure display panel control settings
        auto cfg = _panel_instance.config();    // Get the structure for display panel configuration

        cfg.pin_cs           =    LCD_CS;   // Pin number connected to CS (-1=disabled)
        cfg.pin_rst          =    LCD_RST;  // Pin number connected to RST (-1=disabled)
        cfg.pin_busy         =    -1;       // Pin number connected to BUSY (-1=disabled)

        cfg.panel_width      =   SCREEN_WIDTH;  // Actual displayable width
        cfg.panel_height     =   SCREEN_WIDTH;  // Actual displayable height
        cfg.offset_x         =     0;  // Panel X direction offset
        cfg.offset_y         =     0;  // Panel Y direction offset
        cfg.offset_rotation  =     0;  // Rotation offset value 0~7 (4~7 for vertical flip)
        cfg.dummy_read_pixel =     8;  // Dummy read bits before pixel read
        cfg.dummy_read_bits  =     1;  // Dummy read bits before non-pixel data read
        cfg.readable         =  true;  // Set to true if data can be read
        cfg.invert           =  true;  // Set to true if panel brightness is inverted
        cfg.rgb_order        = false;  // Set to true if panel red/blue colors are swapped
        cfg.dlen_16bit       = false;  // Set to true for 16-bit parallel or SPI panels sending data in 16-bit units
        cfg.bus_shared       =  true;  // Set to true if bus is shared with SD card (bus control is performed in functions like drawJpgFile)

        _panel_instance.config(cfg);
    }

    { // Configure backlight control settings (remove if not needed)
        auto cfg = _light_instance.config();    // Get the structure for backlight configuration

        cfg.pin_bl = LCD_BL;                    // Pin number connected to backlight
        cfg.invert = false;                     // Set to true if backlight brightness needs to be inverted
        cfg.freq   = LCD_BL_FREQ;               // PWM frequency for backlight
        cfg.pwm_channel = LCD_BL_CHANNEL;       // PWM channel number to use

        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance);  // Set the backlight to the panel
    }

    { // Configure touch screen control settings (remove if not needed)
        auto cfg = _touch_instance.config();

        cfg.x_min      = 0;               // Minimum X value from touch screen (raw value)
        cfg.x_max      = SCREEN_WIDTH;    // Maximum X value from touch screen (raw value)
        cfg.y_min      = 0;               // Minimum Y value from touch screen (raw value)
        cfg.y_max      = SCREEN_WIDTH;    // Maximum Y value from touch screen (raw value)
        cfg.pin_int    = TP_INT;          // Pin number connected to INT
        cfg.pin_rst    = TP_RST;          // Pin number connected to RST
        cfg.bus_shared = true;            // Set to true if bus is shared with screen
        cfg.offset_rotation = 0;          // Adjustment value when display and touch orientation differ, set 0~7
    
    // I2C connection settings
        cfg.i2c_port = 0;         // Select I2C to use (0 or 1)
        cfg.i2c_addr = TP_ADDR;   // I2C device address
        cfg.pin_sda  = TP_SDA;    // Pin number connected to SDA
        cfg.pin_scl  = TP_SCL;    // Pin number connected to SCL
        cfg.freq     = TP_FREQ;   // Set I2C clock frequency
    
        _touch_instance.config(cfg);
        
        _touch_instance.setWire(_dev_i2c);
        
        _panel_instance.setTouch(&_touch_instance);  // Set the touch screen to the panel
    }
    
    setPanel(&_panel_instance); // Set the panel to use
}

void Rootmaker_Lcd::init(void) {
    lgfx::LGFX_Device::init();
}

bool Rootmaker_Lcd::getTouch(uint16_t* x, uint16_t* y) {
    bool touched = false;
    if (xSemaphoreTake(_i2c_mutex, portMAX_DELAY) == pdTRUE) {
        lgfx::touch_point_t tp;
        int count = lgfx::LGFX_Device::getTouch(&tp, 1);
        if (count > 0) {
            *x = tp.x;
            *y = tp.y;
            touched = true;
        }
        xSemaphoreGive(_i2c_mutex);
    }
    return touched;
}
