
//--------------------------------------------------------------------------------
class OvkCtrlLinear;

//--------------------------------------------------------------------------------
class SyscoTempCtrl
{
public:
  static SyscoTempCtrl* createInstance(const int pin_r, const int pin_fan, const int pin_ev, const int pin_in_0_10);
  static SyscoTempCtrl* getInstance();

public:
  void doWork(float setpoint, float temperature);
  void doTick();
  float getSetpoint() { return m_setpoint; }
  float getTemperature() { return m_temperature; }
  float getOutFanValue() { return m_mv_out_fan; }

private:
  SyscoTempCtrl(const int pin_r, const int pin_fan, const int pin_ev, const int pin_in_0_10);

protected:
  OvkCtrlLinear* m_ctrl_res=nullptr;
  OvkCtrlLinear* m_ctrl_fan=nullptr;
  const int      m_pin_out_res;  //! Pin uscita resistenza
  const int      m_pin_pwm_fan;  //! Pin PWM ventilatore
  const int      m_pin_out_ev;   //! Pin uscita elettrovalvola
  const int      m_pin_in_0_10;  //! Pin misura 0-10mV
  float          m_setpoint;     //! Setpoint
  float          m_temperature=-1000.0f;  //! Actual temperture
  float          m_mv_out_fan=-1000.0f;   //! mV uscita 0-10 controllo ventilatore

private:
  static SyscoTempCtrl* m_instance;
};
