#include "Arduino.h"

class Adafruit_SSD1306;


class SyscoDisplay
{
public:
  SyscoDisplay(uint8_t iic_addr=0x3C, uint8_t w=128, uint8_t h=64);

public:
  bool initDisplay();
  bool drawIp(const uint32_t& ip, const IPAddress& x);

protected:
  Adafruit_SSD1306*   m_display;  //! The display

protected:
  bool    m_is_init=false;
  uint8_t m_iic_addr;
};