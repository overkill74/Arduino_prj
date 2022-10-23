
//--------------------------------------------------------------------------------
#include "SyscoTerminal.h"

//--------------------------------------------------------------------------------
SyscoTerminal::SyscoTerminal()
{

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::m_instance = nullptr;
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::createInstance()
{
  if (!m_instance) {
    m_instance = new SyscoTerminal();
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SyscoTerminal* SyscoTerminal::getInstance()
{
  return m_instance;
}
