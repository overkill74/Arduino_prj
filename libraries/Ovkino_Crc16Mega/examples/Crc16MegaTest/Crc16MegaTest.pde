#include "ovk_crc16_mega.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("CRC16 Test");
}

void loop()
{
  OvkCrc16 m_crc_1(0x1021, true, true);
  OvkCrc16 m_crc_2(0x1021, false, false);
  
  // put your main code here, to run repeatedly:
  uint8_t test_1[] = {0x06, 0x00, 0x10, 0x12, 0x34, 0x56, 0xBE, 0x09, 0x78, 0x03, 0x01,
                      0x06, 0x06, 0x26, 0x04, 0x0A, 0x02, 0x06, 0xD3, 0x03}; //, 0x78, 0xB5};
  uint8_t test_2[] = {0x06, 0x0F, 0x0A, 0x19, 0x00, 0x29, 0x28, 0x90, 0x00, 0x42, 0x49, 0x09, 0x78, 0x03};

  uint16_t crc_test_1_1 = m_crc_1.calculateCrc(test_1, sizeof(test_1));
  uint16_t crc_test_2_1 = m_crc_1.calculateCrc(test_2, sizeof(test_2));
  uint16_t crc_test_1_2 = m_crc_2.calculateCrc(test_1, sizeof(test_1));
  uint16_t crc_test_2_2 = m_crc_2.calculateCrc(test_2, sizeof(test_2));

  Serial.print("Test 1 CRC 1: "); Serial.print(crc_test_1_1, HEX); Serial.println(crc_test_1_1 == 0xB578 ? " OK" : " FAIL!");
  Serial.print("Test 1 CRC 2: "); Serial.print(crc_test_1_2, HEX); Serial.println(crc_test_1_2 == 0x7720 ? " OK" : " FAIL!");
  Serial.print("Test 2 CRC 1: "); Serial.print(crc_test_2_1, HEX); Serial.println(crc_test_2_1 == 0x6E06 ? " OK" : " FAIL!");
  Serial.print("Test 2 CRC 2: "); Serial.print(crc_test_2_2, HEX); Serial.println(crc_test_2_2 == 0x839F ? " OK" : " FAIL!");

  while (1);

}

