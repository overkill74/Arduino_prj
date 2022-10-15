#include "Display.h"
//#include <cstdio>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


SyscoDisplay::SyscoDisplay(uint8_t iic_addr, uint8_t w, uint8_t h)
  : m_iic_addr(iic_addr)
{
  m_display = new Adafruit_SSD1306(w, h, &Wire);
  initDisplay();
}

bool SyscoDisplay::initDisplay()
{
  if (!m_display) {
    return false;
  }

  if (!m_is_init) {
    m_is_init = m_display->begin(SSD1306_SWITCHCAPVCC, m_iic_addr);
    m_display->display();

    m_display->cp437(true);
    delay(1000);

    m_display->clearDisplay();
    m_display->display();
  }

  return m_is_init;
}

bool SyscoDisplay::drawIp(const uint32_t& ip)
{
  if (!initDisplay()) { return false; }
  m_display->setTextSize(1);      // Normal 1:1 pixel scale
  m_display->setTextColor(SSD1306_WHITE); // Draw white text

  char txt[32];
  const uint8_t* pip = reinterpret_cast<const uint8_t*>(&ip);

  sprintf(txt, "IP: %d.%d.%d.%d", pip[0], pip[1], pip[2], pip[3]);

  m_display->setCursor(0, 0);     // Start at top-left corner
  m_display->write(txt);
  m_display->display();
}

