// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2

void setup()
{
  Serial.begin(115200);	  // Debugging only
  if (!driver.init()) {
    Serial.println("init failed");
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop()
{
  const char* on  = "ON";
  const char* off = "OFF";

  driver.send((uint8_t *)on, strlen(on));
  driver.waitPacketSent();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  
  driver.send((uint8_t *)off, strlen(off));
  driver.waitPacketSent();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
