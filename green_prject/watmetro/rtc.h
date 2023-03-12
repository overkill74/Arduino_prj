
#ifndef OVK_RTC_H
#define OVK_RTC_H

#include <Arduino.h>

// Forward declarations
struct DateTime;
class OvkRtcChip;

class OvkRtc
{
public:
  OvkRtc();
  ~OvkRtc();

  ///
  /// \brief Time string to tm struct
  /// 
  bool toTmTime(const char* str, DateTime& tm);
  ///
  /// \brief Get time to integers
  /// 
  bool toTmTime(const int hour, const int min, const int sec, DateTime& tm);
  ///
  /// \brief Get date to string
  /// 
  bool toTmDate(const char* str, DateTime& tm);
  ///
  /// \brief Get date to integers
  /// 
  bool toTmDate(const int day, const int month, const int year, DateTime& tm);
  ///
  /// \brief set RTC
  ///
  bool setRtcFromCompilationDateTime();
  ///
  /// \brief read RTC
  ///
  bool readToTm(DateTime& tm);
  ///
  /// \brief read RTC to string
  ///
  bool readToString(char* str, size_t str_n);
  ///
  /// \brief Check if initializzation is needed
  ///
  bool isInitNeed();
  ///
  /// \brief Tells if RTC is working
  ///
  bool isChipOk();
  ///
  /// \brief Create unique 8.3 file name from RTC value
  ///
  bool createRtcFileName(char* fname, size_t fname_sz, const char* f_ext=NULL);


private:
  const char* m_month_name[12]; // Month names
  OvkRtcChip* m_rtc;
  bool        m_rtc_ok;
};

#endif
