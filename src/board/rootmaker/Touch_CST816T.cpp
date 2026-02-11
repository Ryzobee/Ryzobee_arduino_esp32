#include "Touch_CST816T.hpp"

#include <LovyanGFX.hpp>
#include <Wire.h>



namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  static constexpr uint8_t CST816S_TOUCH_REG  = 0x01;
  static constexpr uint8_t CST816S_SLEEP_REG  = 0xE5;
  static constexpr uint8_t CST816S_CHIPID_REG = 0xA7;

  static constexpr uint8_t CST816S_SLEEP_IN   = 0x03;

  bool Touch_CST816T::_write_reg(uint8_t reg, uint8_t val)
  {
    return i2c::writeRegister8(_cfg.i2c_port, _cfg.i2c_addr, reg, val, 0, _cfg.freq).has_value();
  }

  bool Touch_CST816T::_write_regs(uint8_t* val, size_t length)
  {
    return i2c::transactionWrite(_cfg.i2c_port, _cfg.i2c_addr, val, length, _cfg.freq).has_value();
  }

  bool Touch_CST816T::_read_reg(uint8_t reg, uint8_t *data, size_t length)
  {
    return lgfx::i2c::transactionWriteRead(_cfg.i2c_port, _cfg.i2c_addr, &reg, 1, data, length, _cfg.freq).has_value();
  }

  bool Touch_CST816T::_check_init(void)
  {
    if (_inited) return true;

    uint8_t tmp[3] = { 0 };
    _inited = _read_reg(CST816S_CHIPID_REG, tmp, 3);
    ESP_LOGI("_inited:%02x", _inited);
    return _inited;
  }

//----------------------------------------------------------------------------
  bool Touch_CST816T::init(void)
  {

    // _check_init();

    _inited = false;

    if (_cfg.pin_rst >= 0)
    {
      lgfx::pinMode(_cfg.pin_rst, pin_mode_t::output);
      lgfx::gpio_lo(_cfg.pin_rst);
      lgfx::delay(10);
      lgfx::gpio_hi(_cfg.pin_rst);
      lgfx::delay(10);
    }

    if (_cfg.pin_int >= 0)
    {
      lgfx::pinMode(_cfg.pin_int, pin_mode_t::input_pullup);
    }
    lgfx::i2c::init(_cfg.i2c_port, _cfg.pin_sda, _cfg.pin_scl).has_value();

    return true;
  }

  void Touch_CST816T::wakeup(void)
  {
    if (!_inited) return;
    if (_cfg.pin_int < 0) return;
    lgfx::gpio_hi(_cfg.pin_int);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::output);
    delay(5);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::input);
  }

  void Touch_CST816T::sleep(void)
  {
    if (!_inited) return;
  }

  size_t Touch_CST816T::_read_data(uint8_t* readdata)
  {
    size_t res = 0;
    if (lgfx::i2c::beginTransaction(_cfg.i2c_port, _cfg.i2c_addr, _cfg.freq, false))
    {
      readdata[0] = 0x02;
      if (lgfx::i2c::writeBytes(_cfg.i2c_port, readdata, 1)
      && lgfx::i2c::restart(_cfg.i2c_port, _cfg.i2c_addr, _cfg.freq, true)
      && lgfx::i2c::readBytes(_cfg.i2c_port, readdata, 1))
      {
        uint_fast8_t points = std::min<uint_fast8_t>(max_touch_points, readdata[0] & 0x0Fu);
        if (points)
        {
          if (lgfx::i2c::readBytes(_cfg.i2c_port, &readdata[1], points * 6 - 2))
          {
            res = points * 6 - 1;
          }
        }
        else
        {
          res = 1;
        }
      }
      lgfx::i2c::endTransaction(_cfg.i2c_port).has_value();
    }
    return res;
  }

  uint_fast8_t Touch_CST816T::getTouchRaw(touch_point_t *tp, uint_fast8_t count)
  {
    if (count > max_touch_points) { count = max_touch_points; }

    uint8_t readdata[8] = {0};
    if (!_read_reg(0x01, readdata, 6))
    {
      return 0;
    }
    
    
    uint8_t points = readdata[1];
    
    // CST816 only supports single-point touch
    if (points == 0 || points > max_touch_points)
    {
      return 0;
    }
    
    uint8_t event_flag = readdata[2] & 0xC0;
    
    // The coordinates may be inaccurate during the Up event, ignored
    if (event_flag == 0x40)
    {
      return 0;
    }
    
    int32_t x = ((readdata[2] & 0x0F) << 8) + readdata[3];
    int32_t y = ((readdata[4] & 0x0F) << 8) + readdata[5];
    
    tp[0].id = 0;
    tp[0].size = 1;
    tp[0].x = x;
    tp[0].y = y;
    
    return 1;
  }

//----------------------------------------------------------------------------
 }
}
