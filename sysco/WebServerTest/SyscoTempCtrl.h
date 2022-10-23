
//--------------------------------------------------------------------------------
#include "Arduino.h"
#include <OvkCtrlLinear.h>

//--------------------------------------------------------------------------------
class SyscoTempCtrl
{
public:
  static SyscoTempCtrl* createInstance(const int pin_r, const int pin_ev);
  static SyscoTempCtrl* getInstance();

public:
  void doWork();
  void doTick();
  float getSetpoint() { return m_setpoint; }
  float getTmeperature() { return m_temperature; }

private:
  SyscoTempCtrl(const int pin_r, const int pin_ev);

protected:
  OvkCtrlLinear* m_ctrl_res=nullptr;
  OvkCtrlLinear* m_ctrl_fan=nullptr;
  const int      m_pin_out_res;  //! Pin uscita resistenza
  const int      m_pin_out_ev;   //! Pin uscita elettrovalvola
  float          m_setpoint;     //! Setpoint
  float          m_temperature=-1000.0f;  //! Actual temperture

private:
  static SyscoTempCtrl* m_instance;
};
