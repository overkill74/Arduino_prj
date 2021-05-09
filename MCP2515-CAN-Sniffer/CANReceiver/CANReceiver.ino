// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("CAN Receiver");

  // start the CAN bus at 250 kbps
  int err = CAN.begin(250E3);
  if (err < 0) {
    Serial.print("Starting CAN failed: ");
    Serial.println(err);
    CAN.dumpRegisters(Serial);
    while (1);
  }

}

void loop()
{
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    if (CAN.packetExtended()) {
      Serial.print("ext ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" req len ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" len ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print(CAN.read(), HEX);
        Serial.print(' ');
      }
      Serial.println();
    }

    Serial.println();
  }
}
