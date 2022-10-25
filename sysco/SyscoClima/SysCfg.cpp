//--------------------------------------------------------------------------------
#include "SysCfg.h"
#include <EEPROM.h>

//--------------------------------------------------------------------------------
SysCfg::SysCfg()
{
  read_cfg();
  printCfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_res_d_a (float new_val)
{ 
  m_cfg.res_d_a  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_res_p_a (float new_val)
{ 
  m_cfg.res_p_a  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_res_d_b (float new_val)
{ 
  m_cfg.res_d_b  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_res_p_b (float new_val)
{ 
  m_cfg.res_p_b  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_fan_d_a (float new_val)
{ 
  m_cfg.fan_d_a  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_fan_p_a (float new_val)
{ 
  m_cfg.fan_p_a  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_fan_d_b (float new_val)
{ 
  m_cfg.fan_d_b  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_fan_p_b (float new_val)
{ 
  m_cfg.fan_p_b  = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::setParam_t_offset(float new_val)
{ 
  m_cfg.t_offset = new_val;
  update_cfg();
}
//--------------------------------------------------------------------------------
void SysCfg::read_cfg()
{
  Cfg tmp_cfg;
  EEPROM.get(0, tmp_cfg);
  if (tmp_cfg.magic == Cfg::getMagic()) {
    memcpy(&m_cfg, &tmp_cfg, sizeof(Cfg));
  }
  else {
    Serial.println("New Config!");
    update_cfg();
  }
}
//--------------------------------------------------------------------------------
void SysCfg::update_cfg()
{
  EEPROM.put(0, m_cfg);
}
//--------------------------------------------------------------------------------
void SysCfg::printCfg(const Cfg& cfg, Stream& str)
{
    Serial.print("magic    :0x"); str.println(cfg.magic, HEX);
    Serial.print("res_d_a  :");   str.println(cfg.res_d_a );
    Serial.print("res_p_a  :");   str.println(cfg.res_p_a );
    Serial.print("res_d_b  :");   str.println(cfg.res_d_b );
    Serial.print("res_p_b  :");   str.println(cfg.res_p_b );
    Serial.print("fan_d_a  :");   str.println(cfg.fan_d_a );
    Serial.print("fan_p_a  :");   str.println(cfg.fan_p_a );
    Serial.print("fan_d_b  :");   str.println(cfg.fan_d_b );
    Serial.print("fan_p_b  :");   str.println(cfg.fan_p_b );
    Serial.print("t_offset :");   str.println(cfg.t_offset);
 
}
//--------------------------------------------------------------------------------
void SysCfg::printCfg(Stream& str)
{
  printCfg(m_cfg, str);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SysCfg* SysCfg::m_instance = nullptr;
//--------------------------------------------------------------------------------
SysCfg* SysCfg::createInstance()
{
  if (!m_instance) {
    m_instance = new SysCfg();
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SysCfg* SysCfg::getInstance()
{
  return m_instance;
}
