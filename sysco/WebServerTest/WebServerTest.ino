#include <Arduino.h>
#include <string.h>

// Display
#include "Display.h"
#include "SyscoEth.h"
#include "SyscoTempCtrl.h"
#include "SyscoTerminal.h"
#include "SysCfg.h"

//--------------------------------------------------------------------------------
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Welcome to Sysco Temperature Controller");
  // System config
  SysCfg::createInstance();
  // Terminale
  SyscoTerminal::createInstance(Serial);
  // Display
  SyscoDisplay* dysp = SyscoDisplay::createInstance(0x3C, 128, 64);
  dysp->drawSetPoint(21.5);
  dysp->drawTemperature(18.7);
  // Tmperature Control
  SyscoTempCtrl::createInstance(39, 10, 47, A0);

}
//--------------------------------------------------------------------------------
void loop()
{
  const uint32_t delay_pwm = 20;
  static uint32_t tout_pwm = 0;

  const uint32_t delay_1_sec = 1000;
  static uint32_t tout_1_sec = 0;
  
  SyscoTempCtrl* temp_ctrl = SyscoTempCtrl::getInstance();

  uint32_t now = millis();
  
  // For Display
  static float setpoit = 0.0f;

  // Update Measure
  if (now > tout_1_sec) {
    tout_1_sec += delay_1_sec;
    temp_ctrl->doWork();
  }

  // Update_PWM
  if (now > tout_pwm) {
    tout_pwm += delay_pwm;
    temp_ctrl->doTick();
  }

  // Update display
  SyscoDisplay* dysp = SyscoDisplay::getInstance();
  dysp->updateBegin();
  dysp->drawSetPoint(temp_ctrl->getSetpoint());
  dysp->drawTemperature(temp_ctrl->getTemperature());
  dysp->drawPwmOut(temp_ctrl->getOutFanValue());
  dysp->updateEnd();

  // Terminale
  SyscoTerminal::getInstance()->doWork();
}

