/*! \file
 *
 *  \brief  Additions to the stdfix.h file to support full Draft ISO/IEC
 *   standards compliance.
 *
 *  \author Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 *
 *  DETAILS
 *    Created on       : 12 December 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#ifndef __STDFIX_FULL_ISO_H__
#define __STDFIX_FULL_ISO_H__

#include <stdint.h>
#include <stdbool.h>
#include "arm_acle.h"

//! \brief Returns the minimum of two arguments.
//! \param[in] a First argument
//! \param[in] b Second argument
//! \return The minimum of a and b.
#ifndef min
#define min(a, b)	(((a)<(b)) ? (a) : (b))
#endif

//! \brief Returns the maximum of two arguments.
//! \param[in] a First argument
//! \param[in] b Second argument
//! \return The maximum of a and b.

#define __stdfix_max(a, b) \
    (((a)<(b)) ? (b) : (a))

//! \brief An macro definition which pretends to use variable a.
//! \param[in] a First argument

// note that this is a use of the semicolon swallowing as defined in:
// https://gcc.gnu.org/onlinedocs/cpp/Swallowing-the-Semicolon.html
#define __stdfix_use(a) \
    do {} while ((a)!=(a))

//! \brief Returns the absolute value of its argument.
//! \param[in] a First argument.
//! \return The absolute value of a.

#define __stdfix_abs(a) \
    (((a)<0) ? -(a) : (a))

//! \brief Returns the most significant 32-bits of a 64-bit argument.
//! \param[in] x A 64-bit value.
//! \return The most significant 32-bits of x.

#define __stdfix_ms_u32(x) \
    ((x) >> 32)

//! \brief Returns the least significant 32-bits of a 64-bit argument.
//! \param[in] x A 64-bit value.
//! \return The least significant 32-bits of x.

#define __stdfix_ls_u32(x) \
    ((x) & UINT32_MAX)

//! \brief Multiplies two 64-bit unsigned integers returning a 128-bit result
//! in hi and lo.
//! \param[out] hi First argument
//! \param[out] lo Second argument
//! \param[in] x First argument
//! \param[in] y Second argument
//! \return [hi:lo] = x*y

void __stdfix_64x64_128(uint64_t *hi, uint64_t *lo, uint64_t x, uint64_t y);

//! \brief This is the integer type into which signed short fract can be
//! converted.

typedef int8_t  int_hr_t;

//! \brief This is the integer type into which signed fract can be converted.

typedef int16_t int_r_t;

//! \brief This is the integer type into which signed long fract can be
//! converted.

typedef int32_t int_lr_t;

//! \brief This is the integer type into which signed short accum can be
//! converted.

typedef int16_t int_hk_t;

//! \brief This is the integer type into which signed accum can be converted.

typedef int32_t int_k_t;

//! \brief This is the integer type into which signed long accum can be
//! converted.

typedef int64_t int_lk_t;

//! \brief This is the unsigned integer type into which unsigned short accum
//! can be converted.

typedef uint8_t  uint_uhr_t;

//! \brief This is the unsigned integer type into which unsigned fract can be
//! converted.

typedef uint16_t uint_ur_t;

//! \brief This is the unsigned integer type into which unsigned long fract can
//! be converted.

typedef uint32_t uint_ulr_t;

//! \brief This is the unsigned integer type into which unsigned short accum
//! can be converted.

typedef uint16_t uint_uhk_t;

//! \brief This is the unsigned integer type into which unsigned accum can be
//! converted.

typedef uint32_t uint_uk_t;

//! \brief This is the unsigned integer type into which unsigned long accum can
//! be converted.

typedef uint64_t uint_ulk_t;

#ifdef __arm__
#include <stdfix.h>

//! \brief An alternative name for the signed short fract type.

typedef short fract s07;

//! \brief An alternative name for the signed fract type.

typedef fract s015;

//! \brief An alternative name for the signed long fract type.

typedef long fract s031;

//! \brief An alternative name for the signed short accum type.

typedef short accum s87;

//! \brief An alternative name for the signed accum type.

typedef accum s1615;

//! \brief An alternative name for the signed long accum type.

typedef long accum s3231;

//! \brief An alternative name for the unsigned short fract type.

typedef unsigned short fract u08;

//! \brief An alternative name for the unsigned fract type.

typedef unsigned fract u016;

//! \brief An alternative name for the unsigned long fract type.

typedef unsigned long fract u032;

//! \brief An alternative name for the unsigned short accum type.

typedef unsigned short accum u88;

//! \brief An alternative name for the unsigned accum type.

typedef unsigned accum u1616;

//! \brief An alternative name for the unsigned long accum type.

typedef unsigned long accum u3232;

#else /* ! __arm__ */

typedef int8_t s07;
typedef int16_t s015;
typedef int32_t s031;
typedef int16_t s87;
typedef int32_t s1615;
typedef int64_t s3231;

typedef uint8_t u08;
typedef uint16_t u016;
typedef uint32_t u032;
typedef uint16_t u88;
typedef uint32_t u1616;
typedef uint64_t u3232;

#endif /* __arm__ */

// 7.18a.6.5 The bitwise fixed-point to integer conversion functions

//! \brief Converts a short fract into an integer.
//! \param[in] f A short fract
//! \return The integer representation.

