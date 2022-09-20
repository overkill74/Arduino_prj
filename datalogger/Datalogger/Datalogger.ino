//-----------------------------------------------------------------------
// Author: Ivan Zoli 2022
//-----------------------------------------------------------------------
// Data Logger
//-----------------------------------------------------------------------

//#define WITH_OLED

#include <termargs.h>
#include <terminale.h>

#include "rtc.h"
#include <SPI.h>
#include <SD.h>

#if WITH_OLED
#include <Tiny4kOLED.h>
//#include <Tiny4kOLED_bitbang.h>
#endif

//-----------------------------------------------------------------------
#define SD_CS   10
String g_data_fname;
static OvkRtc* g_rtc=NULL;
static bool g_ist_line = true;
static char tmp[32];

//-----------------------------------------------------------------------
static bool create_data_file()
{
  // Will be replaced with RTC information ;)
  int i=0;
  
  // Read last index
  static const char idx_fname[] = "index.txt";
  File idx_file = SD.open(idx_fname, FILE_READ);
  if (idx_file) {
    idx_file.read(tmp, 8);
    idx_file.close();
    i=atoi(tmp);
  }

  for (; i<10000; ++i)
  {
    sprintf(tmp, "data%04d.csv", i);
    if (SD.exists(tmp)) {
      continue;
    }
    // Found new fname
    g_data_fname = String(tmp);
    break;
  }

  if (SD.exists(idx_fname)) {
    SD.remove(idx_fname);
  }

  idx_file = SD.open(idx_fname, FILE_WRITE);
  if (idx_file) {
    idx_file.print(i+1);
    idx_file.close();
  }

  return g_data_fname.length() >= 8;
}

//-----------------------------------------------------------------------
//static bool create_data_file_from_rtc()
//{
//  char tmp[16];
//  if (!g_rtc->createRtcFileName(tmp, sizeof(tmp), "csv")) {
//      Serial.println(tmp);
//      Serial.println("RTC Fail!");
//      return create_data_file();
//  }
//
//  File dataFile = SD.open(tmp, FILE_WRITE);
//  if (dataFile) {
//    // OK!
//    g_data_fname = String(tmp);
//    dataFile.close();
//    return true;
//  }
//
//  return create_data_file();
//}

//-----------------------------------------------------------------------
static int get_fname(int argc, char** argv)
{
  Serial.print("Recording Fname: ");
  Serial.println(g_data_fname);
  return 0;
}

//-----------------------------------------------------------------------
static int rw_rtc(int argc, char** argv)
{
  if (!g_rtc->readToString(tmp, sizeof(tmp))) {
    return -1;
  }
  Serial.println(tmp);
  return 0;
}

//-----------------------------------------------------------------------
static const TermCmd g_term_cmd[] =
{
  // <PAR obbligatorio>
  // [PAR opzionale]

  //-------------------------------------------------------------------------
//  { "FN",   get_fname ,  0, "Datalogger file name"            },
  { "RTC",  rw_rtc    ,  0, ""    },
  TERMINALE_FINE_LISTA
};

//-----------------------------------------------------------------------
//static Terminale g_term(g_term_cmd);

//-----------------------------------------------------------------------
void setup()
{
  // Serial
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("\n\n\nDatalogger Starting");

  g_rtc = new OvkRtc;
//  if (g_rtc->isInitNeed()) {
//    Serial.println("\n\n\nInit RTC!");
//    g_rtc->setRtcFromCompilationDateTime();
//  }

  if (!g_rtc->readToString(tmp, sizeof(tmp))) {
    strcpy(tmp, "RTC Fail");
  }

  bool log_ok = false;
  do
  {
    // see if the card is present and can be initialized:
    if (!SD.begin(SD_CS)) {
      Serial.println("Card failed");
      break;
    }
    Serial.println("Card ok");

    if (!create_data_file()) {
      Serial.println("Cannot log");
      break;
    }

    log_ok = true;
  } while (0);

#if WITH_OLED
  // Display Init
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.clear();
  oled.setFont(FONT8X16);
  oled.on();

  oled.setCursor(0,6);
  oled.println(tmp);

  oled.setCursor(0,0);
#endif
  
  if (g_data_fname.length() > 8) {
    Serial.print("Logging file: ");
    Serial.println(g_data_fname);
#if WITH_OLED
    oled.println(g_data_fname);
  }
  else {
    oled.println("Missong SD :(");
#endif
  }
}

//-----------------------------------------------------------------------
void loop()
{

#define DELTA_PRINT_ms  (5*60*1000UL)
#define DELTA_ACQUIS_ms 10
  static int32_t to = 0;
  static int32_t to_print = -1;
  static float old_adc = -1000;
  String txt;
  static int32_t actual_ms = 0;

  if (g_ist_line) {
    g_ist_line = false;
    if (g_rtc->readToString(tmp, sizeof(tmp))) {
      //Serial.println(tmp);
      txt += tmp;
      txt += '\n';
    }
    txt += "Time[ms];ADC;A0[V]\n";
  }

  //g_term.doWorkAndAnswer();

  while (millis() < to);
  actual_ms = to;
  to += DELTA_ACQUIS_ms;

  // Timeout print
  bool prn = (millis() >= to_print);
  float adc = (float)analogRead(0);
//  if (abs((adc - old_adc) / (adc + old_adc)) > 0.02) {
  if (abs((adc - old_adc) / adc) > 0.02) {
    // over 2% treshold
    prn = true;
  }
  if (old_adc < 0) {
    old_adc = adc;
  }
  old_adc = old_adc * 0.5 + adc * 0.5;

  if (!prn) {
    // Do not print
    return;
  }
  to_print = actual_ms + DELTA_PRINT_ms;

  float vin = old_adc * (44.2477876);
  String vin_str = String(vin / 1000, 2);
  // Add time
  txt += String(actual_ms);
  // Add ADC Counts
  txt += String(";"); txt += String((int)(adc));
  // Add A0
  txt += String(";"); txt += vin_str;

#if WITH_OLED
  oled.setCursor(0,4);
  oled.print(vin_str);
  oled.println("   ");
#endif

  File df = SD.open(g_data_fname, FILE_WRITE);
  if (!df) {
    Serial.println(txt);
    return;
  }

  df.println(txt);
  df.close();
}
