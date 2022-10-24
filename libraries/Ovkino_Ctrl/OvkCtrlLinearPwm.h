///
/// \brief Linear control object
/// Output is PWM on Timer1
/// 

#ifndef OVK_CTRL_LINEAR_OUT_PWM_H
#define OVK_CTRL_LINEAR_OUT_PWM_H


#include "OvkCtrlLinear.h"

class OvkCtrlLinearPwm : public OvkCtrlLinear
{
public:
  OvkCtrlLinearPwm(const int out_pin=LED_BUILTIN);

public:
  void applyOutput() override;
  void doTick() override;
private:
  int m_old_pwm_level = -100;
};

#endif