static inline int_hr_t bitshr(
	const s07 f)
{
    union { int_hr_t r; s07 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a fract into an integer.
//! \param[in] f A fract
//! \return The integer representation.

static inline int_r_t bitsr(
	const s015 f)
{
    union { int_r_t r; s015 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a long fract into an integer.
//! \param[in] f A long fract
//! \return The integer representation.

static inline int_lr_t bitslr(
	const s031 f)
{
    union { int_lr_t r; s031 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a short accum into an integer.
//! \param[in] f A short accum
//! \return The integer representation.

static inline int_hk_t bitshk(
	const s87 f)
{
    union { int_hk_t r; s87 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a accum into an integer.
//! \param[in] f A accum
//! \return The integer representation.

static inline int_k_t bitsk(
	const s1615 f)
{
    union { int_k_t r; s1615 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a long accum into an integer.
//! \param[in] f A long accum
//! \return The integer representation.

static inline int_lk_t bitslk(
	const s3231 f)
{
    union { int_lk_t r; s3231 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts an unsigned  short fract into an integer.
//! \param[in] f An unsigned short fract
//! \return The integer representation.

static inline uint_uhr_t bitsuhr(
	const u08 f)
{
    union { uint_uhr_t r; u08 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts a unsigned fract into an integer.
//! \param[in] f An unsigned fract
//! \return The integer representation.

static inline uint_ur_t bitsur(
	const u016 f)
{
    union { uint_ur_t r; u016 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts an unsigned long fract into an integer.
//! \param[in] f An unsigned long fract
//! \return The integer representation.

static inline uint_ulr_t bitsulr(
	const u032 f)
{
    union { uint_ulr_t r; u032 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts an unsigned short accum into an integer.
//! \param[in] f An unsigned short accum
//! \return The integer representation.

static inline uint_uhk_t bitsuhk(
	const u88 f)
{
    union { uint_uhk_t r; u88 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts an unsigned accum  into an integer.
//! \param[in] f An unsigned accum
//! \return The integer representation.

static inline uint_uk_t bitsuk(
	const u1616 f)
{
    union { uint_uk_t r; u1616 fx; } x;

    x.fx = f;
    return x.r;
}

//! \brief Converts an unsigned long accum into an integer.
//! \param[in] f An unsigned long accum
//! \return The integer representation.

static inline uint_ulk_t bitsulk(
	const u3232 f)
{
    union { uint_ulk_t r; u3232 fx; } x;

    x.fx = f;
    return x.r;
}

// 7.18a.6.6 The bitwise integer to fixed-point conversion functions

//! \brief Converts an signed integer into an signed short fract.
//! \param[in] n An signed integer
//! \return The signed short fract represented by n.

static inline s07 hrbits(
	const int_hr_t n)
{
    union { int_hr_t r; s07 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an signed integer into an signed fract.
//! \param[in] n An signed integer
//! \return The signed fract represented by n.

static inline s015 rbits(
	const int_r_t n)
{
    union { int_r_t r; s015 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an signed integer into an signed long fract.
//! \param[in] n An signed integer
//! \return The signed long fract represented by n.

static inline s031 lrbits(
	const int_lr_t n)
{
    union { int_lr_t r; s031 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an signed integer into an signed short accum.
//! \param[in] n An signed integer
//! \return The signed short accum represented by n.

static inline s87 hkbits(
	const int_hk_t n)
{
    union { int_hk_t r; s87 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an signed integer into an signed accum.
//! \param[in] n An signed integer
//! \return The signed accum represented by n.

static inline s1615 kbits(
	const int_k_t n)
{
    union { int_k_t r; s1615 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an signed integer into an signed long accum.
//! \param[in] n An signed integer
//! \return The signed long accum represented by n.

static inline s3231 lkbits(
	const int_lk_t n)
{
    union { int_lk_t r; s3231 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned short fract.
//! \param[in] n An unsigned integer
//! \return The unsigned short fract represented by n.

static inline u08 uhrbits(
	const uint_uhr_t n)
{
    union { uint_uhr_t r; u08 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned fract.
//! \param[in] n An unsigned integer
//! \return The unsigned fract represented by n.

static inline u016 urbits(
	const uint_ur_t n)
{
    union { uint_ur_t r; u016 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned long fract.
//! \param[in] n An unsigned integer
//! \return The unsigned long fract represented by n.

static inline u032 ulrbits(
	const uint_ulr_t n)
{
    union { uint_ulr_t r; u032 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned short accum.
//! \param[in] n An unsigned integer
//! \return The unsigned short accum represented by n.

static inline u88 uhkbits(
	const uint_uhk_t n)
{
    union { uint_uhk_t r; u88 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned accum.
//! \param[in] n An unsigned integer
//! \return The unsigned accum represented by n.

static inline u1616 ukbits(
	const uint_uk_t  n)
{
    union { uint_uk_t  r; u1616 fx; } x;

    x.r = n;
    return x.fx;
}

//! \brief Converts an unsigned integer into an unsigned long accum.
//! \param[in] n An unsigned integer
//! \return The unsigned long accum represented by n.

static inline u3232 ulkbits(
	const uint_ulk_t n)
{
    union { uint_ulk_t r; u3232 fx; } x;

    x.r = n;
    return x.fx;
}

// Wrapped casts
#define __I(x)    ((int) (x))
#define __I32(x)  ((int32_t) (x))
#define __I64(x)  ((int64_t) (x))
#define __LI(x)   ((long int) (x))
#define __U32(x)  ((uint32_t) (x))
#define __U64(x)  ((uint64_t) (x))
#define __UI(x)   ((unsigned int) (x))
#define __ULI(x)  ((unsigned long int) (x))

// Saturation operations

//! \brief Saturates an integer to 8 bits.
//! \param[in] x An integer
//! \return The value of x saturated to 8 bits.

static inline int32_t __stdfix_sat_hr(
	const int32_t x)
{
    if (x > INT8_MAX) {
	return INT8_MAX;
    }
    if (x < INT8_MIN) {
	return INT8_MIN;
    }

    return x;
}

//! \brief Saturates an integer to 16 bits.
//! \param[in] x An integer
//! \return The value of x saturated to 16 bits.

static inline int32_t __stdfix_sat_r(
	const int32_t x)
{
    if (x > INT16_MAX) {
	return INT16_MAX;
    }
    if (x < INT16_MIN) {
	return INT16_MIN;
    }

    return x;
}

//! \brief Saturates a 64-bit integer to 32-bits.
//! \param[in] x An integer
//! \return The value of x saturated to 32-bits.

static inline int32_t __stdfix_sat_lr(
	const int64_t x)
{
    if (x > INT32_MAX) {
	return INT32_MAX;
    }
    if (x < INT32_MIN) {
	return INT32_MIN;
    }

    return __I32(x);
}

//! \brief Saturates an integer to 16 bits.
//! \param[in] x An integer
//! \return The value of x saturated to 16 bits.

static inline int32_t __stdfix_sat_hk(
	const int32_t x)
{
    if (x > INT16_MAX) {
	return INT16_MAX;
    }
    if (x < INT16_MIN) {
	return INT16_MIN;
    }

    return x;
}

//! \brief Saturates a 64-bit integer to 32-bits.
//! \param[in] x An integer
//! \return The value of x saturated to 32-bits.

static inline int32_t __stdfix_sat_k(
	const int64_t x)
{
    if (x > INT32_MAX) {
	return INT32_MAX;
    }
    if (x < INT32_MIN) {
	return INT32_MIN;
    }

    return __I32(x);
}

//! \brief Saturates an unsigned integer to 8 bits.
//! \param[in] x An unsigned integer
//! \return The value of x saturated to 8 bits.

static inline uint32_t __stdfix_sat_uhr(
	const uint32_t x)
{
    if (x > UINT8_MAX) {
	return UINT8_MAX;
    }

    return x;
}

//! \brief Saturates an unsigned integer to 16 bits.
//! \param[in] x An unsigned integer
//! \return The value of x saturated to 16 bits.

static inline uint32_t __stdfix_sat_ur(
	const uint32_t x)
{
    if (x > UINT16_MAX) {
	return UINT16_MAX;
    }

    return x;
}

//! \brief Saturates an unsigned 64-bit integer to 32 bits.
//! \param[in] x An unsigned integer
//! \return The value of x saturated to 32 bits.

static inline uint32_t __stdfix_sat_ulr(
	const uint64_t x)
{
    if (x > UINT32_MAX) {
	return UINT32_MAX;
    }

    return __U32(x);
}

//! \brief Saturates an unsigned integer to 16 bits.
//! \param[in] x An unsigned integer
//! \return The value of x saturated to 16 bits.

static inline uint32_t __stdfix_sat_uhk(
	const uint32_t x)
{
    if (x > UINT16_MAX) {
	return UINT16_MAX;
    }

    return x;
}

//! \brief Saturates an unsigned 64-bit integer to 32 bits.
//! \param[in] x An unsigned integer
//! \return The value of x saturated to 32 bits.

static inline uint32_t __stdfix_sat_uk(
	const uint64_t x)
{
    if (x > UINT32_MAX) {
	return UINT32_MAX;
    }

    return __U32(x);
}

// software simulation of basic saturating arithmetic

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 8-bit integer
//! \param[in] y An 8-bit integer
//! \return The value of x+y saturated to 8 bits.

static inline int32_t __stdfix_sadd_hr(
	int_hr_t x,
	int_hr_t y)
{
    return __stdfix_sat_hr(__I32(x) + __I32(y));
}

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 8-bit integer
//! \param[in] y An 8-bit integer
//! \return The value of x-y saturated to 8 bits.

static inline int32_t __stdfix_ssub_hr(
	int_hr_t x,
	int_hr_t y)
{
    return __stdfix_sat_hr(__I32(x) - __I32(y));
}

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 8-bit integer
//! \return The value of -x saturated to 8 bits.

static inline int32_t __stdfix_sneg_hr(
	int_hr_t x)
{
    return __stdfix_sat_hr(__I32(-x));
}

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \param[in] y An 16-bit integer
//! \return The value of x+y saturated to 16 bits.

static inline int32_t __stdfix_sadd_r(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_r(__I32(x) + __I32(y));
}

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \param[in] y An 16-bit integer
//! \return The value of x-y saturated to 16 bits.

static inline int32_t __stdfix_ssub_r(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_r(__I32(x) - __I32(y));
}

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \return The value of -x saturated to 16 bits.

static inline int32_t __stdfix_sneg_r(
	int32_t x)
{
    return __stdfix_sat_r(__I32(-x));
}

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \param[in] y An 32-bit integer
//! \return The value of x+y saturated to 32 bits.

static inline int32_t __stdfix_sadd_lr(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_lr(__I64(x) + __I64(y));
}

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \param[in] y An 32-bit integer
//! \return The value of x-y saturated to 32 bits.

static inline int32_t __stdfix_ssub_lr(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_lr(__I64(x) - __I64(y));
}

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \return The value of -x saturated to 32 bits.

static inline int32_t __stdfix_sneg_lr(
	int32_t x)
{
    return __stdfix_sat_lr(__I64(-x));
}

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \param[in] y An 16-bit integer
//! \return The value of x+y saturated to 16 bits.

static inline int32_t __stdfix_sadd_hk(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_hk(__I32(x) + __I32(y));
}

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \param[in] y An 16-bit integer
//! \return The value of x-y saturated to 16 bits.

static inline int32_t __stdfix_ssub_hk(
	int32_t x,
	int32_t y)
{
    return __stdfix_sat_hk(__I32(x) - __I32(y));
}

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 16-bit integer
//! \return The value of -x saturated to 16 bits.

static inline int32_t __stdfix_sneg_hk(
	int32_t x)
{
    return __stdfix_sat_hk(__I32(-x));
}

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \param[in] y An 32-bit integer
//! \return The value of x+y saturated to 32 bits.

static inline int32_t __stdfix_sadd_k(
	int32_t x,
	int32_t y)
{
#ifdef   __ARM_FEATURE_DSP
    return __qadd(x, y);
#else  /*__ARM_FEATURE_DSP*/
    return __stdfix_sat_k(__I64(x) + __I64(y));
#endif /*__ARM_FEATURE_DSP*/
}

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \param[in] y An 32-bit integer
//! \return The value of x-y saturated to 32 bits.

static inline int32_t __stdfix_ssub_k(
	int32_t x,
	int32_t y)
{
#ifdef   __ARM_FEATURE_DSP
    return __qsub(x, y);
#else  /*__ARM_FEATURE_DSP*/
    return __stdfix_sat_k(__I64(x) - __I64(y));
#endif /*__ARM_FEATURE_DSP*/
}

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 32-bit integer
//! \return The value of -x saturated to 32 bits.

static inline int32_t __stdfix_sneg_k(
	int32_t x)
{
#ifdef   __ARM_FEATURE_DSP
    return __qsub(0, x);
#else  /*__ARM_FEATURE_DSP*/
    return __stdfix_sat_k(__I64(-x));
#endif /*__ARM_FEATURE_DSP*/
}

//! \brief Saturated addition of the underlying integer representations
//! \param[in] x An 64-bit integer
//! \param[in] y An 64-bit integer
//! \return The value of x+y saturated to 64 bits.

int64_t __stdfix_sadd_lk(int64_t x, int64_t y);

//! \brief Saturated subtraction of the underlying integer representations
//! \param[in] x An 64-bit integer
//! \param[in] y An 64-bit integer
//! \return The value of x-y saturated to 64 bits.

int64_t __stdfix_ssub_lk(int64_t x, int64_t y);

//! \brief Saturated negation of the underlying integer representations
//! \param[in] x An 64-bit integer
//! \return The value of -x saturated to 64 bits.

int64_t __stdfix_sneg_lk(int64_t x);

//! \brief Saturated multiplication of the underlying integer representations
//! \param[in] x A 32-bit integer representing of a short fract.
//! \param[in] y A 32-bit integer representing of a short fract.
//! \return The value of x*y saturated to 8 bits representing of a short fract.

static inline int32_t __stdfix_smul_hr(
	int32_t x,
	int32_t y)
{
    if (x == INT8_MIN && y == INT8_MIN) { // special case for -1.0*-1.0
	return INT8_MAX;
    }

    return __I32(__stdfix_sat_hr((__I32(x) * __I32(y)) >> 7));
}

//! \brief Saturated multiplication of the underlying integer representations
//! \param[in] x A 32-bit integer representing of a fract.
//! \param[in] y A 32-bit integer representing of a fract.
//! \return The value of x*y saturated to 16 bits representing of a fract.

static inline int32_t __stdfix_smul_r(
	int32_t x,
	int32_t y)
{
    if (x == INT16_MIN && y == INT16_MIN) { // special case for -1.0*-1.0
	return INT16_MAX;
    }

    return __I32(__stdfix_sat_r((__I32(x) * __I32(y)) >> 15));
}

//! \brief Saturated multiplication of the underlying integer representations
//! \param[in] x A 32-bit integer representing of a long fract.
//! \param[in] y A 32-bit integer representing of a long fract.
//! \return The value of x*y saturated to 32 bits representing of a long fract.

static inline int32_t __stdfix_smul_lr(
	int32_t x,
	int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
	return INT32_MAX;
    }

    return __stdfix_sat_lr((__I64(x) * __I64(y)) >> 31);
}

//! \brief Saturated multiplication of the underlying integer representations
//! \param[in] x A 32-bit integer representing of a short accum.
//! \param[in] y A 32-bit integer representing of a short accum.
//! \return The value of x*y saturated to 16 bits representing of a short accum.

static inline int32_t __stdfix_smul_hk(
	int32_t x,
	int32_t y)
{
    if (x == INT16_MIN && y == INT16_MIN) { // special case for -1.0*-1.0
	return INT16_MAX;
    }

    return __stdfix_sat_hk((__I32(x) * __I32(y)) >> 7);
}

//! \brief Saturated multiplicaion of the underlying integer representations
//! \param[in] x A 32-bit integer representing of a accum.
//! \param[in] y A 32-bit integer representing of a accum.
//! \return The value of x*y saturated to 32 bits representing of a accum.

static inline int32_t __stdfix_smul_k(
	int32_t x,
	int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
	return INT32_MAX;
    }

    return __stdfix_sat_k((__I64(x) * __I64(y)) >> 15);
}

//! \brief Saturated multiplicaion of the underlying integer representations
//! \param[in] x A 64-bit integer representing of a long accum.
//! \param[in] y A 64-bit integer representing of a long accum.
//! \return The value of x*y saturated to 64 bits representing of a long accum.

int64_t __stdfix_smul_lk(int64_t x, int64_t y);

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x+y saturated to 8 bits.

static inline uint32_t __stdfix_sadd_uhr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhr(__U32(x) + __U32(y));
}

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x An 32-bit unsigned integer
//! \param[in] y An 32-bit unsigned integer
//! \return The value of x-y saturated to 8 bits.

static inline uint32_t __stdfix_ssub_uhr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhr(__U32(x) - __U32(y));
}

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x+y saturated to 16 bits.

static inline uint32_t __stdfix_sadd_ur(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ur(__U32(x) + __U32(y));
}

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x-y saturated to 16 bits.

static inline uint32_t __stdfix_ssub_ur(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ur(__U32(x) - __U32(y));
}

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x+y saturated to 32 bits.

static inline uint32_t __stdfix_sadd_ulr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ulr(__U64(x) + __U64(y));
}

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x-y saturated to 32 bits.

static inline uint32_t __stdfix_ssub_ulr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ulr(__U64(x) - __U64(y));
}

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x+y saturated to 16 bits.

static inline uint32_t __stdfix_sadd_uhk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhk(__U32(x) + __U32(y));
}

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x-y saturated to 16 bits.

static inline uint32_t __stdfix_ssub_uhk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhk(__U32(x) - __U32(y));
}

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x+y saturated to 32 bits.

static inline uint32_t __stdfix_sadd_uk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uk(__U64(x) + __U64(y));
}

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer
//! \param[in] y A 32-bit unsigned integer
//! \return The value of x-y saturated to 32 bits.

static inline uint32_t __stdfix_ssub_uk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uk(__U64(x) - __U64(y));
}

//! \brief Saturated addition of the underlying unsigned integer representations
//! \param[in] x A 64-bit unsigned integer
//! \param[in] y A 64-bit unsigned integer
//! \return The value of x+y saturated to 64 bits.

uint64_t __stdfix_sadd_ulk(uint64_t x, uint64_t y);

//! \brief Saturated subtraction of the underlying unsigned integer representations
//! \param[in] x A 64-bit unsigned integer
//! \param[in] y A 64-bit unsigned integer
//! \return The value of x-y saturated to 64 bits.

uint64_t __stdfix_ssub_ulk(uint64_t x, uint64_t y);

//! \brief Saturated multiplication of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer representing of an unsigned short fract.
//! \param[in] y A 32-bit unsigned integer representing of an unsigned short fract.
//! \return The value of x*y saturated to 8 bits representing of an unsigned short fract.

static inline uint32_t __stdfix_smul_uhr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhr((__U32(x) * __U32(y)) >> 8);
}

//! \brief Saturated multiplicaion of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer representing of an unsigned fract.
//! \param[in] y A 32-bit unsigned integer representing of an unsigned fract.
//! \return The value of x*y saturated to 16 bits representing of an unsigned fract.

static inline uint32_t __stdfix_smul_ur(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ur((__U32(x) * __U32(y)) >> 16);
}

//! \brief Saturated multiplicaion of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer representing of an unsigned long fract.
//! \param[in] y A 32-bit unsigned integer representing of an unsigned long fract.
//! \return The value of x*y saturated to 32 bits representing of an unsigned long fract.

static inline uint32_t __stdfix_smul_ulr(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_ulr((__U64(x) * __U64(y)) >> 32);
}

//! \brief Saturated multiplicaion of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer representing of an unsigned short accum.
//! \param[in] y A 32-bit unsigned integer representing of an unsigned short accum.
//! \return The value of x*y saturated to 16 bits representing of an unsigned short accum.

static inline uint32_t __stdfix_smul_uhk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uhk((__U32(x) * __U32(y)) >> 8);
}

//! \brief Saturated multiplicaion of the underlying unsigned integer representations
//! \param[in] x A 32-bit unsigned integer representing of an unsigned accum.
//! \param[in] y A 32-bit unsigned integer representing of an unsigned accum.
//! \return The value of x*y saturated to 32 bits representing of an unsigned accum.

static inline uint32_t __stdfix_smul_uk(
	uint32_t x,
	uint32_t y)
{
    return __stdfix_sat_uk((__U64(x) * __U64(y)) >> 16);
}

//! \brief Saturated multiplicaion of the underlying unsigned integer representations
//! \param[in] x A 64-bit unsigned integer representing of an unsigned long accum.
//! \param[in] y A 64-bit unsigned integer representing of an unsigned long accum.
//! \return The value of x*y saturated to 64 bits representing of an unsigned long accum.

uint64_t __stdfix_smul_ulk(uint64_t x, uint64_t y);

// 7.18a.6.1 The fixed-point arithmetic operation support functions Synopsis

//! \brief Multiplies an integer by a fract giving an integer answer.
//! \param[in] n An integer
//! \param[in] x An s0.15 fract
//! \return The integer part of n*x.

static inline int mulir(
	int n,
	s015 x)
{
    return __I((__I64(n) * __I64(bitsr(x))) >> 15);
}

//! \brief Multiplies a long integer by a long fract giving an integer answer.
//! \param[in] n A long integer
//! \param[in] x An s0.31 fract
//! \return The integer part of n*x.

static inline long int mulilr(
	long int n,
	s031 x)
{
    return __LI((__I64(n) * __I64(bitslr(x))) >> 31);
}

//! \brief Multiplies an integer by an accum giving an integer answer.
//! \param[in] n An integer
//! \param[in] x An s16.15 accum
//! \return The integer part of n*x.

static inline int mulik(
	int n,
	s1615 x)
{
    return __I((__I64(n) * __I64(bitsk(x))) >> 15);
}

//! \brief Multiplies a long integer by a long accum giving a long integer answer.
//! \param[in] n A long integer
//! \param[in] x An s32.31 accum
//! \return The integer part of n*x.

static inline long int mulilk(
	long int n,
	s3231 x)
{
    int64_t r = bitslk(x);
    int64_t k = __I64(n) * (r >> 31);
    int64_t c = (__I64(n) * (r & INT64_C(0x7FFFFFF))) >> 31;

    return __LI(k + c);
}

//! \brief Divides an integer by a fract giving an integer answer.
//! \param[in] n An integer
//! \param[in] x An s0.15 fract
//! \return The integer part of n/x.

static inline int divir(
	int n,
	s015 x)
{
    return __I((__I64(n) << 15) / bitsr(x));
}

//! \brief Divides a long integer by a long fract giving a long integer answer.
//! \param[in] n A long integer
//! \param[in] x An s0.31 fract
//! \return The integer part of n/x.

static inline long int divilr(
	long int n,
	s031 x)
{
    return __LI((__I64(n) << 31) / bitslr(x));
}

//! \brief Divides an integer by an accum giving an integer answer.
//! \param[in] n An integer
//! \param[in] x An s16.15 accum
//! \return The integer part of n/x.

static inline int divik(
	int n,
	s1615 x)
{
    return __LI((__I64(n) << 15) / bitsk(x));
}

//! \brief Divides an integer by a long accum giving a long integer answer.
//! \param[in] n A long integer
//! \param[in] x An s32.31 accum
//! \return The integer part of n/x.

static inline long int divilk(
	long int n,
	s3231 x)
{
    return __LI((__I64(n) << 31) / bitslk(x));
}

//! \brief Divides two integers giving a fract answer.
//! \param[in] i An integer
//! \param[in] j An integer
//! \return a fract representing i/j.

static inline s015 rdivi(
	int i,
	int j)
{
    return rbits((int_r_t) ((__I64(i) << 15) / __I64(j)));
}

//! \brief Divides two integers giving a long fract answer.
//! \param[in] i An integer
//! \param[in] j An integer
//! \return a long fract representing i/j.

static inline s031 lrdivi(
	long int i,
	long int j)
{
    return lrbits((int_lr_t) ((__I64(i) << 31) / __I64(j)));
}

//! \brief Divides two integers giving an accum answer.
//! \param[in] i An integer
//! \param[in] j An integer
//! \return an accum representing i/j.

static inline s1615 kdivi(
	int i,
	int j)
{
    return kbits((int_k_t) ((__I64(i) << 15) / __I64(j)));
}

//! \brief Divides two integers giving a long accum answer.
//! \param[in] i An integer
//! \param[in] j An integer
//! \return a long accum representing i/j.

static inline s3231 lkdivi(
	long int i,
	long int j)
{
    return lkbits((int_lk_t) ((__I64(i) << 31) / __I64(j)));
}

//! \brief Divides two fracts giving an integer answer.
//! \param[in] x A fract
//! \param[in] y A fract
//! \return An integer representing the integer part of x/y.

static inline int idivr(
	s015 x,
	s015 y)
{
    return __I(rbits(x) / rbits(y));
}

//! \brief Divides two long fracts giving a long integer answer.
//! \param[in] x A long fract
//! \param[in] y A long fract
//! \return A long integer representing the integer part of x/y.

static inline long int idivlr(
	s031 x,
	s031 y)
{
    return __LI(lrbits(x) / lrbits(y));
}

//! \brief Divides two accums giving an integer answer.
//! \param[in] x An accum
//! \param[in] y An accum
//! \return An integer representing the integer part of x/y.

static inline int idivk(
	s1615 x,
	s1615 y)
{
    return __I(kbits(x) / kbits(y));
}

//! \brief Divides two long accums giving a long integer answer.
//! \param[in] x A long accum
//! \param[in] y A long accum
//! \return A long integer representing the integer part of x/y.

static inline long int idivlk(
	s3231 x,
	s3231 y)
{
    return __LI(lkbits(x) / lkbits(y));
}

//! \brief Multiplies an unsigned integer by an unsigned fract giving an
//! unsigned integer answer.
//! \param[in] n An unsigned integer
//! \param[in] x An unsigned fract
//! \return An unsigned integer representing the integer part of n*x.

static inline unsigned int muliur(
	unsigned int n,
	u016 x)
{
    return __UI((__U64(n) * __U64(bitsur(x))) >> 16);
}

//! \brief Multiplies an unsigned long integer by an unsigned long fract giving
//! an unsigned integer answer.
//! \param[in] n An unsigned long integer
//! \param[in] x An unsigned long fract
//! \return An unsigned long integer representing the integer part of n*x.

static inline unsigned long int muliulr(
	unsigned long int n,
	u032 x)
{
    return __ULI((__U64(n) * __U64(bitsulr(x))) >> 32);
}

//! \brief Multiplies an unsigned integer by an unsigned accum giving an
//! unsigned integer answer.
//! \param[in] n An unsigned integer
//! \param[in] x An unsigned accum
//! \return An unsigned integer representing the integer part of n*x.

static inline unsigned int muliuk(
	unsigned int n,
	u1616 x)
{
    return __UI((__U64(n) << 16) / bitsuk(x));
}

//! \brief Multiplies an unsigned long integer by an unsigned long accum giving
//! an unsigned long integer answer.
//! \param[in] n An unsigned long integer
//! \param[in] x An unsigned long accum
//! \return An unsigned long integer representing the integer part of n*x.

static inline unsigned long int muliulk(
	unsigned long int n,
	u3232 x)
{
    uint64_t r = bitsulk(x);
    uint64_t k = __U64(n) * (r >> 32);
    uint64_t c = (__U64(n) * (r & INT64_C(0xFFFFFFF))) >> 32;

    return __ULI(k + c);
}

//! \brief Divides an unsigned integer by an unsigned fract giving an unsigned
//! integer answer.
//! \param[in] n An unsigned integer
//! \param[in] x An unsigned fract
//! \return An unsigned integer representing the integer part of n/x.

static inline unsigned int diviur(
	unsigned int n,
	u016 x)
{
    return __UI((__U64(n) << 16) / bitsur(x));
}

//! \brief Divides a long unsigned integer by a long unsigned fract giving an
//! unsigned integer answer.
//! \param[in] n A long unsigned integer
//! \param[in] x A long unsigned fract
//! \return A long unsigned integer representing the integer part of n/x.

static inline unsigned long int diviulr(
	unsigned long int n,
	u032 x)
{
    return __ULI((__U64(n) << 32) / bitsulr(x));
}

//! \brief Divides an unsigned integer by an unsigned accum giving an unsigned
//! integer answer.
//! \param[in] n An unsigned integer
//! \param[in] x An unsigned accum
//! \return An unsigned integer representing the integer part of n/x.

static inline unsigned int diviuk(
	unsigned int n,
	u1616 x)
{
    return __ULI((__U64(n) << 16) / bitsuk(x));
}

//! \brief Divides a long unsigned integer by a long unsigned accum giving an
//! unsigned integer answer.
//! \param[in] n A long unsigned integer
//! \param[in] x A long unsigned accum
//! \return A long unsigned integer representing the integer part of n/x.

static inline unsigned long int diviulk(
	unsigned long int n,
	u3232 x)
{
    return __ULI((__U64(n) << 32) / bitsulk(x));
}

//! \brief Divides two unsigned integers giving an unsigned fract answer.
//! \param[in] i An unsigned integer
//! \param[in] j An unsigned integer
//! \return An unsigned fract representing i/j.

static inline u016 urdivi(
	unsigned int i,
	unsigned int j)
{
    return ulrbits((uint_ulr_t) ((__U64(i) << 16) / __U64(j)));
}

//! \brief Divides two unsigned long integers giving an unsigned long fract
//! answer.
//! \param[in] i An unsigned long integer
//! \param[in] j An unsigned long integer
//! \return An unsigned long fract representing i/j.

static inline u032 ulrdivi(
	unsigned long int i,
	unsigned long int j)
{
    return ulrbits((uint_ulr_t) ((__U64(i) << 32) / __U64(j)));
}

//! \brief Divides two unsigned integers giving an unsigned accum answer.
//! \param[in] i An unsigned integer
//! \param[in] j An unsigned integer
//! \return An unsigned accum representing i/j.

static inline u1616 ukdivi(
	unsigned int i,
	unsigned int j)
{
    return ukbits((uint_uk_t) ((__U64(i) << 16) / __U64(j)));
}

//! \brief Divides two unsigned long integers giving an unsigned long accum
//! answer.
//! \param[in] i An unsigned long integer
//! \param[in] j An unsigned long integer
//! \return An unsigned long accum representing i/j.

static inline u3232 ulkdivi(
	unsigned long int i,
	unsigned long int j)
{
    return ulkbits((uint_ulk_t) ((__U64(i) << 32) / __U64(j)));
}

//! \brief Divides two unsigned fracts giving an unsigned integer answer.
//! \param[in] x An unsigned fract
//! \param[in] y An unsigned fract
//! \return An unsigned integer representing the integer part of x/y.

static inline unsigned int idivur(
	u016 x,
	u016 y)
{
    return __UI(urbits(x) / urbits(y));
}

//! \brief Divides two unsigned long fracts giving an unsigned long integer
//! answer.
//! \param[in] x An unsigned long fract
//! \param[in] y An unsigned long fract
//! \return An unsigned long integer representing the integer part of x/y.

static inline unsigned long int idivulr(
	u032 x,
	u032 y)
{
    return __ULI(ulrbits(x) / ulrbits(y));
}

//! \brief Divides two unsigned accums giving an unsigned integer answer.
//! \param[in] x An unsigned accum
//! \param[in] y An unsigned accum
//! \return An unsigned integer representing the integer part of x/y.

static inline unsigned int idivuk(
	u1616 x,
	u1616 y)
{
    return __UI(ukbits(x) / ukbits(y));
}

//! \brief Divides two unsigned long accums giving an unsigned long integer answer.
//! \param[in] x An unsigned long accum
//! \param[in] y An unsigned long accum
//! \return An unsigned long integer representing the integer part of x/y.

static inline unsigned long int idivulk(
	u3232 x,
	u3232 y)
{
    return __ULI(ulkbits(x) / ulkbits(y));
}

// Description
//
// The above functions compute the mathematically exact result of the
// multiplication or division operation on the operands with the indicated
// types, and return a value with the indicated type.
//
// Returns
//
// For functions returning an integer value, the return value is rounded
// towards zero. For functions returning a fixed-point value, the return value
// is saturated on overflow. If the second operand of one of the divide
// functions is zero, the behavior is undefined.

// 7.18a.6.2 The fixed-point absolute value functions

//! \brief This function returns the absolute value of a short fract.
//! \param[in] f A short fract.
//! \return The absolute value of f.

static inline s07 abshr(
	s07 f)
{
    int_hr_t r = bitshr(f);

    if (r < 0) {
	r = (r == INT8_MIN) ? INT8_MAX : -r;
    }

    return hrbits(r);
}

//! \brief This function returns the absolute value of a fract.
//! \param[in] f A fract.
//! \return The absolute value of f.

static inline s015 absr(
	s015 f)
{
    int_r_t r = bitsr(f);

    if (r < 0) {
	r = (r == INT16_MIN) ? INT16_MAX : -r;
    }

    return rbits(r);
}

//! \brief This function returns the absolute value of a long fract.
//! \param[in] f A long fract.
//! \return The absolute value of f.

static inline s031 abslr(
	s031 f)
{
    int_lr_t r = bitslr(f);

    if (r < 0) {
	r = (r == INT32_MIN) ? INT32_MAX : -r;
    }

    return lrbits(r);
}

//! \brief This function returns the absolute value of a short accum.
//! \param[in] f A short accum.
//! \return The absolute value of f.

static inline s87 abshk(
	s87 f)
{
    int_hk_t r = bitshk(f);

    if (r < 0) {
	r = (r == INT16_MIN) ? INT16_MAX : -r;
    }

    return hkbits(r);
}

//! \brief This function returns the absolute value of an accum.
//! \param[in] f An accum.
//! \return The absolute value of f.

static inline s1615 absk(
	s1615 f)
{
    int_k_t r = bitsk(f);

    if (r < 0) {
	r = (r == INT32_MIN) ? INT32_MAX : -r;
    }

    return kbits(r);
}

//! \brief This function returns the absolute value of a long accum.
//! \param[in] f A long accum.
//! \return The absolute value of f.

static inline s3231 abslk(
	s3231 f)
{
    int_lk_t r = bitslk(f);

    if (r < 0) {
	r = (r == INT64_MIN) ? INT64_MAX : -r;
    }

    return lkbits(r);
}

// Description
//
// The above functions compute the absolute value of a fixed-point value f.
//
// Returns
//
// The functions return |f|. If the exact result cannot be represented, the
// saturated result is returned.

// 7.18a.6.3 The fixed-point rounding functions

//! \brief This function rounds the input unsigned 8-bit integer to a number
//! of bits, returning an 8-bit integer.
//! \param[in] f An 8-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline uint8_t __stdfix_round_u8(
	uint8_t x,
	int n)
{
    register uint8_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input unsigned 16-bit integer to a number
//! of bits, returning a 16-bit integer.
//! \param[in] f An 16-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline uint16_t __stdfix_round_u16(
	uint16_t x,
	int n)
{
    register uint16_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input unsigned 32-bit integer to a number
//! of bits, returning an 32-bit integer.
//! \param[in] f An 32-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline uint32_t __stdfix_round_u32(
	uint32_t x,
	int n)
{
    register uint32_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input unsigned 64-bit integer to a number
//! of bits, returning a 64-bit integer.
//! \param[in] f A 64-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline uint64_t __stdfix_round_u64(
	uint64_t x,
	int n)
{
    register uint64_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input signed 8-bit integer to a number of
//! bits, returning an 8-bit integer.
//! \param[in] f An 8-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline int8_t __stdfix_round_s8(
	int8_t x,
	int n)
{
    register int8_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input signed 16-bit integer to a number
//! of bits, returning a 16-bit integer.
//! \param[in] f An 16-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline int16_t __stdfix_round_s16(
	int16_t x,
	int n)
{
    register int16_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input signed 32-bit integer to a number
//! of bits, returning an 32-bit integer.
//! \param[in] f An 32-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline int32_t __stdfix_round_s32(
	int32_t x,
	int n)
{
    register int32_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input signed 64-bit integer to a number
//! of bits, returning a 64-bit integer.
//! \param[in] f A 64-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline int64_t __stdfix_round_s64(
	int64_t x,
	int n)
{
    register int64_t r, c;

    c = (x >> (n-1)) & 0x1;
    r = x >> n;
    r = (r + c) << n;

    return r;
}

//! \brief This function rounds the input short fract to a number of bits,
//! returning a short fract.
//! \param[in] f A short fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s07 roundhr(
	s07 f,
	int n)
{
    return hrbits(__stdfix_round_s8(bitshr(f), n));
}

//! \brief This function rounds the input fract to a number of bits,
//! returning a fract.
//! \param[in] f A fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s015 roundr(
	s015 f,
	int n)
{
    return rbits(__stdfix_round_s16(bitsr(f), n));
}

//! \brief This function rounds the input long fract to a number of bits,
//! returning a long fract.
//! \param[in] f A long fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s031 roundlr(
	s031 f,
	int n)
{
    return lrbits(__stdfix_round_s32(bitslr(f), n));
}

//! \brief This function rounds the input short accum to a number of bits,
//! returning a short accum.
//! \param[in] f A short accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s87 roundhk(
	s87 f,
	int n)
{
    return hkbits(__stdfix_round_s16(bitshk(f), n));
}

//! \brief This function rounds the input accum to a number of bits,
//! returning a accum.
//! \param[in] f A accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s1615 roundk(
	s1615 f,
	int n)
{
    return kbits(__stdfix_round_s32(bitsk(f), n));
}

//! \brief This function rounds the input long accum to a number of bits,
//! returning a long accum.
//! \param[in] f A long accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline s3231 roundlk(
	s3231 f,
	int n)
{
    return lkbits(__stdfix_round_s64(bitslk(f), n));
}

//! \brief This function rounds the input unsigned short fract to a number of
//! bits, returning an unsigned short fract.
//! \param[in] f An unsigned short fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u08 rounduhr(
	u08 f,
	int n)
{
    return uhrbits(__stdfix_round_u8(bitsuhr(f), n));
}

//! \brief This function rounds the input unsigned fract to a number of
//! bits, returning an unsigned fract.
//! \param[in] f An unsigned fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u016 roundur(
	u016 f,
	int n)
{
    return urbits(__stdfix_round_u16(bitsur(f), n));
}

//! \brief This function rounds the input unsigned long fract to a number of
//! bits, returning an unsigned long fract.
//! \param[in] f An unsigned long fract.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u032 roundulr(
	u032 f,
	int n)
{
    return ulrbits(__stdfix_round_u32(bitsulr(f), n));
}

//! \brief This function rounds the input unsigned short accum to a number of
//! bits, returning an unsigned short accum.
//! \param[in] f An unsigned short accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u88 rounduhk(
	u88 f,
	int n)
{
    return uhkbits(__stdfix_round_u16(bitsuhk(f), n));
}

//! \brief This function rounds the input unsigned accum to a number of bits,
//! returning a unsigned accum.
//! \param[in] f An unsigned accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u1616 rounduk(
	u1616 f,
	int n)
{
    return ukbits(__stdfix_round_u32(bitsuk(f), n));
}

//! \brief This function rounds the input unsigned long accum to a number of
//! bits, returning an unsigned long accum.
//! \param[in] f An unsigned long accum.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits.

static inline u3232 roundulk(
	u3232 f,
	int n)
{
    return ulkbits(__stdfix_round_u64(bitsulk(f), n));
}

// Description
//
// The above functions compute the value of f, rounded to the number of
// fractional bits specified in n. The rounding applied is to-nearest, with
// unspecified rounding direction in the halfway case. When saturation has not
// occurred, fractional bits beyond the rounding point are set to zero in the
// result. The value of n must be nonnegative and less than the number of
// fractional bits in the fixed-point type of f.
//
// Returns
//
// The rounding functions return the rounded result, as specified. If the
// value of n is negative or larger than the number of fractional bits in the
// fixed-point type of f, the result is unspecified. If the exact result
// cannot be represented, the saturated result is returned.

// 7.18a.6.4 The fixed-point countls functions

//! \brief This function counts the number of leading sign-bits of a
//! short fract.
//! \param[in] f A short fract.
//! \return The number of leading sign-bits in f.

static inline int countlshr(
	s07 f)
{
    int32_t n = __I32(bitshr(f));

    if (n == 0) {
	return 7;
    }

    return __builtin_clrsb(n) - 24;
}

//! \brief This function counts the number of leading sign-bits of a fract.
//! \param[in] f A fract.
//! \return The number of leading sign-bits in f.

static inline int countlsr(
	s015 f)
{
    int32_t n = __I32(bitsr(f));

    if (n == 0) {
	return 15;
    }

    return __builtin_clrsb(n) - 16;
}

//! \brief This function counts the number of leading sign-bits of a
//! long fract.
//! \param[in] f A long fract.
//! \return The number of leading sign-bits in f.

static inline int countlslr(
	s031 f)
{
    int_lr_t n = bitslr(f);

    if (n == 0) {
	return 31;
    }

    return __builtin_clrsb(n);
}

//! \brief This function counts the number of leading sign-bits of a short
//! accum.
//! \param[in] f A short accum.
//! \return The number of leading sign-bits in f.

static inline int countlshk(
	s87 f)
{
    int32_t n = __I32(bitshk(f));

    if (n == 0) {
	return 15;
    }

    return __builtin_clrsb(n) - 16;
}

//! \brief This function counts the number of leading sign-bits of an accum.
//! \param[in] f An accum.
//! \return The number of leading sign-bits in f.

static inline int countlsk(
	s1615 f)
{
    int_k_t n = bitsk(f);

    if (n == 0) {
	return 31;
    }

    return __builtin_clrsb(n);
}

//! \brief This function counts the number of leading sign-bits of a long
//! accum.
//! \param[in] f A long accum.
//! \return The number of leading sign-bits in f.

static inline int countlslk(
	s3231 f)
{
    int64_t n = __I64(bitslk(f));

    if (n == 0) {
	return 63;
    }

    return __builtin_clrsbll(n);
}

//! \brief This function counts the number of leading zeros of an unsigned
//! short fract.
//! \param[in] f An unsigned short fract.
//! \return The number of leading zeros in f.

static inline int countlsuhr(
	u08 f)
{
    uint32_t n = __U32(bitsuhr(f));

    if (n == 0) {
	return 8;
    }

    return __builtin_clz(n) - 24;
}

//! \brief This function counts the number of leading zeros of an unsigned
//! fract.
//! \param[in] f An unsigned fract.
//! \return The number of leading zeros in f.

static inline int countlsur(
	u016 f)
{
    uint32_t n = __U32(bitsur(f));

    if (n == 0) {
	return 16;
    }

    return __builtin_clz(n) - 16;
}

//! \brief This function counts the number of leading zeros of an unsigned
//! long fract.
//! \param[in] f An unsigned long fract.
//! \return The number of leading zeros in f.

static inline int countlsulr(
	u032 f)
{
    uint_ulr_t n = bitsulr(f);

    if (n == 0) {
	return 32;
    }

    return __builtin_clz(n);
}

//! \brief This function counts the number of leading zeros of an unsigned
//! short accum.
//! \param[in] f An unsigned short accum.
//! \return The number of leading zeros in f.

static inline int countlsuhk(
	u88 f)
{
    uint32_t n = __U32(bitsuhk(f));

    if (n == 0) {
	return 16;
    }

    return __builtin_clz(n) - 16;
}

//! \brief This function counts the number of leading zeros of an unsigned
//! accum.
//! \param[in] f An unsigned accum.
//! \return The number of leading zeros in f.

static inline int countlsuk(
	u1616 f)
{
    uint_uk_t n = (uint_uk_t) bitsuk(f);

    if (n == 0) {
	return 32;
    }

    return __builtin_clz(n);
}

//! \brief This function counts the number of leading zeros of an unsigned long
//! accum.
//! \param[in] f An unsigned long accum.
//! \return The number of leading zeros in f.

static inline int countlsulk(
	u3232 f)
{
    uint_ulk_t n = (uint_ulk_t) bitsulk(f);

    if (n == 0) {
	return 64;
    }

    return __builtin_clzll(n);
}

// Description
//
// The integer return value of the above functions is defined as follows:
// - if the value of the fixed-point argument f is non-zero, the return value
//   is the largest integer k for which the expression f<<k does not overflow;
// - if the value of the fixed-point argument is zero, an integer value is
//   returned that is at least as large as N, where N is the total number of
//   value bits of the fixed-point type of the argument.
//
// Note: if the value of the fixed-point argument is zero, the recommended
// return value is exactly N.
//
// Returns
//
// The countls functions return the integer value as indicated.

// 7.18a.6.7 Type-generic fixed-point functions

// For each of the fixed-point absolute value functions in 7.18a.6.2, the
// fixed-point rounding functions in 7.18a.6.3 and the fixed-point countls
// functions in 7.18a.6.4, a type-generic macro is defined as follows:
//
//                                                   type-generic macro
//      the fixed-point absolute value functions     absfx
//      the fixed-point rounding functions           roundfx
//      the fixed-point countls functions            countlsfx
//
// For each macro, use of the macro invokes the function whose corresponding
// type and type domain is the fixed-point type of the first generic argument.
// If the type of the first generic argument is not a fixed-point type, the
// behavior is undefined

//! \brief This macro provides type-generic access to the absolute value
//! functions.
//! \param[in] f A signed fixed-point value.
//! \return The absolute value of f (with the same type as f).

#define absfx(f) \
    ({									\
	__typeof__(f) tmp = (f);					\
	if (__builtin_types_compatible_p(__typeof__(f), s07)) {		\
	    tmp = abshr(tmp);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s015)) {	\
	    tmp = absr(tmp);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s031)) {	\
	    tmp = abslr(tmp);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s87)) {	\
	    tmp = abshk(tmp);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s1615)) { \
	    tmp = absk(tmp);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s3231)) { \
	    tmp = abslk(tmp);						\
	} else {							\
	    abort();							\
	} 								\
	tmp;								\
    })

//! \brief This macro provides type-generic access to the rounding
//! functions.
//! \param[in] f A fixed-point value.
//! \param[in] n The number of bits to be rounded off.
//! \return The rounded value of f(with the same type as f).

#define roundfx(f, n) \
    ({									\
	__typeof__(f) tmp = (f);					\
	if (__builtin_types_compatible_p(__typeof__(f), s07)) {		\
	    tmp = roundhr(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), s015)) {	\
	    tmp = roundr(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), s031)) {	\
	    tmp = roundlr(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), s87)) {	\
	    tmp = roundhk(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), s1615)) { \
	    tmp = roundk(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), s3231)) { \
	    tmp = roundlk(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u08)) {	\
	    tmp = rounduhr(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u016)) {	\
	    tmp = roundur(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u032)) {	\
	    tmp = roundulr(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u88)) {	\
	    tmp = rounduhk(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u1616)) { \
	    tmp = rounduk(tmp, n);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u3232)) { \
	    tmp = roundulk(tmp, n);					\
	} else {							\
	    abort();							\
	}								\
	tmp;								\
    })

//! \brief This macro provides type-generic access to the functions for
//! counting leading sign-bits.
//! \param[in] f A fixed-point value.
//! \return The number of leading sign-bits in f.

#define countlsfx(f) \
    ({									\
	int tmp = 0;							\
	if (__builtin_types_compatible_p(__typeof__(f), s07)) {		\
	    tmp = countlshr(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s015)) {	\
	    tmp = countlsr(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s031)) {	\
	    tmp = countlslr(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s87)) {	\
	    tmp = countlshk(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s1615)) { \
	    tmp = countlsk(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), s3231)) { \
	    tmp = countlslk(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), u08)) {	\
	    tmp = countlsuhr(f);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u016)) {	\
	    tmp = countlsur(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), u032)) {	\
	    tmp = countlsulr(f);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u88)) {	\
	    tmp = countlsuhk(f);					\
	} else if (__builtin_types_compatible_p(__typeof__(f), u1616)) { \
	    tmp = countlsuk(f);						\
	} else if (__builtin_types_compatible_p(__typeof__(f), u3232)) { \
	    tmp = countlsulk(f);					\
	} else {							\
	    abort();							\
	}								\
	tmp;								\
    })

