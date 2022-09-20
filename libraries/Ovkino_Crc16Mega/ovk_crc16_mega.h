/////////////////////////////////////////////////////////////////////////////
///
/// CRC
///
/// \author     Ivan Zoli
/// \copyright  2022 All right reserved
///
//////////////////////////////////////////////////////////////////////////////


#ifndef OVK_CRC_H
#define OVK_CRC_H

//////////////////////////////////////////////////////////////////////////////
//                         I N C L U D E S                                  //
//////////////////////////////////////////////////////////////////////////////

// System Library Include
#include <stdint.h>

// User Library Include

// Application Local Include

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                 F O R W A R D   D E C L A R A T I O N S                  //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//                 C O N S T A N T S   D E F I N I T I O N S                //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//                    C L A S S    D E C L A R A T I O N                    //
//////////////////////////////////////////////////////////////////////////////

///
/// \brief reflect
/// \param v
/// \return reflectd input
///
template<typename t>
t OvkReflect(t v)
{
  t r = 0;

  for (int i = 0; i < (8 * sizeof v); ++i)
    {
      r <<= 1;
      r |= v&1;
      v >>= 1;
    }

  return r;
}

///
/// \brief The OvkCrc16 class
///
class OvkCrc16
{
public:
  // OvkCrc16(const vector<uint16_t>& table, bool reflec_in, bool reflect_out);
  OvkCrc16(const uint16_t& polynominal, bool reflec_in, bool reflect_out);

  ///
  /// \brief calculateCrc
  /// \param in_data
  /// \param begin_crc
  /// \return CRC16
  ///
  uint16_t calculateCrc(const uint8_t* in_data, int in_data_sz, uint16_t begin_crc=0);

protected:
  ///
  /// \brief init_table
  /// \param polynominal
  ///
  void init_table(const uint16_t& polynominal);
  ///
  /// \brief init_table
  /// \param polynominal
  ///
  void clear_table();
  ///
  /// \brief calc_rev_simple
  /// \param in_data
  /// \param begin_crc
  /// \return CRC16
  ///
  uint16_t calc_rev_simple(const uint8_t* in_data, int in_data_sz, uint16_t begin_crc=0);

protected:
  uint16_t    m_table[256];               //! LockUp Table
  uint16_t    m_poly=0;                   //! Polynominal
  bool        m_is_in_reflect = false;    //! Input is reflected
  bool        m_is_out_reflect = false;   //! Output is reflected
};

#endif


/** EMACS **
 * Local variables:
 * mode: c++
 * End:
 */
