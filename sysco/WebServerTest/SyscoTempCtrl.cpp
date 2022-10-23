#include "Arduino.h"

//--------------------------------------------------------------------------------
#include "SyscoTempCtrl.h"
#include "SysCfg.h"

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
  m_ctrl_res = new OvkCtrlLinear(m_pin_out_res, 25.0f);
  m_ctrl_fan = new OvkCtrlLinear(m_pin_out_ev, 25.0f);

  // Riscaldatore
  OvkCtrlLinear::Config& cfg_r = m_ctrl_res->getCfg();
  SysCfg* sys = SysCfg::getInstance();
  cfg_r.d_a = sys->getParam_res_d_a();
  cfg_r.p_a = sys->getParam_res_p_a();
  cfg_r.d_b = sys->getParam_res_d_b();
  cfg_r.p_b = sys->getParam_res_p_b();
  m_ctrl_res->setCfg(cfg_r);
  m_ctrl_res->doWork(25.0f);
  m_ctrl_res->startPwm();

  // Raffreddamento
  OvkCtrlLinear::Config& cfg_c = m_ctrl_fan->getCfg();
  cfg_c.d_a = sys->getParam_fan_d_a();
  cfg_c.p_a = sys->getParam_fan_p_a();
  cfg_c.d_b = sys->getParam_fan_d_b();
  cfg_c.p_b = sys->getParam_fan_p_b();
  m_ctrl_fan->setCfg(cfg_c);
  m_ctrl_fan->doWork(25.0f);
  m_ctrl_fan->startPwm();

  //--------------------------------------
  // Local Setpoint
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A3, LOW);
  digitalWrite(A5, HIGH);
}

//--------------------------------------------------------------------------------
void SyscoTempCtrl::doWork()
{
  // Read Setpoint
  int val = analogRead(A4);
  m_setpoint = 15.0f + (15.0f/1020.0f) * (float)val;
  m_ctrl_res->setSetpoint(m_setpoint);
  m_ctrl_fan->setSetpoint(m_setpoint);

  // Read temperature
  val = analogRead(A1);
  float mv = (float)val * (5000.0f / 1024.0f);
  float kelvin_10 = mv - 2731.5f + SysCfg::getInstance()->getParam_t_offset();
  if (m_temperature < -20.0f) {
    m_temperature = kelvin_10 / 10.0f;
  }
  else {
    m_temperature = m_temperature * 0.9f + kelvin_10 * (0.1f / 10.0f);
  }
  m_ctrl_res->doWork(m_temperature);
  m_ctrl_fan->doWork(m_temperature);
}
//--------------------------------------------------------------------------------
void SyscoTempCtrl::doTick()
{
  m_ctrl_res->doTick();
  m_ctrl_fan->doTick();
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
