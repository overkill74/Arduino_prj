
//--------------------------------------------------------------------------------
#include "Arduino.h"

//--------------------------------------------------------------------------------
class SyscoEth
{
public:
  static SyscoEth* createInstance();
  static SyscoEth* getInstance();

private:
  SyscoEth();

private:
  static SyscoEth* m_instance;
};
