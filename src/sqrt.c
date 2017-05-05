/*! \file
 *
 *  \brief Implementation of exp for the unsigned accum type, returning a
 *  u0.12 result.
 *
 *  \details The details of the algorithm are from
 *     "Elementary Functions: Algorithms and Implemenation", 2nd edn,
 *     Jean-Michel Muller, Birkhauser, 2006.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester, Jamie Knight and The University of Manchester,
 *    2014.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 23 October, 2014
 *
 */

#include "stdfix-full-iso.h"
#include "polynomial.h"
#include "debug.h"
#include "pair.h"
#include "arm_acle.h"
#include "arm.h"

#define __SQRT_HALF UINT32_C(3037000500)

static inline int even(int x) {
    return (x & 1) == 0;
}
static inline int odd(int x) {
    return (x & 1) == 1;
}

//! \brief This function multiplies a 64 bit unsigned quantity, by a
//! u0.32.
//! \param[in] x A 64-bit unsigned quantity.
//! \param[in] y A 32-bit int representing an unsigned long fract (u0.32)
//! \return x * y >> 32

//static inline
uint64_t __x_u64_ulr(uint64_t x, uint32_t y)
{
    register uint64_t result;

    result = (x & UINT64_C(0xFFFFFFFF)) * ((uint64_t) y);
                                       // least significant multiplication.
    result = (result >> 32) + ((result & UINT64_C(0x80000000)) >> 31);
                                       // rounding of least significant bit
    result += (x >> 32) * ((uint64_t) y);

    return result;
}

//-----------------------------------------------------------------------
//
// Implementation notes
//
// The calculation is:
//
//          R' = R + (1 - X * R^2) * R/2
//
// Note that we can align X in the range [1,2), by left shifts, and that
// We need not retain the leading 1. Note also that the trailing bit
// must be 0. Therefore it is actually easier to retain X as an
// unsigned u1.31.
//
// Likewise, since R lies in the range (0.5,1], we may take R = 1 - r/2.
// Thus r is in [0,1), and is also a long unsigned fract.
//
// We have:
//
//   X * R = X * (1 - r/2)
//         = X - X * (r/2)
//
//   R^2   = 1 - r + (r/2)^2
//
//-----------------------------------------------------------------------

//! \brief This function calculates a the square of r.
//! \param[in] r The value to be squared (where r represents the
//! number R = 1-r/2^33, i.e. 2^31 represents 0.75.
//! \return A 64-bit representation of 1 - R^2

static inline uint64_t r_squared(uint32_t r)
{
    uint64_t t = (uint64_t)(r) << 32;

    t -= ((uint64_t)(r >> 1)) * ((uint64_t)(r >> 1));

    return t;
}

//! \brief This function calculates an improved value of r, the reciprocal
//! square-root.
//! \param[in] x The number to be square-rooted, in u1.31 format
//! \param[in] r The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * r in u1.63 format

static inline uint64_t newton_xr(uint32_t x, uint32_t r)
{
    register uint64_t t = ((uint64_t) x)*((uint64_t) r);

    t = ((uint64_t)(x) << 32) - (t >> 1);

    return t;
}

//! \brief This function calculates an improved value of r, the reciprocal
//! square-root.
//! \param[in] x The number to be square-rooted, in u1.31 format
//! \param[in] r The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * r in u1.63 format

static inline uint64_t newton_xlr(uint32_t x, uint64_t r)
{
    register uint64_t t = __x_u64_ulr(r, x);

    t = ((uint64_t)(x) << 32) - (t >> 1);

    return t;
}

//! \brief This function calculates an improved value of r, the reciprocal
//! square-root.
//! \param[in] x The number to be square-rooted, in u1.63 format
//! \param[in] r The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * R in u1.63 format

//static inline 
uint64_t newton_lxr(uint64_t x, uint32_t r)
{
    return x - __x_u64_ulr(x, r >> 1);
}

