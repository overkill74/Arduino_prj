#include "web-time.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

WebTime::WebTime()
{
  m_net_udp = new WiFiUDP();
  m_client = new NTPClient(*m_net_udp);

  m_client->begin();
  m_client->setTimeOffset(3600*2);
}

String WebTime::getDateTime()
{
  m_client->update();
  return m_client->getFormattedTime();
}