// 7.18a.6.8 Numeric conversion functions

#ifndef __arm__
s07   strtofxhr (const char *restrict nptr, char **restrict endptr);
s015  strtofxr  (const char *restrict nptr, char **restrict endptr);
s031  strtofxlr (const char *restrict nptr, char **restrict endptr);
s87   strtofxhk (const char *restrict nptr, char **restrict endptr);
s1615 strtofxk  (const char *restrict nptr, char **restrict endptr);
s3231 strtofxlk (const char *restrict nptr, char **restrict endptr);

u08   strtofxuhr(const char *restrict nptr, char **restrict endptr);
u016  strtofxur (const char *restrict nptr, char **restrict endptr);
u032  strtofxulr(const char *restrict nptr, char **restrict endptr);
u88   strtofxuhk(const char *restrict nptr, char **restrict endptr);
u1616 strtofxuk (const char *restrict nptr, char **restrict endptr);
u3232 strtofxulk(const char *restrict nptr, char **restrict endptr);
#endif /*__arm__*/

// Description
//
// The strtofxhr, strtofxr, strtofxlr, strtofxhk, strtofxk, strtofxlk,
// strtofxuhr, strtofxur, strtofxulr, strtofxuhk, strtofxuk and strtofxulk
// functions convert the initial portion of the string pointed to by nptr to
// short fract, fract, long fract, short accum, accum, long accum, unsigned
// short fract, unsigned fract, unsigned long fract, unsigned short accum,
// unsigned accum, and unsigned long accum representation, respectively.
// First,they decompose the input string into three parts: an initial,
// possibly empty, sequence of white-space characters (as specified by the is
// space function), a subject sequence resembling a fixed-point constant; and
// a final string of one or more unrecognized characters, including the
// terminating null character of the input string. Then, they attempt to
// convert the subject sequence to a fixed-point number, and return the
// result.
//
// The expected form of the subject sequence is an optional plus or minus
// sign, then one of the following:
//
// - a nonempty sequence of decimal digits optionally containing a
//   decimal-point character, then an optional exponent part as defined in
//   6.4.4.3;
//
// - a 0x or 0X, then a nonempty sequence of hexadecimal digits optionally
//   containing a decimal- point character, then an optional binary exponent
//   part as defined in 6.4.4.3.
//
// The subject sequence is defined as the longest initial subsequence of the
// input string, starting with the first non-white-space character, that is of
// the expected form. The subject sequence contains no characters if the input
// string is not of the expected form.
//
// If the subject sequence has the expected form for a fixed-point number, the
// sequence of characters starting with the first digit or the decimal-point
// character (whichever occurs first) is interpreted as a fixed-point constant
// according to the rules of 6.4.4.3, except that the decimal-point character
// is used in place of a period, and that if neither an exponent part nor a
// decimal-point character appears in a decimal fixed-point number, or if a
// binary exponent part does not appear in a hexadecimal fixed- point number,
// an exponent part of the appropriate type with value zero is assumed to
// follow the last digit in the string. If the subject sequence begins with a
// minus sign, the sequence is interpreted as negated. A pointer to the final
// string is stored in the object pointed to by endptr, provided that endptr
// is not a null pointer.
//
// The value resulting from the conversion is rounded as necessary in an
// implementation-defined manner.
//
// In other than the "C" locale, additional locale-specific subject sequence
// forms may be accepted.
//
// If the subject sequence is empty or does not have the expected form, no
// conversion is performed; the value of nptr is stored in the object pointed
// to by endptr, provided that endptr is not a null pointer.
//
// Returns
//
// The functions return the converted value, if any. If no conversion could be
// performed, zero is returned. If the correct value is outside the range of
// representable values, a saturated result is returned (according to the
// return type and sign of the value), and the value of the macro ERANGE
// is stored in errno.

#endif /* __STDFIX_FULL_ISO_H__ */
