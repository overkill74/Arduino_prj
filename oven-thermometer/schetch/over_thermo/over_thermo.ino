#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <max6675.h>
#include <EEPROM.h>
#include <ezButton.h>

//----------------------------------------------------------------------
#include <DallasTemperature.h>
#include <OneWire.h>
#include "IvanSensore_DS18B20.h"
#include "temp_ctrl.h"

//-----------------------------------------------------------------------
// Confiugratore dello sketch
// define to enable SD log
//#define APP_LOG_ON_SD

// true or false to enable temp simulators
#define APP_SIMULATORE false

// define to enable modbus, undef output on Serial
#define APP_MODBUS

// define pseudo frequency of main loop
#define APP_FRQ_HZ   50
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// PIN ASSIGNMENT
//-----------------------------------------------------------------------
// One Wire
#define ONE_WIRE_BUS_MANDATA    8
#define ONE_WIRE_PIN_VCC        9
#define ONE_WIRE_PIN_GND        10

// Thermocouple
#define TC_CLK                  11
#define TC_CS                   12
#define TC_SDO                  13

// NTC
#define NTC                     A5

// SSR Output
#define SSR_OUTPUT              A4

// LCD
#define LCD_CLK                 7
#define LCD_DIN                 6
#define LCD_DC                  5
#define LCD_CS                  4
#define LCD_RST                 3
#define BKL_PIN                 2

// KEY
#define KEY_PLUS                A0
#define KEY_MINUS               A1

#define ONE_WIRE_RESOLUTION     12

//-----------------------------------------------------------------------
static IvanSensore_DS18B20 m_sens_temp(ONE_WIRE_BUS_MANDATA, ONE_WIRE_RESOLUTION, APP_SIMULATORE);
static MAX6675 m_thermocouple(TC_CLK, TC_CS, TC_SDO);
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_CLK, LCD_DIN, LCD_DC, LCD_CS, LCD_RST);

// Buttons
static ezButton   m_key_plus(KEY_PLUS);
static ezButton   m_key_minus(KEY_MINUS);

static TempCtrl m_temp_ctrl(SSR_OUTPUT);

uint32_t m_rst_time = 0;  //! Reference time

//-----------------------------------------------------------------------
static void stampa_tempo(int linea, bool do_blink)
{
  unsigned long runMillis = millis() - m_rst_time;
  unsigned long allSeconds = runMillis / 1000;
  int runHours = allSeconds / 3600;
  int secsRemaining = allSeconds % 3600;
  int runMinutes = secsRemaining / 60;
  int runSeconds = secsRemaining % 60;
  static int cnt = 0;

  //  char buf2[32];
  //  sprintf(buf2, "%d:%02d:%02d", runHours, runMinutes, runSeconds);
  //  Serial.println(buf2);
  ++cnt;
  bool is_blank = (do_blink && (cnt & 1));


  display.setTextSize(2);
  display.setCursor(0, linea);
  if (is_blank) {
    display.print(" ");
  }
  else {
    display.print(runHours);
  }
  display.setCursor(8, linea);
  if (cnt & 2) {
    display.print(is_blank ? " " : ":");
  }
  else {
    display.print(" ");
  }
  display.setCursor(16, linea);
  char buf[16];
  sprintf(buf, "%02d", runMinutes);
  display.print(is_blank ? " " : buf);

  display.drawFastVLine(42, 28, 20, 0xFFFF);
}

