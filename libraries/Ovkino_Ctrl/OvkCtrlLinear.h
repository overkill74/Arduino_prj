///
/// \brief Linear control object
/// The object has a setpoint and confront it with the current value.
/// The tick function must be called with a timer and set the PWM out.
///
///                     ^ PWM                                           
///                     |                                           
///                     |   ________________________________  P[b]
///                     |  /|                                           
///                     | /                                            
///                     |/  |                                          
///                     /                                              
///                    /|   |                                          
///                   / |
///                  /  |   |            
/// P[a] ___________/ _ |                                               
///                 |   |   |                                            
///  -------------------+-----------------------------------------------> Delta
///              D[a]   |   D[b]
///
/// Delta = Misura - StPoint
/// D[a] < D[b]
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
      // D[a] < D[b]
      if (d_a > d_b) {
        float d_tmp = d_b;
        float p_tmp = p_b;
        d_b = d_a;
        p_b = p_a;
        d_a = d_tmp;
        p_a = p_tmp;
      }
		}
		float  d_a = 5.0f;    //! Delta, a
		float  d_b = 0.0f;    //! Delta, b
		float  p_a = 100.0f;  //! PWM, a
		float  p_b = 0.0f;    //! PWM, b
	};
	
public:
  OvkCtrlLinear(const int out_pin=LED_BUILTIN, float setpoint=50.0f);
public:
  ///
  /// \brief Apply output
  ///
  virtual void applyOutput();
  ///
  /// \brief tick callback
  ///
  virtual void doTick();

public:
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

protected:
  const int m_out_pin;              //! Output PIN
  float     m_setpoint;             //! The current setpoint 
  int       m_pwm_level   = 0;      //! Out PWM level

private:
  int       m_pwm_cnt     = 0;      //! Step counter
  bool      m_is_running  = true;   //! PWM is running
  Config	  m_cfg;					        //! The configuration
  float     m_cm;                   //! m coefficient
  float     m_cq;                   //! q coefficient
};



#endif
