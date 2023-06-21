
#include <WString.h>

class WiFiUDP;
class NTPClient;

class WebTime
{
public:
  WebTime();

  String getDateTime();

private:
  WiFiUDP*   m_net_udp=nullptr;
  NTPClient* m_client=nullptr;
};
