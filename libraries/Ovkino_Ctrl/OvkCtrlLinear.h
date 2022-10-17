///
/// \brief Linear control object
/// The object has a setpoint and confront it with the current value.
/// The tick function must be called with a timer and set the PWM out.
///

#ifndef OVK_CTRL_LINEAR_H
#define OVK_CTRL_LINEAR_H

///
/// \brief Temperature control class
///
class OvkCtrlLinear
{
public:
	///
	/// \brief Configuration
	///
	struct Config
	{
		void copy(const Config& new_cfg) {
			memcpy(this, &new_cfg, sizeof(Config));
		}
		float  coeff_m  = 2.5f;   //! coefficiente m
		float  coeff_q  = -0.5f;  //! coefficiente q
		float  setp_min = 10.0f;  //! Min Setpoint
		float  setp_max = 300.0f; //! Max Setpoint
	};
	
public:
  OvkCtrlLinear(const int out_pin=LED_BUILTIN, float setpoint=50.0f);

  ///
  /// \brief read setpoint
  ///
  float getSetpoint() { return m_setpoint; }
  ///
  /// \prief get PWM level
  ///
  int getPwm();
  ///
  /// \brief write setpoint
  ///
  float setSetpoint(float new_val);
  float setSetpoint(int new_val) {
    return setSetpoint((float)new_val);
  }
  ///
  /// \brief Do work
  ///
  void doWork(float new_temp);
  ///
  /// \brief tick callback
  ///
  void doTick();
  ///
  /// \brief start PWM
  ///
  void startPwm();
  ///
  /// \brief stop PWM
  ///
  void stopPwm();
  ///
  /// \brief get configuration
  ///
  Config& getCfg();
  ///
  /// \brief set New configuration
  ///
  void setCfg(const Config& new_cfg);

private:
  const int m_out_pin;              //! Output PIN
  const int m_pwm_steps;            //! PWM steps
  float     m_setpoint;             //! The current setpoint 

private:
  int       m_pwm_cnt     = 0;      //! Step counter
  int       m_pwm_level   = 0;      //! Out PWM level
  bool      m_is_running  = true;   //! PWM is running
  Config	m_cfg;					//! The configuration
};



#endif