//-----------------------------------------------------------------------
static void stampa_temperatura(float celsius, int line, bool is_tc, bool is_ntc)
{
  static bool is_high = false;
  bool is_error = false;
  int x = 0;

  if (celsius > 999.0 || celsius < -99.0) {
    is_error = true;
  }

  if (celsius >= 0.0) {
    // Histeresys positive value
    if (is_high && celsius < 90.0) {
      is_high = false;
    }
    if (!is_high && celsius >= 100.0) {
      is_high = true;
    }
  }
  else {
    // Histeresys negative value
    if (is_high && celsius >= -9.0) {
      is_high = false;
    }
    if (!is_high && celsius <= -10.0) {
      is_high = true;
    }
  }

  String tmp;
  if (is_error) {
    // Errore
    tmp = "Err!";
    x = 4;
  }
  else {
    if (is_high) {
      // 100
      int c = (int)celsius;
      tmp = String(c);
      if (c < 100) {
        x = 36;
      }
      else {
        x = 18;
      }
    }
    else {
      // 25.1
      tmp = String(celsius, 1);
      if (celsius >= 0.0 && celsius <= 9.99) {
        x = 18;
      }
    }
  }
  //Serial.println(tmp);

  // text display tests
  display.setTextColor(BLACK);
  display.setTextSize(3);

  // Draw temperature
  display.setCursor(x, line);
  display.print(tmp);

  // Draw symbol
  if (!is_error) {
    display.setCursor(72, line);
    display.setTextSize(2);
    display.println("C");
  }

  // TC symbol
  if (is_ntc) {
    display.setTextSize(1);
    display.setCursor(72, line + 16);
    display.println("Nt");
  }
  else if (is_tc) {
    display.setTextSize(1);
    display.setCursor(72, line + 16);
    display.println("TC");
  }

  display.drawFastHLine(0, line + 24, 86, 0xFFFF);
}
//-----------------------------------------------------------------------
static void stampa_setpoint(float sp, int linea, bool do_blink)
{
  //static uint32_t old = 0;

  static bool old_blink = false;
  static int cnt = 0;

  if (do_blink) {
    if (!old_blink) {
      cnt = 0;
    }
    ++cnt;
  }
  old_blink = do_blink;

  char buf2[16];
  int isp = (int)sp;
  if (do_blink) {
    if (cnt & 1) {
      if (sp >= 100) {
        sprintf(buf2, "%d", (int)sp);
      }
      else {
        sprintf(buf2, "% 3d", (int)sp);
      }
    }
    else {
      sprintf(buf2, "   ");
    }
  }
  else {
    if (sp >= 100) {
      sprintf(buf2, "%d", (int)sp);
    }
    else {
      sprintf(buf2, "% 3d", (int)sp);
    }
  }

  display.setTextSize(2);
  display.setCursor(48, linea);
  display.print(buf2);

  //Serial.println(millis() - old);
  //old = millis();
}

//-----------------------------------------------------------------------
static float read_tc()
{
  static float old_tc = -10000.0f;
  static bool fail = true;

  float tc = m_thermocouple.readCelsius();

  if (isnan(tc)) {
    fail = true;
  }

  if (fail) {
    // In case of fail must read something different than 0,0
    if (fabs(tc) > 0.1f) {
      fail = false;
    }
  }

  if (fail) {
    tc = -10000.0f;
    old_tc = -10000.0f;
  }
  else {
    if (old_tc < -1000.0f) {
      // init value
      old_tc = tc;
    }
    else {
      old_tc = old_tc * 0.6 + tc * 0.4;
    }
  }

  return old_tc;
}

//-----------------------------------------------------------------------
static float read_ntc()
{
  static float old_ntc = -10000.0f;

  int counts = analogRead(NTC);
  if (counts > 1000) {
    // Overflow
    old_ntc = -10000.0f;
  }
  else {
    float ntc = -0.14197f * (float)(counts) + 146.515f;

    if (old_ntc < -1000.0f) {
      // init value
      old_ntc = ntc;
    }
    else {
      old_ntc = old_ntc * 0.6 + ntc * 0.4;
    }
  }

  return old_ntc;
}

//-----------------------------------------------------------------------
unsigned int outputPin = SSR_OUTPUT;
void setup()
{
  Serial.begin(115200);

  Serial.println("Init Temperature");
  pinMode(ONE_WIRE_PIN_VCC, OUTPUT);
  pinMode(ONE_WIRE_PIN_GND, OUTPUT);
  digitalWrite(ONE_WIRE_PIN_VCC, HIGH);
  digitalWrite(ONE_WIRE_PIN_GND, LOW);

  pinMode(SSR_OUTPUT, OUTPUT);
  digitalWrite(SSR_OUTPUT, LOW);

  pinMode(BKL_PIN, OUTPUT);
  digitalWrite(BKL_PIN, HIGH);

  Serial.println("Init Display");
  display.begin();

  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(45);

  // Backlight
  digitalWrite(BKL_PIN, LOW);

  // Init sensor
  for (int x = 0; x < 10; ++x) {
    m_sens_temp.readTemperatureC();
  }

  // Read Setpoint from EEPROM
  float sp = 0.0f;

  EEPROM.get(0, sp);

  sp = m_temp_ctrl.setSetpoint(sp);

  Serial.print("Setpoint: ");
  Serial.println(sp);

  Serial.println("ONEW;Tc;NTC;Setpoint;Out;");
  return;
}

