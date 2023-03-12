//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class Adafruit_SSD1306;

//--------------------------------------------------------------------------------
class OvkDisplay
{
public:
  static OvkDisplay* createInstance(uint8_t iic_addr=0x3C, uint8_t w=128, uint8_t h=64);
  static OvkDisplay* getInstance();

public:
  bool initDisplay();
  void updateBegin();
  void updateEnd();
  bool drawStatus(const char* txt);
  bool drawInstantWatt(const int val);
  bool drawInstantWattHour(const int val);
  bool drawTime(const uint32_t val);

private:
  OvkDisplay(uint8_t iic_addr, uint8_t w, uint8_t h);

protected:
  Adafruit_SSD1306*   m_display;  //! The display
  char                m_txt[8];  //! Tmp buffer

protected:
  bool    m_is_init=false;
  uint8_t m_iic_addr;

private:
  static OvkDisplay* m_instance;
};
