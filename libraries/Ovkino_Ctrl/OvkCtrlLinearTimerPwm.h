///
/// \brief Linear control object
/// Output is PWM on Timer1
/// 

#ifndef OVK_CTRL_LINEAR_PWM_H
#define OVK_CTRL_LINEAR_PWM_H


#include "OvkCtrlLinear.h"

class OvkCtrlLinearTimerPwm : public OvkCtrlLinear
{
public:
  OvkCtrlLinearTimerPwm(float frequency, const int out_pin=LED_BUILTIN);

public:
  void applyOutput() override;
  void doTick() override;
};

#endif
