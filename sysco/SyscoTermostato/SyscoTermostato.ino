
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
}
//--------------------------------------------------------------------------------
void loop()
{
  static uint32_t onesec = 0;
  while (millis() < onesec);
  onesec+=2500;
  //--------------------------------------
  // Read Setpoint
  int val = analogRead(A2);
  float setpoint = 15.0f + (15.0f/1020.0f) * (float)val;
  sensore.requestTemperatures();
  float temperature = sensore.getTempCByIndex(0) - 3.0f;

  String s = String((int)(setpoint*100.0f));
  String t = String((int)(temperature*100.0f));
  while(s.length() < 6) { s='0' + s; }
  while(t.length() < 6) { t='0' + t; }

  digitalWrite(2, HIGH);  // Tx
  delay(100);
  Serial.println();
  delay(100);
  Serial.println();
  delay(100);
  Serial.print("$$");
  Serial.print(s);
  Serial.print(t);
  Serial.println();
  delay(100);
  digitalWrite(2, LOW);  // Tx
  //return;

  // SyscoDisplay* dysp = SyscoDisplay::getInstance();
  // dysp->updateBegin();
  // dysp->drawSetPoint(setpoint);
  // dysp->drawTemperature(temperature);
  // dysp->updateEnd();

}

