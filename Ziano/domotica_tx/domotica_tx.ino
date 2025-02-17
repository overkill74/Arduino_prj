// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

//-------------------------------------------------------------------------
//TX 1
#define   PIN_T1_DATA  11

#define   PIN_IN1  2
#define   PIN_IN2  3
#define   PIN_IN3  4



static RH_ASK m_driver_t1(2000, 5, PIN_T1_DATA, 0);


static void led_pulse(uint32_t ms=100)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(ms);
  digitalWrite(LED_BUILTIN, LOW);
}

static void send_packet(const char* payload)
{
  String t1 = "";
  t1.concat(payload);
  Serial.println(t1);
  m_driver_t1.send((uint8_t*)t1.c_str(), t1.length());
  led_pulse(100);
  m_driver_t1.waitPacketSent();
}

void setup()
{
  Serial.begin(115200);    // Debugging only
  Serial.println("Welcome  to TX demo");

  if (!m_driver_t1.init()) {
    Serial.println("init m_driver_t1 failed");
    while (1) {
      led_pulse(250);
      delay(250);
    }
  }
  
  Serial.println("Setup LED");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(PIN_IN1, INPUT);
  pinMode(PIN_IN2, INPUT);
  pinMode(PIN_IN3, INPUT);
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, HIGH);
}

void loop()
{
  int in_1 = digitalRead(PIN_IN1);
  int in_2 = digitalRead(PIN_IN2);
  int in_3 = digitalRead(PIN_IN3);

  if (!in_1) {
    send_packet("UNO");
  }
  if (!in_2) {
    send_packet("DUE");
  }
  if (!in_3) {
    send_packet("TRE");
  }
}
