
//--------------------------------------------------------------------------------
#include "Arduino.h"

class Terminale;

//--------------------------------------------------------------------------------
class SyscoTerminal
{
public:
  static SyscoTerminal* createInstance(Stream& str=Serial);
  static SyscoTerminal* getInstance();

public:
  void doWork();

private:
  SyscoTerminal(Stream& str);

protected:
  Terminale* m_term=nullptr;

private:
  static SyscoTerminal* m_instance;
};
