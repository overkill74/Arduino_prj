#include <Arduino.h>
#include <string.h>

// Display
#include "Display.h"

//--------------------------------------------------------------------------------
void setup()
{
  //--------------------------------------
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Welcome to Watmeter :)");
  //--------------------------------------
  // Display
  OvkDisplay* dysp = OvkDisplay::createInstance(0x3C, 128, 64);
  dysp->updateBegin();
  dysp->drawStatus("Datalogger Starting");
  dysp->drawInstantWatt(0);
  dysp->drawInstantWattHour(0);
  dysp->updateEnd();
  //--------------------------------------
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  digitalWrite(A0, HIGH);
}
//--------------------------------------------------------------------------------
void loop()
{
  static uint32_t onesec = 0;
  static uint32_t watt_sec = 0;
  const int calibra = -63;
  
  while (millis() < onesec) { };
  onesec += 1000;
 
  int i_max = 0;
  int i_min = 5000;
  uint32_t half_sec = millis() + 500;
  while (millis() < half_sec) {
    int ar = analogRead(A1) - 512;
    if (ar > i_max) { i_max = ar; }
    if (ar < i_min) { i_min = ar; }
  }
  //onesec+=1000;

  int i_med = (i_max - i_min) / 2;

  int w_inst = (int)(((float)i_med) * (230 * 1.414213562f * 0.01f * 5000.0f / 1024.0f)) + calibra;
  if (w_inst < 20) {
    w_inst = 0;
  }
  watt_sec += w_inst;

  int32_t sec = (int32_t)millis();///1000;
  Serial.print(sec); Serial.print(" - ");
  int32_t minu = (sec / 60);
  minu %= 60;
  Serial.print(minu); Serial.print(" - ");
  int32_t hour = (minu / 60);
  hour %= 60;
  Serial.print(hour); Serial.print(" - ");

  char tmp[32];
  sprintf(tmp, "%02d:%02d:%02d", hour, minu, sec);
  Serial.println(tmp);

  OvkDisplay* dysp = OvkDisplay::getInstance();
  dysp->updateBegin();
  dysp->drawStatus(tmp);
  dysp->drawInstantWatt(w_inst);
  dysp->drawInstantWattHour(watt_sec / 3600);
  //dysp->drawTime();
  dysp->updateEnd();

}
