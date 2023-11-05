
#include <WString.h>

class WiFiUDP;
class NTPClient;

class WebTime
{
public:
  WebTime();

  String getDateTime();
  void setDst(bool is_dst);

private:
  WiFiUDP*   m_net_udp=nullptr;
  NTPClient* m_client=nullptr;
};
