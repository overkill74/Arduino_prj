
#include <Arduino.h>
#include <string.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <AceCRC.h>
using namespace ace_crc::crc16ccitt_nibble;

// Display
//#include "Display.h"

#define UART_BUFFER_SIZE 128

//--------------------------------------------------------------------------------
OneWire oneWire(A0);
DallasTemperature sensore(&oneWire);

static uint8_t m_modbus_addr = 1;

union ModbusRegisters {
  struct regs {
    uint16_t on_off;        //  1 Acceso / spento 1=on
    uint16_t rfu_2;         //  2
    uint16_t working_mode;  //  3 Modalita' funzionamento 0=Cool, 1=Heat, 2=vent
    uint16_t temp_setpont;  //  4 Setpoint temperatura ambiente
    uint16_t key_lock;      //  5 Blocco tasti 1=locked
    uint16_t rfu_6;         //  6
    uint16_t rfu_7;         //  7
    uint16_t rfu_8;         //  8
    uint16_t temp_amb;      //  9 Temperatura ambiente
    uint16_t rfu_10;        //  10
  } regs;
  uint16_t words[10];
};
typedef union ModbusRegisters ModbusRegisters;

static ModbusRegisters m_registers;

//--------------------------------------------------------------------------------
static uint16_t swapEndian(uint16_t value) {
    return (value >> 8) | (value << 8);
}

//--------------------------------------------------------------------------------
static bool modbus_read_coils(const char* buffer) {
  uint16_t adr = ((uint16_t)buffer[2] << 8) | buffer[3];
  uint16_t num = ((uint16_t)buffer[4] << 8) | buffer[5];
  uint16_t crc = ((uint16_t)buffer[6] << 8) | buffer[7];

  uint16_t modbus_ans[16];
  size_t modbus_ans_sz = 0;

  for (uint16_t i=0; i<num; ++i) {
    modbus_ans[i+1] = swapEndian(adr + i); //m_registers.words[adr + i];
  }

  modbus_ans[0] = m_modbus_addr | (uint16_t)(((num * 2)) << 8);

  crc_t crc16 = crc_init();
  crc16 = crc_update(crc16, modbus_ans, num * 2 + 2);
  crc16 = crc_finalize(crc16);
  modbus_ans[num] = swapEndian(0x55AA);

  Serial.write((const char*)modbus_ans, num * 2 + 4);
}

//--------------------------------------------------------------------------------
static bool modbus_parser(const char* buffer, size_t buffer_sz)
{
  if (buffer_sz < 8) {
    return false;
  }
  size_t max_len = buffer_sz - 8;
  for (size_t x=0; x<=max_len; ++x)
  {
    if (buffer[x] == m_modbus_addr)
    {
      if (buffer[x + 1] == 3) {
        modbus_read_coils(&buffer[x]);
      }
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------
static void modbus_receive()
{
  static char message[UART_BUFFER_SIZE];
  static size_t message_pos = 0;
  while (Serial.available() > 0) {
    if (message_pos >= UART_BUFFER_SIZE) {
      // Overflow
      break;
    }
    message[message_pos++] = Serial.read();
    delay(25);
  }

  // if (message_pos) {
  //   // Serial.println(message_pos);
  //    Serial.write((const char*)&message_pos, 2);
  //    Serial.write(message, message_pos);

  // }

  // Parser
  if (modbus_parser(message, message_pos)) {
    // Parser OK
  }

  message_pos = 0;
}

//--------------------------------------------------------------------------------
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Tx

  //--------------------------------------
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
    ;
  //Serial.println("Welcome to Sysco Thermostat");
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);  // Tx

  //--------------------------------------
  // Display
  // SyscoDisplay* dysp = SyscoDisplay::createInstance(0x3C, 128, 64);
  // dysp->drawSetPoint(21.5);
  // dysp->drawTemperature(18.7);
  // dysp->updateEnd();
  //--------------------------------------
  // Local Setpoint
  // pinMode(A1, OUTPUT);
  // pinMode(A3, OUTPUT);
  // digitalWrite(A1, LOW);
  // digitalWrite(A3, HIGH);
  //--------------------------------------
  // Sensore
  sensore.begin();
  // pinMode(3, OUTPUT);
  // digitalWrite(3, HIGH);
  digitalWrite(LED_BUILTIN, LOW);  // Tx
}
//--------------------------------------------------------------------------------
void loop() {
  static uint32_t onesec = 0;
  static int delay_send = 5;
  while (millis() < onesec) {
      modbus_receive();
  }
  onesec += 1000;
  //--------------------------------------
  // Read Setpoint
  int val = analogRead(A2);
  float setpoint = 15.0f + (15.0f / 1020.0f) * (float)val;
  static float old_setpoint = -1.0f;

  if (fabs(old_setpoint - setpoint) > 2) {
    delay_send = 0;
  }

  if (delay_send) {
    --delay_send;
    return;
  }

  delay_send = 5;
  old_setpoint = setpoint;

  sensore.requestTemperatures();
  float temperature = sensore.getTempCByIndex(0) - 3.0f;


  /*
  String s = String((int)(setpoint*100.0f));
  String t = String((int)(temperature*100.0f));
  while(s.length() < 6) { s='0' + s; }
  while(t.length() < 6) { t='0' + t; }

  digitalWrite(2, HIGH);  // Tx
  digitalWrite(LED_BUILTIN, HIGH);  // Tx
  delay(10);
  Serial.println();
  delay(10);
  Serial.println();
  delay(30);
  Serial.print("$$");
  Serial.print(s);
  Serial.print(t);
  Serial.println();
  delay(50);
  digitalWrite(2, LOW);  // Tx
  digitalWrite(LED_BUILTIN, LOW);  // Tx
  //return;
*/
  // SyscoDisplay* dysp = SyscoDisplay::getInstance();
  // dysp->updateBegin();
  // dysp->drawSetPoint(setpoint);
  // dysp->drawTemperature(temperature);
  // dysp->updateEnd();
}
