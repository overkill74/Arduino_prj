
//--------------------------------------------------------------------------------
#include "Arduino.h"
#include "SyscoTermostatoBase.h"

//--------------------------------------------------------------------------------
class SyscoTermostatoIvan : public SyscoTermostatoBase
{
public:
  SyscoTermostatoIvan();

  bool begin() override;
  void doWork() override;

private:
};
