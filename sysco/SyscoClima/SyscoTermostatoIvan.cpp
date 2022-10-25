#include "HardwareSerial.h"
//#include <cstring>
//#include <cstdlib>
//--------------------------------------------------------------------------------
#include "SyscoTermostatoIvan.h"

//--------------------------------------------------------------------------------
SyscoTermostatoIvan::SyscoTermostatoIvan()
{

}
//--------------------------------------------------------------------------------
bool SyscoTermostatoIvan::begin()
{
  Serial1.begin(9600);
  while (!Serial1);

  pinMode(22, OUTPUT);  digitalWrite(22, LOW);
  pinMode(23, OUTPUT);  digitalWrite(23, LOW);
  pinMode(24, OUTPUT);  digitalWrite(24, LOW);
  pinMode(25, OUTPUT);  digitalWrite(25, LOW);

  return true;
}
//--------------------------------------------------------------------------------
void SyscoTermostatoIvan::doWork()
{
  char rx_sp[8];
  char rx_te[8];
  int i=0;
  int i_sp=0;
  int i_te=0;
  
  while (Serial1.available()) {
    char ch = Serial1.read();
    // Serial.print("ch   :"); Serial.println(ch);
    // Serial.print("i    :"); Serial.println(i);
    if (0 == i) {
      // Wait for '$'
      if (ch == '$') {
        // Start
        i_sp=0;
        i_te=0;
      }
      else {
        continue;
      }
    }

    if (1 == i) {
      // Wait for 2nd '$'
      if (ch != '$') {
        i = 0;
        continue;
      }
    }
    // 01 234567 890123
    // $$ 002976 002443
    if (i>=2 && i<=7) {
      if (ch>= '0' && ch <='9') {
        rx_sp[i_sp++] = ch;
        rx_sp[i_sp] = '\0';
      }
      else {
        i=0;
        continue;
      }
    }

    if (i>=8 && i<=13) {
      if (ch>= '0' && ch <='9') {
        rx_te[i_te++] = ch;
        rx_te[i_te] = '\0';
      }
      else {
        i=0;
        continue;
      }
    }
    
    // Waste other bytes
    ++i;
  }

  // Check for compliance
  if (i_sp != 6 || i_te != 6) {
    // Serial.println("Skip malformed buffer");
    // Serial.print("i_sp :"); Serial.print(i_sp); Serial.print(" -> '"); Serial.print(rx_sp); Serial.println("'");
    // Serial.print("i_te :"); Serial.print(i_te); Serial.print(" -> '"); Serial.print(rx_te); Serial.println("'");
    return;
  }
  // Interpret
  int isp = atoi(rx_sp);
  int ite = atoi(rx_te);

  m_setpoint = ((float)isp) / 100.0f;
  m_temperature = ((float)ite) / 100.0f;
}


