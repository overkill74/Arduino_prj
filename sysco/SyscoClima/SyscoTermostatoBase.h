
//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class SyscoTermostatoBase
{
public:
  SyscoTermostatoBase();
  float getSetPoint() { return m_setpoint; }
  float getTemperature() { return m_temperature; }

  virtual bool begin() = 0;
  virtual void doWork() = 0;

protected:
  float   m_setpoint=-1000.0f;
  float   m_temperature=-1000.0f;;
};
