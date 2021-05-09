// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2

void setup()
{
  Serial.begin(115200);	// Debugging only
  Serial.println("Starting receiver");
  if (!driver.init()) {
    Serial.println("init failed");
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf) - 1;
  memset(buf, 0, sizeof(buf));

  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;

    if (!strcmp(buf, "ON")) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (!strcmp(buf, "OFF")) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      Serial.println((char*)buf);
    }
    // Message with a good checksum received, dump it.
    //driver.printBuffer("Got: ", buf, buflen);
  }
}
