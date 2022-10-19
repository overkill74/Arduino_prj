#include <Arduino.h>
#include "OvkCtrlLinearTimerPwm.h"

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     3

#define USE_TIMER_1     true

#include <TimerInterrupt.h>
#include <TimerInterrupt.hpp>
#include <ISR_Timer.h>
#include <ISR_Timer.hpp>


//-------------------------------------------------------------------
//-------------------------------------------------------------------
static int g_out_pin = LED_BUILTIN;
static int g_pwm_level = 50;
static int g_pwm_cnt = 0;
static int g_out_level = 0;

//-------------------------------------------------------------------
static void TimerHandler1(void)
{
  if (++g_pwm_cnt >= 100) {
    g_pwm_cnt = 0;
  }
  if (g_pwm_cnt < g_pwm_level) {
    if (!g_out_level) {
      digitalWrite(g_out_pin, HIGH);
      g_out_level = 1;
    }
  }
  else {
    if (g_out_level) {
      digitalWrite(g_out_pin, LOW);
      g_out_level = 0;
    }
  }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
OvkCtrlLinearTimerPwm::OvkCtrlLinearTimerPwm(float frequency, const int out_pin)
  : OvkCtrlLinear(out_pin)
{
  ITimer1.init();
  ITimer1.attachInterrupt(frequency, TimerHandler1);
}
//-------------------------------------------------------------------
void OvkCtrlLinearTimerPwm::applyOutput()
{
}
//-------------------------------------------------------------------
void OvkCtrlLinearTimerPwm::doTick()
{
  g_pwm_level = m_pwm_level;
}


