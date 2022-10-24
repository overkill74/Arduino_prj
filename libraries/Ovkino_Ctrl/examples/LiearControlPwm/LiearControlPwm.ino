
#include <OvkCtrlLinearPwm.h>

// The controller
static OvkCtrlLinearPwm m_ctrl(10);


void setup()
{
  Serial.begin(115200);
  Serial.println("TestApp Controllo lineare");

  // Get Configuration
  OvkCtrlLinear::Config cfg = m_ctrl.getCfg();
  // Change some Configuration

  // Heating example
  cfg.d_a = -10.0f;
  cfg.d_b = 0.0f;
  cfg.p_a = 100.0f;
  cfg.p_b = 0.0f;

  // Cooling example
  // cfg.d_a = 0.0f;
  // cfg.d_b = 5.0f;
  // cfg.p_a = 0.0f;
  // cfg.p_b = 100.0f;
/*
  pinMode(10, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(47, OUTPUT);
while(1) {
  analogWrite(10, 255);
  digitalWrite(39, HIGH);
  digitalWrite(47, HIGH);
  delay(10000);
  digitalWrite(39, LOW);
  analogWrite(10, 127);
  delay(10000);
  analogWrite(10, 0);
  digitalWrite(47, LOW);
  delay(3000);
}  
*/

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
  
  static float mis=15.0f;
  
  uint32_t now = millis();

  // Update Measure
  if (now > tout_3sec) {
    tout_3sec = now + delay_3sec;
    m_ctrl.doWork(mis);
    mis = mis + 1.0f;
    if (mis > 30.0f) {
      mis = 15.0f;
    }
  }

  // Update_PWM
  if (now > tout_pwm) {
    tout_pwm = now + delay_pwm;
    m_ctrl.doTick();
  }
}
