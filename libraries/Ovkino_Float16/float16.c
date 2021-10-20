////////////////////////////////////////////////////////////////////////
/// @file   float16.c
/// @author Ivan Zoli
/// @date   mar 14 aprile 2015
///
/// @brief
///
////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <float16.h>

//-------------------------------------------------------------------------
union Bits
{
  float     f;
  int32_t   si;
  uint32_t  ui;
};
typedef union Bits Bits;

//-------------------------------------------------------------------------
struct s_f16_conv
{
  int shift;
  int shiftSign;

  int32_t infN;  // flt32 infinity
  int32_t maxN;  // max flt16 normal as a flt32
  int32_t minN;  // min flt16 normal as a flt32
  int32_t signN; // flt32 sign bit

  int32_t infC;
  int32_t nanN;  // minimum flt16 nan as a flt32
  int32_t maxC;
  int32_t minC;
  int32_t signC; // flt16 sign bit

  int32_t mulN;  // (1 << 23) / minN
  int32_t mulC;  // minN / (1 << (23 - shift))

  int32_t subC;  // max flt32 subnormal down shifted
  int32_t norC;  // min flt32 normal down shifted

  int32_t maxD;
  int32_t minD;
};
typedef struct s_f16_conv s_f16_conv;

//-------------------------------------------------------------------------
static const s_f16_conv m_this =
{
  0x0000000d,
  0x00000010,
  0x7f800000,
  0x477fe000,
  0x38800000,
  0x80000000,
  0x0003fc00,
  0x7f802000,
  0x00023bff,
  0x0001c400,
  0xffff8000,
  0x52000000,
  0x33800000,
  0x000003ff,
  0x00000400,
  0x0001c000,
  0x0001c000
};

//-------------------------------------------------------------------------
_f16 floatToF16(float value)
{
  Bits v, s;
  uint32_t sign;

  v.f = value;
  sign = v.si & m_this.signN;
  v.si ^= sign;
  sign >>= m_this.shiftSign; // logical shift
  s.si = m_this.mulN;
  s.si = (int32_t)(s.f * v.f); // correct subnormals
  v.si ^= (s.si ^ v.si) & -(m_this.minN > v.si);
  v.si ^= (m_this.infN ^ v.si) & -((m_this.infN > v.si) & (v.si > m_this.maxN));
  v.si ^= (m_this.nanN ^ v.si) & -((m_this.nanN > v.si) & (v.si > m_this.infN));
  v.ui >>= m_this.shift; // logical shift
  v.si ^= ((v.si - m_this.maxD) ^ v.si) & -(v.si > m_this.maxC);
  v.si ^= ((v.si - m_this.minD) ^ v.si) & -(v.si > m_this.subC);

  return (_f16)((v.ui | sign) & 0xFFFF);
}

//-------------------------------------------------------------------------
float f16ToFloat(_f16 value)
{
  Bits v, s;
  int32_t sign;
  int32_t mask;

  v.ui = value;
  sign = v.si & m_this.signC;
  v.si ^= sign;
  sign <<= m_this.shiftSign;
  v.si ^= ((v.si + m_this.minD) ^ v.si) & -(v.si > m_this.subC);
  v.si ^= ((v.si + m_this.maxD) ^ v.si) & -(v.si > m_this.maxC);

  s.si = m_this.mulC;
  s.f *= v.si;

  mask = -(m_this.norC > v.si);
  v.si <<= m_this.shift;
  v.si ^= (s.si ^ v.si) & mask;
  v.si |= sign;

  return v.f;
}

/** EMACS **
 * Local variables:
 * mode: c++
 * End:
 */
