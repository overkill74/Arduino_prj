#include "OvkCtrlLinearPwm.h"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
OvkCtrlLinearPwm::OvkCtrlLinearPwm(const int out_pin)
  : OvkCtrlLinear(out_pin)
{
}
//-------------------------------------------------------------------
void OvkCtrlLinearPwm::applyOutput()
{
  int pwm_level = m_pwm_level * 255;
  pwm_level /= 100;
  if (m_old_pwm_level == pwm_level) {
    return;
  }
  m_old_pwm_level = pwm_level;
  /*
  Serial.print("\nPWM :"); Serial.print(m_pwm_level);
  Serial.print("\tout :"); Serial.println(pwm_level);
  Serial.print("Pin :"); Serial.print(m_out_pin);
  */
  analogWrite(m_out_pin, pwm_level);
}
//-------------------------------------------------------------------
void OvkCtrlLinearPwm::doTick()
{
  applyOutput();
}


