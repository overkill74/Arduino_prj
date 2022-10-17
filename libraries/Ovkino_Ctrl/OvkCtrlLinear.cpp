
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
  return m_cfg;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::setCfg(const Config& new_cfg)
{
  m_cfg.copy(new_cfg);
  DBG_PRINT("coeff_m : "); DBG_PRINTLN(m_cfg.coeff_m );
  DBG_PRINT("coeff_q : "); DBG_PRINTLN(m_cfg.coeff_q );
  DBG_PRINT("setp_min: "); DBG_PRINTLN(m_cfg.setp_min);
  DBG_PRINT("setp_max: "); DBG_PRINTLN(m_cfg.setp_max);
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
  if (isnan(new_val)) { new_val = 25.0f; }

  DBG_PRINT("New: "); DBG_PRINTLN(new_val);
  DBG_PRINT("Max: "); DBG_PRINTLN(m_cfg.setp_min);
  DBG_PRINT("Min: "); DBG_PRINTLN(m_cfg.setp_max);
  
  if (new_val < m_cfg.setp_min) { new_val = m_cfg.setp_min; }
  if (new_val > m_cfg.setp_max) { new_val = m_cfg.setp_max; }
  m_setpoint = new_val;
  DBG_PRINT("SP : "); DBG_PRINTLN(m_setpoint);
  return m_setpoint;
}
//-------------------------------------------------------------------
void OvkCtrlLinear::doWork(float new_temp)
{
//  startPwm();

  float delta = m_setpoint - new_temp;
  DBG_PRINT("new_temp= ");   DBG_PRINTLN(new_temp);
  DBG_PRINT("Delta   = ");   DBG_PRINTLN(delta);

  int pwm = (int)(delta * m_cfg.coeff_m + m_cfg.coeff_q);
  if (pwm >= 100) {
    pwm = 100;
  }
  if (pwm <= 0) {
    pwm = 0;
  }
  m_pwm_level = pwm;
  DBG_PRINT("PWM     = ");   DBG_PRINTLN(m_pwm_level);
}
//-------------------------------------------------------------------
void OvkCtrlLinear::doTick()
{
  if (!m_is_running) {
    digitalWrite(m_out_pin, LOW);
    return;
  }

  if (++m_pwm_cnt > 100) {
    m_pwm_cnt = 0;
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
