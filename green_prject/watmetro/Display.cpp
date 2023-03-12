#include "HardwareSerial.h"
#include "Display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//--------------------------------------------------------------------------------
OvkDisplay* OvkDisplay::m_instance = nullptr;

//--------------------------------------------------------------------------------
OvkDisplay::OvkDisplay(uint8_t iic_addr, uint8_t w, uint8_t h)
  : m_iic_addr(iic_addr)
{
  Serial.println(__LINE__);
  m_display = new Adafruit_SSD1306(w, h, &Wire);
  Serial.println(__LINE__);
  initDisplay();
}
//--------------------------------------------------------------------------------
bool OvkDisplay::initDisplay()
{
  Serial.println(__LINE__);
  if (!m_display) {
    return false;
  }

  Serial.println(__LINE__);
  if (!m_is_init) {
  Serial.println(__LINE__);
    m_is_init = m_display->begin(SSD1306_SWITCHCAPVCC, m_iic_addr);
  Serial.println(__LINE__);
    m_display->display();

  Serial.println(__LINE__);
    m_display->cp437(true);
  Serial.println(__LINE__);
    delay(1000);

  Serial.println(__LINE__);
    m_display->clearDisplay();
  Serial.println(__LINE__);
    m_display->display();
  }

  Serial.println(__LINE__);
  return m_is_init;
}
//--------------------------------------------------------------------------------
void OvkDisplay::updateBegin()
{
  m_display->clearDisplay();
}
//--------------------------------------------------------------------------------
void OvkDisplay::updateEnd()
{
  m_display->display();
}
//--------------------------------------------------------------------------------
bool OvkDisplay::drawStatus(const char* txt)
{
  m_display->setTextSize(1);      // Normal 1:1 pixel scale
  m_display->setTextColor(SSD1306_WHITE); // Draw white text
  m_display->setCursor(0, 0);     // Start at top-left corner
  m_display->write(txt);
}
//--------------------------------------------------------------------------------
bool OvkDisplay::drawInstantWatt(const int val)
{
  m_display->setTextSize(4);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(0, 8);
  m_display->print(val);
}
//--------------------------------------------------------------------------------
bool OvkDisplay::drawInstantWattHour(const int val)
{
  m_display->setTextSize(3);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(0, 8+4*8);
  m_display->print(val);
}
//--------------------------------------------------------------------------------
bool OvkDisplay::drawTime(const uint32_t val)
{
  m_display->setTextSize(2);
  m_display->setTextColor(SSD1306_WHITE);
  m_display->setCursor(90, 8+4*8);
  m_display->print(val);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
OvkDisplay* OvkDisplay::createInstance(uint8_t iic_addr, uint8_t w, uint8_t h)
{
  if (!m_instance) {
    m_instance = new OvkDisplay(iic_addr, w, h);
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
OvkDisplay* OvkDisplay::getInstance()
{
  return m_instance;
}

