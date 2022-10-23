#include "HardwareSerial.h"
#include <avr/pgmspace.h>
#include "Display.h"
//#include <cstdio>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//--------------------------------------------------------------------------------
SyscoDisplay* SyscoDisplay::m_instance = nullptr;

//--------------------------------------------------------------------------------
SyscoDisplay::SyscoDisplay(uint8_t iic_addr, uint8_t w, uint8_t h)
  : m_iic_addr(iic_addr)
{
  m_display = new Adafruit_SSD1306(w, h, &Wire);
  initDisplay();
}
//--------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------
void SyscoDisplay::updateBegin()
{
  m_display->clearDisplay();
}
//--------------------------------------------------------------------------------
void SyscoDisplay::updateEnd()
{
  m_display->display();
}
//--------------------------------------------------------------------------------
bool SyscoDisplay::drawIp(const uint32_t& ip)
{
  m_display->setTextSize(1);      // Normal 1:1 pixel scale
  m_display->setTextColor(SSD1306_WHITE); // Draw white text

  char txt[32];
  const uint8_t* pip = reinterpret_cast<const uint8_t*>(&ip);

  sprintf(txt, "IP: %d.%d.%d.%d", pip[0], pip[1], pip[2], pip[3]);

  m_display->setCursor(0, 0);     // Start at top-left corner
  m_display->write(txt);
  m_display->display();
}
//--------------------------------------------------------------------------------
bool SyscoDisplay::drawSetPoint(const float val)
{
  m_display->setTextSize(3);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(0, 10);
  m_display->write(to_celsius(val));
}
//--------------------------------------------------------------------------------
bool SyscoDisplay::drawTemperature(const float val)
{
  m_display->setTextSize(3);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(0, 10+3*8);
  m_display->setCursor(0, 10+3*8);
  m_display->write(to_celsius(val));
}
//--------------------------------------------------------------------------------
bool SyscoDisplay::drawStatus(int line, const char* txt)
{
  if (!initDisplay()) { return false; }
  m_display->setTextSize(2);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(0, 8*line);
  m_display->write(txt);
  m_display->display();
}
//--------------------------------------------------------------------------------
const char* SyscoDisplay::to_celsius(float deg_c)
{
  int ideg = (int)(deg_c*10.0f);
  if (ideg < -99) {
    strcpy(m_txt, "--.-");
    return m_txt;
  }
  if (ideg > 999) {
    strcpy(m_txt, "++.+");
    return m_txt;
  }

  sprintf(m_txt, "%d", ideg);

  int anticrash = 0;
  while (strlen(m_txt) < 3) {
    if (++anticrash > 3) { break; }
    int len = strlen(m_txt);
    if (!len) {
      m_txt[0] = '0';
      len = 1;
    }
    m_txt[len+1] = '\0';
    for (int x=len; x>0; --x) {
      m_txt[x] = m_txt[x-1];
    }
    m_txt[0] = '0';
  }
  int dpos = strlen(m_txt) - 1;
  m_txt[dpos+2] = '\0';
  m_txt[dpos+1] = m_txt[dpos];
  m_txt[dpos] = '.';
  if (m_txt[0] == '0') { m_txt[0] = ' '; }

  return m_txt;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoDisplay* SyscoDisplay::createInstance(uint8_t iic_addr, uint8_t w, uint8_t h)
{
  if (!m_instance) {
    m_instance = new SyscoDisplay(iic_addr, w, h);
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SyscoDisplay* SyscoDisplay::getInstance()
{
  return m_instance;
}

