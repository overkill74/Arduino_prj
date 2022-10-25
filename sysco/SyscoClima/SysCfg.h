#include "HardwareSerial.h"
//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class SysCfg
{
public:
  static SysCfg* createInstance();
  static SysCfg* getInstance();

public:
  struct Cfg
  {
    static uint16_t getMagic() { return 0xDA77; }
    //----------------------------
    uint16_t magic = 0xDA77;
    // Resistenza
    float res_d_a = -10.0f;
    float res_p_a = 100.0f;
    float res_d_b = 0.0f;
    float res_p_b = 0.0f;
    // Ventilatore
    float fan_d_a = 0.0f;
    float fan_p_a = 0.0f;
    float fan_d_b = 5.0f;
    float fan_p_b = 100.0f;
    // Taratura
    float t_offset = -80.0f;
  };
public:
    float getParam_res_d_a () { return m_cfg.res_d_a ; }
    float getParam_res_p_a () { return m_cfg.res_p_a ; }
    float getParam_res_d_b () { return m_cfg.res_d_b ; }
    float getParam_res_p_b () { return m_cfg.res_p_b ; }
    float getParam_fan_d_a () { return m_cfg.fan_d_a ; }
    float getParam_fan_p_a () { return m_cfg.fan_p_a ; }
    float getParam_fan_d_b () { return m_cfg.fan_d_b ; }
    float getParam_fan_p_b () { return m_cfg.fan_p_b ; }
    float getParam_t_offset() { return m_cfg.t_offset; }

    void setParam_res_d_a (float new_val);
    void setParam_res_p_a (float new_val);
    void setParam_res_d_b (float new_val);
    void setParam_res_p_b (float new_val);
    void setParam_fan_d_a (float new_val);
    void setParam_fan_p_a (float new_val);
    void setParam_fan_d_b (float new_val);
    void setParam_fan_p_b (float new_val);
    void setParam_t_offset(float new_val);

public:
    void printCfg(Stream& str=Serial);
    void printCfg(const Cfg& cfg, Stream& str=Serial);
    
private:
  SysCfg();
  void read_cfg();
  void update_cfg();

protected:
  Cfg   m_cfg;  // System configuration

private:
  static SysCfg* m_instance;
};
