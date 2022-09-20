//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

#if 0
# include "Arduino.h"
#endif

// Application Local Include
#include "ovk_crc16_mega.h"

// User Library Include

// System Library Include

//////////////////////////////////////////////////////////////////////////////
//                           F U N C T I O N S                              //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// OvkCrc16::OvkCrc16(const vector<uint16_t>& table, bool reflec_in, bool reflect_out)
//   : m_is_in_reflect(reflec_in)
//   , m_is_out_reflect(reflect_out)
// {

//   for (int x=0; x<256; ++x) {
//     m_table[x] =
//   }
// }

//////////////////////////////////////////////////////////////////////////////
OvkCrc16::OvkCrc16(const uint16_t& polynominal, bool reflec_in, bool reflect_out)
  : m_poly(polynominal), m_is_in_reflect(reflec_in), m_is_out_reflect(reflect_out)
  {
  init_table(polynominal);
}

//////////////////////////////////////////////////////////////////////////////
void OvkCrc16::clear_table()
{
  for (int x = 0; x < 256; ++x) {
    m_table[x] = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////
void OvkCrc16::init_table(const uint16_t& polynominal)
{
  clear_table();

  const int table_size = 256;

  if (!m_is_in_reflect)
  {
    for (int divident = 0; divident < table_size; ++divident)
    {
      uint16_t curr_byte = static_cast<uint16_t>(divident << 8);
      for (uint8_t bit = 0; bit < 8; ++bit) {
        if ((curr_byte & 0x8000) != 0) {
          curr_byte <<= 1;
          curr_byte ^= polynominal;
        }
        else {
          curr_byte <<= 1;
        }
      }

      m_table[divident] = curr_byte;
    }
  }
  else
  {
    uint16_t poly = OvkReflect<uint16_t>(polynominal);
    for (int divident = 0; divident < table_size; ++divident)
    {
      uint16_t curr_byte = static_cast<uint16_t>(divident);
      for (uint8_t bit = 0; bit < 8; ++bit) {
        if ((curr_byte & 0x0001) != 0) {
          curr_byte >>= 1;
          curr_byte ^= poly;
        }
        else {
          curr_byte >>= 1;
        }
      }
      m_table[divident] = curr_byte;
    }
  }
#if 0
  int cnt=0;
  Serial.println("\n----------------------------------------------------");
  for (int x=0; x<256; ++x) {
    char tmp[16];
    sprintf(tmp, "0x%04X ", m_table[x]);
    Serial.print(tmp);
    // Serial.print("0x");
    // Serial.print(m_table[x], HEX);
    // Serial.print(" ");
    if (++cnt == 16) {
        cnt = 0;
        Serial.println("");
    }

  }
  Serial.println("\n----------------------------------------------------\n");
#endif

}

//////////////////////////////////////////////////////////////////////////////
uint16_t OvkCrc16::calculateCrc(const uint8_t* in_data, int in_data_sz, uint16_t begin_crc)
{
  uint16_t crc = begin_crc;
  if (!m_is_in_reflect) {
    for (int x=0; x<in_data_sz; ++x) {
      uint8_t pos = static_cast<uint8_t>((crc >> 8) ^ static_cast<uint16_t>(in_data[x]));
      crc = static_cast<uint16_t>((crc << 8) ^ static_cast<uint16_t>(m_table[pos]));
    }
  }
  else {
    for (int x=0; x<in_data_sz; ++x) {
      uint8_t cur_byte = static_cast<uint8_t>(static_cast<uint16_t>(in_data[x]) ^ crc);
      crc = m_table[cur_byte] ^ (crc >> 8);
    }
    return m_is_out_reflect ? crc : OvkReflect<uint16_t>(crc);
  }

  return m_is_out_reflect ? OvkReflect<uint16_t>(crc) : crc;
}

//////////////////////////////////////////////////////////////////////////////
uint16_t OvkCrc16::calc_rev_simple(const uint8_t* in_data, int in_data_sz, uint16_t begin_crc)
{
  uint16_t crc = begin_crc;
 
  for (int x=0; x<in_data_sz; ++x) {
    uint8_t cur_byte = (uint8_t)((uint16_t)in_data[x] ^ crc);
    crc = m_table[cur_byte] ^ (crc >> 8);
  }

  return crc;
}