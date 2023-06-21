
#include "twat.h"

#include <OneWire.h>
#include <DallasTemperature.h>

//#define DEBUG_ME


TWater::TWater(const int pin_dati)
  : m_pin_dati(pin_dati)
{
  m_oneWire = new OneWire(pin_dati);
  m_sensors = new DallasTemperature(m_oneWire);
}

void TWater::init_sensor()
{
  m_sensors->begin();
}

float TWater::readTemperature()
{
  m_sensors->requestTemperatures();
  float tmp = m_sensors->getTempCByIndex(0);

  if (tmp < -120) {
    Serial.println(" Init sensor!");
    init_sensor();
  }
  else {
    m_old_temp = tmp;
  }

#if DEBUG_ME == 1
  Serial.print("Temperatura: ");
  Serial.print(m_old_temp);
  Serial.println(" °C");
#endif

  return m_old_temp;
}