uint64_t newton_lxlr(uint64_t x, uint64_t r)
{
    register uint64_t result, tmp;

    r = r >> 1;

    result = (x & UINT64_C(0xFFFFFFFF)) * (r & UINT64_C(0xFFFFFFFF));
                                       // least significant multiplication.

    result = (x >> 32) * (r & UINT64_C(0xFFFFFFFF)) +
	    ((result & UINT64_C(0x80000000)) >> 31);
                                       // rounding of least significant bit

    tmp = (x & UINT64_C(0xFFFFFFFF)) * (r >> 32) +
	    (result & UINT64_C(0xFFFFFFFF));

    result = (result >> 32) + (tmp >> 32) +
	    ((tmp & UINT64_C(0x80000000)) >> 31);

    result += (x >> 32) * (r >> 32);

    return x - result;
}

//! \brief This function calculates an improved value of r, the reciprocal
//! square-root.
//! \param[in] x The number to be square-rooted, in u1.31 format
//! \param[in] r The current approximation R = 1 - r/2 (r is in u0.32)
//! \return (x * R^2 - 1) in u1.63 format

uint64_t newton_xr2(uint32_t x, uint32_t r)
{
    register uint64_t t;

    t = newton_xr(x, r);	// x*R   in u1.63 format
    t = newton_lxr(t, r);	// x*R^2 in u1.63 format
    t -= (UINT64_C(1) << 63);	// (x*R^2 - 1) in u1.63 format

    return t;
}

uint64_t newton_xlr2(uint32_t x, uint64_t r)
{
    register uint64_t t;

    t = newton_xlr(x, r);	// x*R   in u1.63 format
    t = newton_lxlr(t, r);	// x*R^2 in u1.63 format
    t -= (UINT64_C(1) << 63);	// (x*R^2 - 1) in u1.63 format

    return t;
}

//-----------------------------------------------------------------------
// We have:
//
// If R = 1 - r/2 (and R' = 1 - r'/2), and Z = X*R^2 - 1, then 
//
// R' = R + (1 - X * R^2) * R/2
//    = R - Z * R/2
//
// Thus:
//
// (1 - r'/2) = (1 - r/2) - Z * (1 - r/2)/2
//
// r' = r + Z * (1 - r/2)
//    = r + Z - Z * r/2
//
//-----------------------------------------------------------------------

uint64_t __attribute__((noinline)) square_root_improve(
	uint32_t x, uint32_t r)
{
    register union { uint64_t u; int64_t s;} tmp;
    register uint64_t t;
    register bool neg;

    tmp.u = newton_xr2(x, r);		// tmp.u has Z = (x*R^2 - 1) in s0.63 format.

    neg = (tmp.s < 0);
    if (neg) {
	tmp.s = -tmp.s;
    }

    t = __x_u64_ulr(tmp.u, r >> 1);	// t has |Z|*r/2 in s0.63 format

    tmp.u -= t;

    if (neg) {
	tmp.s = -tmp.s;
    }

    tmp.s <<= 1;

    tmp.u += (uint64_t)(r) << 32;

    return tmp.u;
}

uint64_t __attribute__((noinline)) square_root_ximprove(
	uint32_t x, uint64_t r)
{
    register union { uint64_t u; int64_t s;} tmp;
    register uint64_t t;
    register bool neg;

    //log_info("x = %u, r = (%u:%u)\n",
    //         x,
    //         (uint32_t)(r >> 32),
    //         (uint32_t)(r & UINT32_C(0xFFFFFFFF)));

    tmp.u = newton_xlr2(x, r);		// tmp.u has (x*r^2 - 1) in s0.63 format.

    neg = (tmp.s < 0);
    if (neg) {
	tmp.s = -tmp.s;
    }

    t = __x_u64_ulr(tmp.u, (uint32_t)(r >> 33));
    t += __x_u64_ulr(tmp.u, (uint32_t)((r >> 1) & UINT32_C(0xFFFFFFFF))) >> 32;
					// t has |Z|*r/2 in s0.63 format
    tmp.u -= t;

    if (neg) {
	tmp.s = -tmp.s;
    }

    tmp.s <<= 1;

    //log_info("tmp = (%u:%u)\n",
    //         (uint32_t)(tmp.u >> 32),
    //         (uint32_t)(tmp.u & UINT32_C(0xFFFFFFFF)));

    tmp.u += r;

    return tmp.u;
}

