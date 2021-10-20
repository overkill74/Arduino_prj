///////////////////////////////////////////////////////////////////////////
/// @file   float16.h
/// @author Ivan Zoli 
/// @date   mar 14 aprile 2015
///
/// @brief
///
///////////////////////////////////////////////////////////////////////////


#ifndef __FLOAT16_H_
#define __FLOAT16_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short _f16;

float f16ToFloat(_f16 value);
_f16 floatToF16(float value);


#ifdef __cplusplus
}
#endif

#endif // __FLOAT16_H_

/** EMACS **
 * Local variables:
 * mode: c++
 * End:
 */
