
// Display
#include "Display.h"
#include "SyscoEth.h"
#include "SyscoTempCtrl.h"
#include "SyscoTerminal.h"

//--------------------------------------------------------------------------------
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Welcome to Sysco Temperature Controller");
  // Display
  SyscoDisplay* dysp = SyscoDisplay::createInstance(0x3C, 128, 64);
  dysp->drawSetPoint(21.5);
  dysp->drawTemperature(18.7);
  // Tmperature Control
  SyscoTempCtrl::createInstance(39, 47);
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
  
  // Update Measure
  if (now > tout_1_sec) {
    tout_1_sec += delay_1_sec;
    float setpoit = temp_ctrl->doWork();
    SyscoDisplay* dysp = SyscoDisplay::getInstance();
    if (dysp) { dysp->drawSetPoint(setpoit); }
  }

  // Update_PWM
  if (now > tout_pwm) {
    tout_pwm += delay_pwm;
    temp_ctrl->doTick();
  }
}

