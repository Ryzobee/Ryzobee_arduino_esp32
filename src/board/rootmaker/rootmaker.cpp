#include <LovyanGFX.hpp>
#include "boards.h"
#include "peripheral/screen/rootmaker_pin.h"
#include "peripheral/touch/Touch_CST816T.hpp"

using namespace lgfx::v1;

#define LCD_WRITE_FREQ 40000000
#define LCD_READ_FREQ  16000000
#define LCD_BL_FREQ    44100
#define LCD_BL_CHANNEL 7

#define SCREEN_WIDTH  240
#define CUBE_WIDTH    40

class RootMaker : public lgfx::LGFX_Device {

    lgfx::Panel_ST7789  _panel_instance;
  
    lgfx::Bus_SPI       _bus_instance;
  
    lgfx::Light_PWM     _light_instance;
  
    lgfx::Touch_CST816T _touch_instance;
  
  public:
    RootMaker(void);
};

RootMaker::RootMaker(void) {
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
      cfg.bus_shared = true;            // Set to true if bus is shared with screen
      cfg.offset_rotation = 0;          // Adjustment value when display and touch orientation differ, set 0~7

// I2C connection settings
      cfg.i2c_port = 0;         // Select I2C to use (0 or 1)
      cfg.i2c_addr = TP_ADDR;   // I2C device address
      cfg.pin_sda  = TP_SDA;    // Pin number connected to SDA
      cfg.pin_scl  = TP_SCL;    // Pin number connected to SCL
      cfg.freq     = TP_FREQ;   // Set I2C clock frequency

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // Set the touch screen to the panel
    }

    setPanel(&_panel_instance); // Set the panel to use
}



Panel_Device* panel_load_from_rootmaker(Ryzobee_board_pin_t* pins) {
  auto panel = new lgfx::Panel_ST7796();

  *pins = {
    .reset = -1,
    .lcd = {
      .bl = LCD_BL,
      .i2c    = {-1,-1},
      .spi    = {
        .clk = LCD_SCK,
        .mosi = LCD_SDA,
        .miso = -1,
        .cs = LCD_CS,
        .dc = LCD_RS,
      },
      .qspi   = {-1,-1,-1,-1,-1,-1},
      .mcu    = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
      .rgb565 = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    },
    .touch = {
      .inte = TP_INT,
      .sda  = TP_SDA,
      .scl  = TP_SCL,
      .rst  = TP_RST,
    },
    .i2s = {-1,-1,-1,-1,-1},
    .sd  = {-1,-1,-1,-1,-1,-1},
    .rs485 = {-1,-1,-1},
    .usb   = {-1,-1},
    .external = {-1,-1,-1,-1,-1,-1},
  };

  {
    auto _bus_instance = new Bus_SPI();
    auto cfg     =  _bus_instance->config();
    cfg.freq_write = LCD_WRITE_FREQ;
    cfg.freq_read  = LCD_READ_FREQ;
    cfg.spi_3wire  = true;
    cfg.use_lock   = true;
    cfg.dma_channel = SPI_DMA_CH_AUTO;

    cfg.pin_sclk =  LCD_SCK;
    cfg.pin_mosi =  LCD_SDA;
    cfg.pin_miso =  -1;
    cfg.pin_dc =    LCD_RS;

    _bus_instance->config(cfg);
    panel->setBus(_bus_instance);
  }

  {
    auto cfg = panel->config();
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
    cfg.rgb_order        =  true;  // Set to true if panel red/blue colors are swapped
    cfg.dlen_16bit       = false;  // Set to true for 16-bit parallel or SPI panels sending data in 16-bit units
    cfg.bus_shared       =  true;

    panel->config(cfg);
  }

  {
    auto blk = new Light_PWM();
    auto cfg = blk->config();
    cfg.pin_bl      = LCD_BL;
    cfg.invert      = false;
    cfg.freq        = LCD_BL_FREQ;
    cfg.pwm_channel = LCD_BL_CHANNEL;

    blk->config(cfg);
    panel->setLight(blk);
  }

  {
    auto _touch_instance = new Touch_CST816T();
    auto cfg             = _touch_instance->config();

    cfg.x_min      = 0;               // Minimum X value from touch screen (raw value)
    cfg.x_max      = SCREEN_WIDTH - 1;// Maximum X value from touch screen (raw value)
    cfg.y_min      = 0;               // Minimum Y value from touch screen (raw value)
    cfg.y_max      = SCREEN_WIDTH - 1;// Maximum Y value from touch screen (raw value)
    cfg.pin_int    = TP_INT;          // Pin number connected to INT
    cfg.bus_shared = true;            // Set to true if bus is shared with screen
    cfg.offset_rotation = 0;          // Adjustment value when display and touch orientation differ, set 0~7

// I2C connection settings
    cfg.i2c_port = 0;         // Select I2C to use (0 or 1)
    cfg.i2c_addr = TP_ADDR;   // I2C device address
    cfg.pin_sda  = TP_SDA;    // Pin number connected to SDA
    cfg.pin_scl  = TP_SCL;    // Pin number connected to SCL
    cfg.pin_rst  = TP_RST;
    cfg.freq     = TP_FREQ;   // Set I2C clock frequency

    _touch_instance->config(cfg);
    panel->setTouch(_touch_instance);
  }

  return panel;
}
