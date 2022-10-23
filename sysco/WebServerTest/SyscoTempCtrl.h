
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
  float doWork();
  void doTick();

private:
  SyscoTempCtrl(const int pin_r, const int pin_ev);

protected:
  OvkCtrlLinear* m_ctrl_res=nullptr;
  OvkCtrlLinear* m_ctrl_fan=nullptr;
  const int      m_pin_out_res;  //! Pin uscita resistenza
  const int      m_pin_out_ev;   //! Pin uscita elettrovalvola

private:
  static SyscoTempCtrl* m_instance;
};
