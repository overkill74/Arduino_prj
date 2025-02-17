// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

//-------------------------------------------------------------------------
//RX
#define   PIN_RX_GND   12
#define   PIN_RX_DATA  11
#define   PIN_RX_DATAX 10
#define   PIN_RX_VCC   9


//RH_ASK driver;
RH_ASK m_driver_rx;//(2000, PIN_RX_DATA, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2


static void led_pulse(uint32_t ms=100)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(ms);
  digitalWrite(LED_BUILTIN, LOW);
}


void setup()
{
  Serial.begin(115200);  // Debugging only
  Serial.println("\n\nStarting receiver");
  if (!m_driver_rx.init()) {
    Serial.println("init failed");
    while (1) {
      led_pulse(250);
      delay(250);
    }
  }

  Serial.println("Setup LED");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Power
  Serial.println("Setup GND");
  pinMode(PIN_RX_GND, OUTPUT);
  pinMode(PIN_RX_DATAX, INPUT);
  digitalWrite(PIN_RX_GND, LOW);

  Serial.println("Setup VCC");
  pinMode(PIN_RX_VCC, OUTPUT);
  digitalWrite(PIN_RX_VCC, HIGH);
}

void loop()
{
  //Serial.println("loop");
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf) - 1;
  memset(buf, 0, sizeof(buf));

    static uint32_t timeout_1 = 0;
    static uint32_t timeout_2 = 0;
    static uint32_t timeout_3 = 0;
    static int cnt = 0;


  if (m_driver_rx.recv(buf, &buflen)) // Non-blocking
  {
    int i;

    Serial.println((char*)buf);

    static int last_rx = -1;
    int rx=-1;
    
    if (!strcmp((char * )buf, "UNO")) {
      if (!timeout_1) {
        ++cnt;
      }
      timeout_1 = millis() + 500;
    }
    if (!strcmp((char * )buf, "DUE")) {
      rx = 2;
    }
    if (!strcmp((char * )buf, "TRE")) {
      rx = 3;
    }

    // Message with a good checksum received, dump it.
    //driver.printBuffer("Got: ", buf, buflen);
  }

  if (millis() > timeout_1) {
    timeout_1 = 0;
  }
  if (millis() > timeout_2) {
    timeout_2 = 0;
  }
  if (millis() > timeout_3) {
    timeout_3 = 0;
  }

  digitalWrite(LED_BUILTIN, cnt & 1);
}
