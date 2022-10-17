
#include <OvkCtrlLinear.h>

// The controller
static OvkCtrlLinear m_ctrl;

void setup()
{
  Serial.begin(115200);
  Serial.println("TestApp Controllo lineare");

  // Get Configuration
  OvkCtrlLinear::Config cfg = m_ctrl.getCfg();
  // Change some Configuration
  cfg.setp_min = 15.0f;
  cfg.setp_max = 35.0f;
  cfg.coeff_m = 10.0f;
  cfg.coeff_q = 0.0f;
  // Apply
  m_ctrl.setCfg(cfg);
  // Start Control
  m_ctrl.setSetpoint(25.0f);
  m_ctrl.doWork(10.0f);
  m_ctrl.startPwm();
}

void loop()
{
  const uint32_t delay_pwm = 20;
  static uint32_t tout_pwm = 0;

  const uint32_t delay_3sec = 3000;
  static uint32_t tout_3sec = 0;
  
  static float mis=10.0f;
  
  uint32_t now = millis();

  // Update Measure
  if (now > tout_3sec) {
    tout_3sec = now + delay_3sec;
    m_ctrl.doWork(mis);
    mis = mis + 0.5f;
    if (mis > 26.0f) {
      mis - 10.0f;
    }
  }


  // Update_PWM
  if (now > tout_pwm) {
    tout_pwm = now + delay_pwm;
    m_ctrl.doTick();
  }
}