//-----------------------------------------------------------------------
void loop()
{
  const uint32_t delay_read_temp = 250;
  uint32_t tout_read_temp = 0;

  const uint32_t delay_pwm = 20;
  uint32_t tout_pwm = 0;

  uint32_t tout_blink = 0;
  const uint32_t delay_blink = 2500;

  uint32_t tout_rst_time = 0;
  uint32_t tout_rst_time_sample = 0;
  const uint32_t delay_rst_time = 3000;

  uint32_t time_sample_inc = 0;
  uint32_t time_sample_dec = 0;
  const uint32_t delay_fast_inc_dec = 3000;
  const float fast_inc_dec_step = 5.0f;

  while (1)
  {
    bool time_just_reset = false;

    if (millis() > tout_read_temp)
    {
      // Read Temperature
      tout_read_temp = millis() + delay_read_temp;

      float temperatura = -1000.0f;//m_sens_temp.readTemperatureC();
      float temp_tc = read_tc();
      float temp_ntc = read_ntc();

      // Select sensor to display
      static bool is_tc = false;
      bool is_ntc = (temp_ntc > -1000.0f);

      if (!is_tc) {
        if (temp_tc > temperatura + 30) {
          is_tc = true;
        }
      }
      else {
        if (temp_tc < temperatura + 10) {
          is_tc = false;
        }
      }

      // Set temperature
      float temp = is_ntc ? temp_ntc : (is_tc ? temp_tc : temperatura);

      // Test Keys
      if (m_key_plus.getState() && m_key_minus.getState())
      {
        tout_rst_time = millis() + delay_rst_time;
        if (!tout_rst_time_sample) {
          // Sample
          tout_rst_time_sample = millis() + delay_rst_time;
        }
        if (millis() > tout_rst_time_sample) {
          Serial.println("Reset Timer!");
          m_rst_time = millis();
          time_just_reset = true;
        }
      }
      else {
        tout_rst_time_sample = 0;
      }

      bool set_temp = (millis() < tout_blink);
      bool wait_rst_time = (millis() < tout_rst_time);
      if (wait_rst_time) {
        if (m_key_plus.getState() || m_key_minus.getState()) {
          tout_rst_time = millis() + delay_rst_time;
        }
        set_temp = false;
      }

      bool not_modified = true;
      if (set_temp)
      {
        // Increment
        if (m_key_plus.getState() && !m_key_minus.getState())
        {
          tout_blink = millis() + delay_blink;
          if (!time_sample_inc) {
            time_sample_inc = millis() + delay_fast_inc_dec;
          }
          time_sample_dec = 0;
          float sp = m_temp_ctrl.getSetpoint();
          if ((millis() > time_sample_inc)) {
            sp = sp + fmod(sp, fast_inc_dec_step);
            sp += fast_inc_dec_step;
          }
          else {
            ++sp;
          }
          not_modified = false;
          EEPROM.put(0, m_temp_ctrl.setSetpoint(sp));
        }
        else {
          time_sample_inc = 0;
        }

        // Decrement
        if (!m_key_plus.getState() && m_key_minus.getState())
        {
          tout_blink = millis() + delay_blink;
          if (!time_sample_dec) {
            time_sample_dec = millis() + delay_fast_inc_dec;
          }
          time_sample_inc = 0;
          float sp = m_temp_ctrl.getSetpoint();
          if ((millis() > time_sample_dec)) {
            sp = sp - fmod(sp, fast_inc_dec_step);
            sp -= fast_inc_dec_step;
          }
          else {
            --sp;
          }
          not_modified = false;
          EEPROM.put(0, m_temp_ctrl.setSetpoint(sp));
        }
        else {
          time_sample_dec = 0;
        }
      }

      // Clear display
      display.clearDisplay();

      stampa_temperatura(temp, 4, is_tc, is_ntc);
      stampa_tempo(32, wait_rst_time & ~time_just_reset);
      stampa_setpoint(m_temp_ctrl.getSetpoint() + 0.5f, 32, set_temp & not_modified);

      // Update display
      display.display();

      // Update PWM
      if (temp < -100.0f) {
        m_temp_ctrl.stopPwm();
      }
      else {
        m_temp_ctrl.doWork(temp);
      }

      Serial.print(temperatura); Serial.print(";");
      Serial.print(temp_tc); Serial.print(";");
      Serial.print(temp_ntc); Serial.print(";");
      Serial.print(m_temp_ctrl.getSetpoint()); Serial.print(";");
      Serial.print(m_temp_ctrl.getPwm()); Serial.print(";");
      Serial.println(" ");
    }

    if (millis() > tout_pwm) {
      // Update_PWM
      tout_pwm = millis() + delay_pwm;
      m_temp_ctrl.doTick();
    }

    // Keyboard
    m_key_plus.loop();
    m_key_minus.loop();
    if (m_key_plus.isPressed() && !m_key_minus.isPressed()) {
      tout_blink = millis() + delay_blink;
    }
    if (!m_key_plus.isPressed() && m_key_minus.isPressed()) {
      tout_blink = millis() + delay_blink;
    }
  }

}
