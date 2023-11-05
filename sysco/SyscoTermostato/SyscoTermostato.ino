
#include <Arduino.h>
#include <string.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// Display
//#include "Display.h"

//--------------------------------------------------------------------------------
OneWire oneWire(A0); 
DallasTemperature sensore(&oneWire);

//--------------------------------------------------------------------------------
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Tx

  //--------------------------------------
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Welcome to Sysco Thermostat");
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
void loop()
{
  static uint32_t onesec = 0;
  static int delay_send = 5;
  while (millis() < onesec);
  onesec+=1000;
  //--------------------------------------
  // Read Setpoint
  int val = analogRead(A2);
  float setpoint = 15.0f + (15.0f/1020.0f) * (float)val;
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

  // SyscoDisplay* dysp = SyscoDisplay::getInstance();
  // dysp->updateBegin();
  // dysp->drawSetPoint(setpoint);
  // dysp->drawTemperature(temperature);
  // dysp->updateEnd();

}

