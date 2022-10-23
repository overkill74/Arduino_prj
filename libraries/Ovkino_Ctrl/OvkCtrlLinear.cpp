
#include <Arduino.h>
#include "OvkCtrlLinear.h"

//#define DEBUG_ME

#ifdef DEBUG_ME
#define DBG_PRINT(...) Serial.print(__VA_ARGS__)
#define DBG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DBG_PRINT(...)
#define DBG_PRINTLN(...)
#endif

//-------------------------------------------------------------------
OvkCtrlLinear::OvkCtrlLinear(const int out_pin, float setpoint)
  : m_out_pin(out_pin)
  , m_setpoint(setpoint)

{
  pinMode(out_pin, OUTPUT);
  DBG_PRINTLN("OvkCtrlLinear Init!");
}
//-------------------------------------------------------------------
OvkCtrlLinear::Config& OvkCtrlLinear::getCfg()
{
  DBG_PRINT("A = "); DBG_PRINT(m_cfg.d_a); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_a);
  DBG_PRINT("B = "); DBG_PRINT(m_cfg.d_b); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_b);

  return m_cfg;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::setCfg(const Config& new_cfg)
{
  m_cfg.copy(new_cfg);
  m_cm = (m_cfg.p_b - m_cfg.p_a) / (m_cfg.d_b - m_cfg.d_a);
  m_cq = m_cfg.p_a - m_cm * m_cfg.d_a;
  DBG_PRINT("A = "); DBG_PRINT(m_cfg.d_a); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_a);
  DBG_PRINT("B = "); DBG_PRINT(m_cfg.d_b); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_b);
  DBG_PRINT("coeff_m : "); DBG_PRINTLN(m_cm);
  DBG_PRINT("coeff_q : "); DBG_PRINTLN(m_cq);
}
//-------------------------------------------------------------------
int OvkCtrlLinear::getPwm()
{
  if (!m_is_running) {
    return 0;
  }
  return m_pwm_level;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::startPwm()
{
  if (m_is_running) {
    return;
  }
  m_pwm_cnt = 0;
  m_is_running = true;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::stopPwm()
{
  m_is_running = false;
  m_pwm_cnt = 0;
  digitalWrite(m_out_pin, LOW);
}
//-------------------------------------------------------------------
float OvkCtrlLinear::setSetpoint(float new_val)
{
  if (!isnan(new_val)) {
    m_setpoint = new_val;
  }
  DBG_PRINT("SP : "); DBG_PRINTLN(m_setpoint);

  return m_setpoint;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::doWork(float new_temp)
{
//  startPwm();

  float delta = new_temp - m_setpoint;
  DBG_PRINTLN("");
  DBG_PRINT("new_temp= ");   DBG_PRINT(new_temp);
  DBG_PRINT("\tsetpoint= ");   DBG_PRINT(m_setpoint);
  DBG_PRINT("\tDelta   = ");   DBG_PRINT(delta);

  if (delta <= m_cfg.d_a) { m_pwm_level = m_cfg.p_a; }
  else if (delta >= m_cfg.d_b) { m_pwm_level = m_cfg.p_b; }
  else {
    m_pwm_level = delta * m_cm + m_cq;
  }
  
  DBG_PRINT("\tPWM     = ");   DBG_PRINTLN(m_pwm_level);
  DBG_PRINT("A = "); DBG_PRINT(m_cfg.d_a); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_a);
  DBG_PRINT("B = "); DBG_PRINT(m_cfg.d_b); DBG_PRINT(", "); DBG_PRINTLN(m_cfg.p_b);
  DBG_PRINT("coeff_m : "); DBG_PRINTLN(m_cm);
  DBG_PRINT("coeff_q : "); DBG_PRINTLN(m_cq);
}
//-------------------------------------------------------------------
void OvkCtrlLinear::doTick()
{
  if (++m_pwm_cnt > 100) {
    m_pwm_cnt = 0;
  }
  applyOutput();
}
//-------------------------------------------------------------------
void OvkCtrlLinear::applyOutput()
{
  if (!m_is_running) {
    digitalWrite(m_out_pin, LOW);
    return;
  }

  if (m_pwm_level >= 98) {
    digitalWrite(m_out_pin, HIGH);
  }
  else if (m_pwm_level <= 2) {
    digitalWrite(m_out_pin, LOW);
  }
  else {
    digitalWrite(m_out_pin, (m_pwm_cnt < m_pwm_level));
  }
}