//! This function calculates the reciprocal square-root of the argument
//! \param[in] x An unsigned integer, representing a u1.31. Leading bit is 1.
//! \return An unsigned 64-bit integer representing the reciprocal square-root
//! of x, as a u0.64. INCORRECT FORMAT DOCUMENTATION. FIX THIS!

uint64_t __attribute__((noinline)) recip_normalized_root(uint32_t x)
{
    uint64_t result;
    uint32_t initial = (x & 0x7FFFFFFF) >> 1;

    assert(x > INT32_MAX);
    //log_info("x = (%u), initial = (%u)", x, initial, initial);

    result = square_root_improve(x, initial);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_improve(x, (uint32_t)(result >> 32));
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_improve(x, (uint32_t)(result >> 32));
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_ximprove(x, result);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));

    result = square_root_ximprove(x, result);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));

    // DRL HACK!!! The following result = -(result >> 1);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //         (uint32_t)(result & 0xFFFFFFFF));

    return result;
}

//! This function calculates the square-root of the argument
//! \param[in] x A signed integer, representing an s16.15 accum.
//! \return A signed integer representing the square-root of x, as an s16.15.

int32_t sqrtk_bits(int32_t x)
{
    uint64_t tmp;
    int32_t  n;
    union {uint32_t u; int32_t s;} r;

    assert(x >= 0);

    if ((x == 0) || x == 32768) {
        return x; // x is zero or one.
    }

    n = __builtin_clz(x);
    r.s = x;
    r.u = x << n;
		    // if x = 0.25, then n = 18
		    // if x == 1.0, then n = 16
		    // if x == 4.0, then n = 14
		    // if x == 2^14, then n = 16 - 14 = 2

    //log_info("x = %k (%d), normalised = %u, n = %d", x, x, r.u, n);

    tmp = recip_normalized_root(r.u);

    tmp = newton_xlr (r.u, tmp);
    tmp = tmp >> (16 + ((n - 16) >> 1));
		    // if x was 0.25, tmp = 1.0 (in u1.63), n = 17
		    // if x was 1.0, tmp = 1.0 (in u1.63), n = 16
		    // if x was 4.0, tmp = 1.0, n = 15
		    // if x was 2^14, tmp = 1.0, n = 16 + (n-16)/2 = 9

    if (odd(n)) {
        tmp = __x_u64_ulr(tmp, __SQRT_HALF);
    }

    return (int32_t) (tmp >> 32);
}

#define UNIMPLEMENTED __attribute__((deprecated("Not implemented")))

//! This function calculates the square-root of the argument
//! \param[in] x An unsigned integer, representing an u16.16 accum.
//! \return An unsigned integer representing the square-root of x,
//! as an u16.16.

uint32_t UNIMPLEMENTED sqrtuk_bits(uint32_t x)
{
    //uint64_t tmp;
    //uint32_t r, n = __builtin_clz(x);

    return x;
}

short fract UNIMPLEMENTED sqrthr(short fract x)
{
    return x;
}

fract UNIMPLEMENTED sqrtr(fract x)
{
    return x;
}

long fract UNIMPLEMENTED sqrtlr(long fract x)
{
    return x;
}

short accum UNIMPLEMENTED sqrthk(short accum x)
{
    return x;
}

accum sqrtk(accum x)
{
    int32_t rx = bitsk(x);

    if (rx < 0) {
	rx = 0;
    }

    return kbits(sqrtk_bits(rx));
}

long accum UNIMPLEMENTED sqrtlk(long accum x)
{
    return x;
}

unsigned short UNIMPLEMENTED sqrtuhr(unsigned short fract x)
{
    return x;
}

unsigned fract UNIMPLEMENTED sqrtur(unsigned fract x)
{
    return x;
}

unsigned long fract UNIMPLEMENTED sqrtulr(unsigned long fract x)
{
    return x;
}

short accum UNIMPLEMENTED sqrtuhk(short accum x)
{
    return x;
}

unsigned accum UNIMPLEMENTED sqrtuk(unsigned accum x)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    return ukbits(sqrtuk_bits(bitsuk(x)));
#pragma GCC diagnostic pop
}

unsigned long accum UNIMPLEMENTED sqrtulk(unsigned long accum x)
{
    return x;
}
