#pragma once

#include <LovyanGFX.hpp>

enum Ryzobee_board_t {
    RYZOBEE_ROOTMAKER, // RootMaker
};

typedef struct {
/************************************
*            BASE PIN
************************************/
    int8_t reset;

/************************************
*              LCD
************************************/
    struct {
        int8_t bl;
        struct {
            int8_t sda, scl;
        } i2c;
        struct {
            int8_t clk, mosi, miso, cs, dc, rst;
        } spi;
        struct {
            int8_t clk, cs, d1, d2, d3, d4;
        } qspi;
        struct {
            int8_t cs, rs, wr, rd, d0, d1, d2, d3, d4, d5, d6, d7;
        } mcu;
        struct {
            int8_t pclk, vsvync, hsync, henable, de, d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15;
        } rgb565;
    } lcd;

/************************************
*            Touch
************************************/
    struct {
        int8_t inte, sda, scl, rst;
    } touch;

/************************************
*           I2S PIN
************************************/
  struct {
    int8_t mclk, sclk, lrck, dout, din;
  } i2s;

/************************************
*           SD CARD
************************************/
  struct {
    int8_t clk, cmd, d0, d1, d2, d3;
  } sd;

/************************************
*           UART-485
************************************/
  struct {
    int8_t rts, rxd, txd;
  } rs485;

/************************************
*            USB
************************************/
  struct {
    int8_t dp, dn;
  } usb;

/************************************
*         EXTERNAL PIN
************************************/
  struct {
    int8_t pin_1, pin_2, pin_3, pin_4, pin_5, pin_6;
  } external;

} Ryzobee_board_pin_t;

class Ryzobee : public lgfx::LGFX_Device {
    Ryzobee_board_t _board;
  
  public:
    Ryzobee(Ryzobee_board_t board);
    Ryzobee_board_pin_t pins;
    bool init_impl(bool use_reset, bool use_clear);
};
  