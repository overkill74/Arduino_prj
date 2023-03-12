
// Local include
#include "rtc.h"

// Libraries
#include "RTClib.h"
//#include <Wire.h>

class OvkRtcChip : public RTC_DS1307
{

};

//-----------------------------------------------------------------------
OvkRtc::OvkRtc()
  : m_month_name{"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" }
  , m_rtc(new OvkRtcChip)
  , m_rtc_ok(false)
{
  isChipOk();
}

//-----------------------------------------------------------------------
OvkRtc::~OvkRtc()
{

}
//-----------------------------------------------------------------------
bool OvkRtc::toTmTime(const char* str, DateTime& tm)
{
/*
  int h, m, s;

  if (!str) {
    return false;
  }

  if (sscanf(str, "%d:%d:%d", &h, &m, &s) != 3) {
    return false;
  }
  tm.Hour = h;
  tm.Minute = m;
  tm.Second = s;
  return true;
  */
  return false;
}
//-----------------------------------------------------------------------
bool OvkRtc::toTmTime(const int hour, const int min, const int sec, DateTime& tm)
{
/*
  tm.Hour = hour;
  tm.Minute = min;
  tm.Second = sec;
  return true;
  */
  return false;
}
//-----------------------------------------------------------------------
bool OvkRtc::toTmDate(const char* str, DateTime& tm)
{
/*
  char m[12];
  int d, y;
  uint8_t month_i;

  if (!str) {
    return false;
  }

  if (sscanf(str, "%s %d %d", m, &d, &y) != 3) {
    return false;
  };
  for (month_i = 0; month_i < 12; month_i++) {
    if (strcmp(m, m_month_name[month_i]) == 0) break;
  }
  if (month_i >= 12) {
    return false;
  }
  tm.Day = d;
  tm.Month = month_i + 1;
  tm.Year = CalendarYrToTm(y);

  return true;
  */
  return false;
}
//-----------------------------------------------------------------------
bool OvkRtc::toTmDate(const int day, const int month, const int year, DateTime& tm)
{
  /*
  tm.Day = day;
  tm.Month = month;
  tm.Year = CalendarYrToTm(year);
  return true;
  */
  return false;
}
//-----------------------------------------------------------------------
bool OvkRtc::setRtcFromCompilationDateTime()
{
/*
  DateTime tm;

  if (!toTmDate(__DATE__, tm) || !toTmDate(__TIME__, tm)) {
    return false;
  }

  return RTC.write(tm);
  */
  m_rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));
}
//-----------------------------------------------------------------------
bool OvkRtc::readToTm(DateTime& tm)
{
  if (!isChipOk()) {
    return false;
  }
  tm = m_rtc->now();
  return true;
}
//-----------------------------------------------------------------------
bool OvkRtc::readToString(char* str, size_t str_n)
{
  if (!str || str_n < 24) {
    return false;
  }

  DateTime now = m_rtc->now();

  sprintf(str, "%02d %s %d %02d:%02d:%02d",
          now.day(), m_month_name[now.month()-1], now.year(),
          now.hour(), now.minute(), now.second()
         );

  return true;
}
//-----------------------------------------------------------------------
bool OvkRtc::createRtcFileName(char* fname, size_t fname_sz, const char* f_ext)
{
  if (!fname || fname_sz < 9) {
    return false;
  }
  if (!isChipOk()) {
    return false;
  }

  DateTime now = m_rtc->now();
  char ch_s = (now.second() <= 26) ? ('A' + now.second()) : ('a' + now.second() - 26);
  char ch_y = 'A';
  int yr = now.year() - 2022;
  if (yr < 0) { yr = 0; }
  if (yr > 26) { yr = 26; }
  sprintf(fname, "%c%c%02d%c%02d%c%c%s",
          'A' + yr,
          'a' + (now.month()),
          now.day(),
          'A' + now.hour(),
          now.minute(),
          ch_s,
          f_ext ? '.' : '\0',
          f_ext ? f_ext : ""
          );

  return true;
}

//-----------------------------------------------------------------------
bool OvkRtc::isInitNeed()
{
/*
  DateTime tm;
  if (RTC.read(tm)) {
    return false;
  }
  */
  return !m_rtc->isrunning();
}
//-----------------------------------------------------------------------
bool OvkRtc::isChipOk()
{
  if (!m_rtc_ok) {
    m_rtc_ok = m_rtc->begin();
  }
  return m_rtc_ok;
}
