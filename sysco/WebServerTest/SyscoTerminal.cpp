
//--------------------------------------------------------------------------------
#include "SyscoTerminal.h"
#include "SysCfg.h"
#include <terminale.h>

//--------------------------------------------------------------------------------
static int setParam_res_d_a (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_res_d_a(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_res_p_a (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_res_p_a(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_res_d_b (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_res_d_b(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_res_p_b (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_res_p_b(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_fan_d_a (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_fan_d_a(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_fan_p_a (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_fan_p_a(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_fan_d_b (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_fan_d_b(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_fan_p_b (int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_fan_p_b(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int setParam_t_offset(int argc, char** argv, Stream& str)
{
  if (argc < 2) {
    str.println("Manca il parametro");
    return -1;
  }
  float new_val = atof(argv[1]);
  SysCfg::getInstance()->setParam_t_offset(new_val);
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
static int print_cfg(int argc, char** argv, Stream& str)
{
  SysCfg::getInstance()->printCfg(str);
  return 0;
}
//-----------------------------------------------------------------------
static const TermCmd g_term_cmd[] =
{
  // <PAR obbligatorio>
  // [PAR opzionale]

  //-------------------------------------------------------------------------
  { "RDA" , setParam_res_d_a , 0, "Set res_d_a " },
  { "RPA" , setParam_res_p_a , 0, "Set res_p_a " },
  { "RDB" , setParam_res_d_b , 0, "Set res_d_b " },
  { "RPB" , setParam_res_p_b , 0, "Set res_p_b " },
  { "FDA" , setParam_fan_d_a , 0, "Set fan_d_a " },
  { "FPA" , setParam_fan_p_a , 0, "Set fan_p_a " },
  { "FDB" , setParam_fan_d_b , 0, "Set fan_d_b " },
  { "FPB" , setParam_fan_p_b , 0, "Set fan_p_b " },
  { "TOFF", setParam_t_offset, 0, "Set t_offset" },
  { "CFG" , print_cfg        , 0, "Print Config" },
  TERMINALE_FINE_LISTA
};

//--------------------------------------------------------------------------------
void SyscoTerminal::doWork()
{
  m_term->doWorkAndAnswer();
}
//--------------------------------------------------------------------------------
SyscoTerminal::SyscoTerminal(Stream& str)
{
  m_term = new Terminale(g_term_cmd, str);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::m_instance = nullptr;
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::createInstance(Stream& str)
{
  if (!m_instance) {
    m_instance = new SyscoTerminal(str);
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::getInstance()
{
  return m_instance;
}
