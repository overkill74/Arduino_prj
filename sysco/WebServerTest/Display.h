
//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class Adafruit_SSD1306;

//--------------------------------------------------------------------------------
class SyscoDisplay
{
public:
  static SyscoDisplay* createInstance(uint8_t iic_addr=0x3C, uint8_t w=128, uint8_t h=64);
  static SyscoDisplay* getInstance();

public:
  bool initDisplay();
  bool drawIp(const uint32_t& ip);
  bool drawSetPoint(const float val);
  bool drawTemperature(const float val);

private:
  SyscoDisplay(uint8_t iic_addr, uint8_t w, uint8_t h);
  const char* to_celsius(float deg_c);

protected:
  Adafruit_SSD1306*   m_display;  //! The display
  char                m_txt[32];  //! Tmp buffer

protected:
  bool    m_is_init=false;
  uint8_t m_iic_addr;

private:
  static SyscoDisplay* m_instance;
};