#include "Arduino.h"

//--------------------------------------------------------------------------------
#include "SyscoTempCtrl.h"
#include "Display.h"

//OvkCtrlLinear g_ctrl_res(39, 25.0f);
//OvkCtrlLinear g_ctrl_fan(47, 25.0f);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoTempCtrl::SyscoTempCtrl(const int pin_r, const int pin_ev)
  : m_pin_out_res(pin_r)
  , m_pin_out_ev(pin_ev)
{
  // PINS
  pinMode(pin_r, OUTPUT);
  pinMode(pin_ev, OUTPUT);
  digitalWrite(pin_r, LOW);
  digitalWrite(pin_ev, LOW);


  // Controllers
  OvkCtrlLinear* m_ctrl_res = new OvkCtrlLinear(m_pin_out_res, 25.0f);
  OvkCtrlLinear* m_ctrl_fan = new OvkCtrlLinear(m_pin_out_ev, 25.0f);

  // Riscaldatore
  OvkCtrlLinear::Config cfg_r = m_ctrl_res->getCfg();
  cfg_r.d_a = -10.0f;
  cfg_r.d_b = 0.0f;
  cfg_r.p_a = 100.0f;
  cfg_r.p_b = 0.0f;

  // Raffreddamento
  OvkCtrlLinear::Config cfg_c = m_ctrl_fan->getCfg();
  cfg_c.d_a = 0.0f;
  cfg_c.d_b = 5.0f;
  cfg_c.p_a = 0.0f;
  cfg_c.p_b = 100.0f;

  //--------------------------------------
  // Local Setpoint
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A3, LOW);
  digitalWrite(A5, HIGH);
}

//--------------------------------------------------------------------------------
float SyscoTempCtrl::doWork()
{
  // Read Setpoint
  int val = analogRead(A4);
  float setpoint = 15.0f + (15.0f/1024.0f) * val;
  m_ctrl_res->setSetpoint(setpoint);
  m_ctrl_fan->setSetpoint(setpoint);

  // Read temperature
  float temp = 18.7f;
  //if (dysp) { dysp->drawTemperature(temp); }
  m_ctrl_res->doWork(temp);
  m_ctrl_fan->doWork(temp);
  return setpoint;
}
//--------------------------------------------------------------------------------
void SyscoTempCtrl::doTick()
{
  //m_ctrl_res->doTick();
  //m_ctrl_fan->doTick();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoTempCtrl* SyscoTempCtrl::m_instance = nullptr;
//--------------------------------------------------------------------------------
SyscoTempCtrl* SyscoTempCtrl::createInstance(const int pin_r, const int pin_ev)
{
  if (!m_instance) {
    m_instance = new SyscoTempCtrl(pin_r, pin_ev);
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SyscoTempCtrl* SyscoTempCtrl::getInstance()
{
  return m_instance;
}
