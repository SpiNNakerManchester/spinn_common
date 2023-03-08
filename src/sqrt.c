/*
 * Copyright (c) 2014 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \file
 *
 *  \brief Implementation of sqrt for the signed `accum` type.
 *
 *  \details The details of the algorithm are from
 *     <em>"Elementary Functions: Algorithms and Implementation",</em> 2nd edn,
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
#include "pair.h"
#include "arm_acle.h"
#include "arm.h"

#ifndef NO_INLINE
#define NO_INLINE	__attribute__((noinline))
#endif
#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED	__attribute__((deprecated("Not implemented")))
#endif

//! Square root of half, in integral form of unsigned long fract
#define __SQRT_HALF	UINT32_C(3037000500)

//! \brief Test: is \p x even?
//! \param[in] x: the value to test
//! \return True if the test passes
static inline int even(int x) {
    return (x & 1) == 0;
}

//! \brief Test: is \p x odd?
//! \param[in] x: the value to test
//! \return True if the test passes
static inline int odd(int x) {
    return (x & 1) == 1;
}

//! \brief This function multiplies a 64 bit unsigned quantity, by a u0.32.
//! \param[in] x: A 64-bit unsigned quantity.
//! \param[in] y: A 32-bit int representing an unsigned long fract (u0.32)
//! \return (\p x &times; \p y) >> 32
//static inline
uint64_t __x_u64_ulr(
	uint64_t x,
	uint32_t y)
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
//-----------------------------------------------------------------------

//! \brief This function calculates the square of \p r.
//! \param[in] r: The value to be squared (where r represents the
//!     number R = 1-r/2<sup>33</sup>, i.e. 2<sup>31</sup> represents 0.75.
//! \return A 64-bit representation of 1 - R<sup>2</sup>
static inline uint64_t r_squared(
	uint32_t r)
{
    uint64_t t = ((uint64_t) r) << 32;

    t -= ((uint64_t) (r >> 1)) * ((uint64_t) (r >> 1));
    return t;
}

//! \brief This function calculates an improved value of \p r, the reciprocal
//!     square-root.
//! \param[in] x: The number to be square-rooted, in u1.31 format
//! \param[in] r: The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * r in u1.63 format
static inline uint64_t newton_xr(
	uint32_t x,
	uint32_t r)
{
    register uint64_t t = ((uint64_t) x)*((uint64_t) r);

    t = ((uint64_t) x << 32) - (t >> 1);
    return t;
}

//! \brief This function calculates an improved value of \p r, the reciprocal
//!     square-root.
//! \param[in] x: The number to be square-rooted, in u1.31 format
//! \param[in] r: The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * r in u1.63 format
static inline uint64_t newton_xlr(
	uint32_t x,
	uint64_t r)
{
    register uint64_t t = __x_u64_ulr(r, x);

    t = ((uint64_t)(x) << 32) - (t >> 1);
    return t;
}

//! \brief This function calculates an improved value of \p r, the reciprocal
//!     square-root.
//! \param[in] x: The number to be square-rooted, in u1.63 format
//! \param[in] r: The current approximation R = 1 - r/2 (r is in u0.32)
//! \return x * R in u1.63 format
//static inline
static uint64_t newton_lxr(
	uint64_t x,
	uint32_t r)
{
    return x - __x_u64_ulr(x, r >> 1);
}

//! \brief This function calculates an improved value of \p r, the reciprocal
//!     square-root.
//! \param[in] x: The number to be square-rooted
//! \param[in] r: The current approximation R
//! \return The new approximation
uint64_t newton_lxlr(
	uint64_t x,
	uint64_t r)
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
//!     square-root.
//! \param[in] x: The number to be square-rooted, in u1.31 format
//! \param[in] r: The current approximation R = 1 - r/2 (r is in u0.32)
//! \return (x * R<sup>2</sup> - 1) in u1.63 format
uint64_t newton_xr2(
	uint32_t x,
	uint32_t r)
{
    register uint64_t t;

    t = newton_xr(x, r);	// x*R   in u1.63 format
    t = newton_lxr(t, r);	// x*R^2 in u1.63 format
    t -= UINT64_C(1) << 63;	// (x*R^2 - 1) in u1.63 format
    return t;
}

//! \brief This function calculates an improved value of \p r, the reciprocal
//!     square-root.
//! \param[in] x: The number to be square-rooted
//! \param[in] r: The current approximation R
//! \return The new approximation
uint64_t newton_xlr2(
	uint32_t x,
	uint64_t r)
{
    register uint64_t t;

    t = newton_xlr(x, r);	// x*R   in u1.63 format
    t = newton_lxlr(t, r);	// x*R^2 in u1.63 format
    t -= UINT64_C(1) << 63;	// (x*R^2 - 1) in u1.63 format
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
//-----------------------------------------------------------------------

//! \brief A single stage of recip_normalized_root();
//!     handles coarse approximation
//! \param[in] x: The value to have its square root calculated
//! \param[in] r: The current approximation
//! \return The improved approximation
uint64_t NO_INLINE square_root_improve(
	uint32_t x,
	uint32_t r)
{
    register union { uint64_t u; int64_t s; } tmp;
    register uint64_t t;
    register bool neg;

    tmp.u = newton_xr2(x, r);		// tmp.u has Z = (x*R^2 - 1) in s0.63 format.

    neg = tmp.s < 0;
    if (neg) {
        tmp.s = -tmp.s;
    }

    t = __x_u64_ulr(tmp.u, r >> 1);	// t has |Z|*r/2 in s0.63 format
    tmp.u -= t;

    if (neg) {
        tmp.s = -tmp.s;
    }
    tmp.s <<= 1;
    tmp.u += ((uint64_t) r) << 32;
    return tmp.u;
}

//! \brief A single stage of recip_normalized_root(); refines approximation
//! \param[in] x: The value to have its square root calculated
//! \param[in] r: The current approximation
//! \return The improved approximation
uint64_t NO_INLINE square_root_ximprove(
	uint32_t x,
	uint64_t r)
{
    register union { uint64_t u; int64_t s; } tmp;
    register uint64_t t;
    register bool neg;

    //log_info("x = %u, r = (%u:%u)\n",
    //         x,
    //         (uint32_t)(r >> 32),
    //         (uint32_t)(r & UINT32_C(0xFFFFFFFF)));

    tmp.u = newton_xlr2(x, r);		// tmp.u has (x*r^2 - 1) in s0.63 format.

    neg = tmp.s < 0;
    if (neg) {
        tmp.s = -tmp.s;
    }

    t  = __x_u64_ulr(tmp.u, (uint32_t) (r >> 33));
    t += __x_u64_ulr(tmp.u, (uint32_t) ((r >> 1) & UINT32_C(0xFFFFFFFF))) >> 32;
					// t has |Z|*r/2 in s0.63 format
    tmp.u -= t;

    if (neg) {
        tmp.s = -tmp.s;
    }
    tmp.s <<= 1;

    //log_info("tmp = (%u:%u)\n",
    //         (uint32_t) (tmp.u >> 32),
    //         (uint32_t) (tmp.u & UINT32_C(0xFFFFFFFF)));

    tmp.u += r;
    return tmp.u;
}

//! \brief Calculates the reciprocal square-root of the argument
//! \param[in] x: An unsigned integer, representing a u1.31. Leading bit is 1.
//! \return An unsigned 64-bit integer representing the reciprocal square-root
//!     of x, as a u0.64.
//! \todo INCORRECT FORMAT DOCUMENTATION. FIX THIS!
uint64_t recip_normalized_root(
	uint32_t x)
{
    uint64_t result;
    uint32_t initial = (x & 0x7FFFFFFF) >> 1;

    assert(x > INT32_MAX);
    //log_info("x = (%u), initial = (%u)", x, initial, initial);

    result = square_root_improve(x, initial);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_improve(x, (uint32_t) (result >> 32));
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_improve(x, (uint32_t) (result >> 32));
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));
    result = square_root_ximprove(x, result);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));

    result = square_root_ximprove(x, result);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));

    // DRL HACK!!! The following result = -(result >> 1);
    //log_info("result = %u %u", (uint32_t)(result >> 32),
    //          (uint32_t)(result & 0xFFFFFFFF));

    return result;
}

//! \brief Calculates the square-root of the argument
//! \param[in] x: A signed integer, representing an s16.15 accum.
//! \return A signed integer representing the square-root of x, as an s16.15.
static inline int32_t sqrtk_bits(int32_t x) {
    uint64_t tmp;
    int32_t n;
    union { uint32_t u; int32_t s; } r;

    assert(x >= 0);

    if ((x == 0) || x == 32768) {
        return x;	// x is zero or one.
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

    tmp = newton_xlr(r.u, tmp);
    tmp >>= 16 + ((n - 16) >> 1);
			// if x was 0.25, tmp = 1.0 (in u1.63), n = 17
			// if x was 1.0, tmp = 1.0 (in u1.63), n = 16
			// if x was 4.0, tmp = 1.0, n = 15
			// if x was 2^14, tmp = 1.0, n = 16 + (n-16)/2 = 9

    if (odd(n)) {
        tmp = __x_u64_ulr(tmp, __SQRT_HALF);
    }
    return (int32_t) (tmp >> 32);
}

accum sqrtk(accum x) {
    int32_t rx = bitsk(x);

    if (rx < 0) {
        rx = 0;
    }

    return kbits(sqrtk_bits(rx));
}
