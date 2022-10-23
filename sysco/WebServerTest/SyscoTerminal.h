
//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class SyscoTerminal
{
public:
  static SyscoTerminal* createInstance();
  static SyscoTerminal* getInstance();

private:
  SyscoTerminal();

private:
  static SyscoTerminal* m_instance;
};
