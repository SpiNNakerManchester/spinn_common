/*
 * Copyright (c) 2014 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \file
 *
 *  \brief Implementation of log for the accum type.
 *
 *  \details The details of the algorithm are from
 *     "Elementary Functions: Algorithms and Implementation", 2nd edn,
 *     Jean-Michel Muller, Birkhauser, 2006.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester, Jamie Knight and The University of Manchester,
 *    2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 30 October, 2014
 *
 */

#include "stdfix-full-iso.h"
#include "assert.h"
#include "arm_acle.h"
#include "pair.h"

//! \brief This function calculates the sin function approximation.
//! \param[in] x: an s0.31 representing a number in the interval
//!     [-1/32, 1/32); i.e. INT32_MIN represents -1/32
//! \return A value representing sin(x) in s0.31 format.
static inline int32_t sin_approx(int32_t x) {
    return x >> 5;
}

//! \brief This function calculates the cos function approximation.
//! \param[in] x: an s0.31 representing a number in the interval
//!     [-1/32, 1/32); i.e. INT32_MIN represents -1/32
//! \return A value representing (1 - cos(x)) in s0.31 format.
static inline int32_t cos_approx(int32_t x) {
    return __smultt(x, x) >> 10;
}

//! \brief sin_table[k] = sin(k/16) * 2^31
static const int32_t sin_table[16] = {
              0,  134130364,  267736951,  400298032,
      531295957,  660219183,  786564267,  909837834,
     1029558505, 1145258771, 1256486826, 1362808327,
     1463808091, 1559091715, 1648287120, 1731045999
};

//! \brief cos_table[k] = (cos(k/16) - 1) * 2^31
static const int32_t cos_table[16] = {
               0,   -4192939,  -16755382,  -37638274,
       -66760066, -104007040, -149233746, -202263575,
      -262889447, -330874618, -405953610, -487833239,
      -576193767, -670690148, -770953377, -876591926
};

//! \brief This function calculates the sin function for the interval [0, &pi;/4)
//! \param [in] x An s0.31 representing a value in the range [-1/32,1/32),
//!     with INT32_MIN representing -1/32.
//! \param [in] cos_x The value of cos_x (= (1 - cos(x)) * 2^31)
//!     represented as an s0.31
//! \param [in] break_point A value in the range 0..13
//! \return sin(x + break_point / 16) as s0.31
static inline int32_t sin_pi4(
	int32_t x,
	int32_t cos_x,
	int break_point)
{
    register int32_t r;

    assert(0 <= break_point && break_point <= 13);

    r = sin_table[break_point];
    r = __smlawt(sin_table[break_point], -(cos_x << 1), r);
    r = __smlawt(cos_table[break_point], x >> 4, r + (x >> 5));
    return r;
}

//! \brief This function calculates the cos function for the interval [0, &pi;/4)
//! \param [in] x An s0.31 representing a value in the range [-1/32,1/32),
//!     with INT32_MIN representing -1/32.
//! \param [in] cos_x The value of cos_x (= (1 - cos(x)) * 2^31)
//!     represented as an s0.31
//! \param [in] break_point A value in the range 0..13
//! \return 1 - cos(x + break_point / 16) as s0.31
//!
//------------------------------------------------------------------------
//! \details Implementation notes
//!
//! We are using the identity: cos(x+b) = cos(x)*cos(b) - sin(x)*sin(b)
//!
//! However, we return 1-cos(x+b)
//!         = 1 - cos(x)*cos(b) + sin(x)*sin(b)
//!
//! Let cx = 1-cos(x), and cb = 1-cos(b)
//!
//! Then cx * cb = 1 + cos(x)*cos(b) - cos(x) - cos(b)
//!
//! Thus 1 - cos(x)*cos(b)
//!    = -(cx*cb) + cx + cb
//------------------------------------------------------------------------
static inline int32_t cos_pi4(
	int32_t x,
	int32_t cos_x,
	int break_point)
{
    register int32_t r;

    assert(0 <= break_point && break_point <= 13);

    //log_info("x = %d (%r), 1 - cos(x) = %d, bp = %d",
    //         x, x >> 21, cos_x, break_point);

    r = cos_x - cos_table[break_point];
    r = __smlawt(cos_table[break_point], cos_x << 1, r);
    r = __smlawt(sin_table[break_point], (x >> 4), r);

    //log_info("r = 1 - cos(%r + %d/16) = %r (%d)",
    //         x >> 21, break_point, r >> 16, r);

    return r;
}

//! \brief This function multiplies it's signed argument by sqrt(0.5).
//! \param[in] x is a signed 32-bit quantity.
//! \return x * sqrt(0.5) in the same format as x.
//!
//------------------------------------------------------------------------
//! \details Implementation notes
//!
//! Consider experimenting with smlawt/b instructions.
//
//! The calculation
//!
//!     r = (x * 1518500250) >> 31;
//!
//! is equivalent to:
//!
//!     r = smulwt(x, 23170);
//!     r = smlawb(x, 31130, r) << 1; // n.b. two constants in same register
//!
//------------------------------------------------------------------------
int32_t mul_sqrt_half(int32_t x) {
    register int64_t tmp = (int64_t) x;

    tmp = (tmp * 1518500250) >> 31;
    return (int32_t) tmp;
}

//! \brief This function calculates the sin function for the interval [0, &pi;/4)
//!     divided by sqrt(0.5).
//! \param [in] s An s0.31 representing the value of sin(x).
//! \return An s0.31 representing sin(x) * sqrt(0.5).
static inline int32_t sin_pi4_x_sqrt_half(int32_t s) {
    return mul_sqrt_half(s);
}

//! \brief This function calculates the cos function for the interval [0, &pi;/4)
//!     divided by sqrt(0.5).
//! \param [in] c An s0.31 representing the value of 1 - cos(x).
//! \return An s0.31 representing cos(x) * sqrt(0.5).
//!
//------------------------------------------------------------------------
//! \details Implementation notes
//!
//! Note that we have taken the opportunity to fold the subtraction back into
//! the calculation. The reason is that we no longer have to represent 1.0000
//! as an s0.31.
//------------------------------------------------------------------------
static inline int32_t cos_pi4_x_sqrt_half(int32_t c) {
    return 1518500250 - mul_sqrt_half(c);
}

//! \brief Get the reduced range value out of the result of
//! 	sincos_range_reduction()
//! \param[in] p: The pair
//! \return the reduced range value (range [0,&pi;/4] if fixed point)
static inline int reduced(pair_t p) {
    return (int) fst(p);
}

//! \brief Get the octant out of the result of sincos_range_reduction()
//! \param[in] p: The pair
//! \return the octant (range [0,7])
static inline int quadrant(pair_t p) {
    return (int) snd(p);
}

//! \brief This function returns the range-reduced argument and the quadrant.
//! \param[in] x is an s16.15.
//! \return A pair of values (accessible by use of fst() and snd()).
//!     The first component is the range reduced value of x represented by
//!     an s0.31 in the range [0,&pi;/4). The second is a quadrant, in the
//!     range 0..7
pair_t sincos_range_reduction(int32_t x) {
    register int n = __smulwb(x, 20861) >> 13;
    register int q = n & 7;
    register int d;

    //log_info("n = %d", n);
    assert(-83444 <= n && n < 83444);
    assert(0 <= q && q < 8);

    // At this point n = floor(x * 4/pi)

    // We now need to _accurately_ subtract n * (pi/4) from x, and we will
    // have range reduced to [0, pi/4).

    // We use Cody & Waites' "accurate constant" method, by subtracting
    // multiples of 7358 from n. This has the effect of subtracting 189364951
    // from x. 189364951/2^15 is extremely close to 7358 * pi/4. In fact the
    // approximation is a mere 0.87e-9 away from the correct value.

    while (n < 0) {
        n += 7358;
        x += 189364951;
    }

    while (7358 <= n) {
        n -= 7358;
        x -= 189364951;
    }

    // DRL HACK!!! Replace above with multiply by reciprocal of 7358!!!
    // This will improve efficiency.

    assert(0 <= n && n < 7358);
    assert(0 <= x && x < 189364951);

    //log_info("n = %d, x = %d", n, x);		// correct to here...

    // At this point, we can now scale x, so that further range reductions
    // can be carried out at higher precision.

    x = x << 3;

    // Another iteration of Cody & Waites' method, this time subtracting
    // multiples of 12 gives (12 * pi/4 ~= 2470649/2^18):

    // This is sufficiently accurate that it could be used from the outset.

    d = __smulwb(n, 21845) >> 2;
    //log_info("d = %d", d);
    n = __smlawb(-786432, d, n);
    x -= 2470649 * d;

    assert(0 <= n && n < 12);
    assert(0 <= x && x < 3145728);

    //log_info("n = %d, x = %d", n, x);

    // At this point, we can now scale x, so that further range reductions
    // can be carried out at higher precision.

    x = x << 9;

    x -= 105414357 * n;

    //log_info("x = %d, q = %d", x, q);

    if (x < 0) {		// inadvertently selected n too large.
        x += 105414357;
        q = (q + 7) % 8;
    }

    //log_info("x = %d, q = %d", x, q);

    x = x << 4;
    check(0 <= x && x <= 1686629713, "0 <= %d <= 1686629713 (n = %d)", x, n);

    //log_info("x = %d, q = %d", x, q);

    return pair(x, q);
}

//! \brief Is an integer value even?
//! \param[in] x: The value to test
//! \return True if \p x is even
static inline bool even(int x) {
    return (x & 1) == 0;
}

//! \brief Implementation of sink() on the bit-representation of fixed point
//! 	values
//! \param[in] x: the value to get the sine of
//! \return the sine of the value
static inline int32_t sinkbits(int32_t x) {
    register pair_t p = sincos_range_reduction(x);
    register int break_point;
    register int32_t r, cos_x, q;

    x = reduced(p);
    q = quadrant(p);

    break_point = __stdfix_round_s32(x, 27);

    x -= break_point;
    x = x << 5;
    break_point = break_point >> 27;
    cos_x = cos_approx(x);

    if (even(q)) {
        switch (q) {
        case 0:
            r = sin_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            break;
        case 2:
            r = cos_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            r = 32768 - r;
            break;
        case 4:
            r = -sin_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            break;
        case 6:
            r = cos_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            r = r - 32768;
            break;
        default:
            r = 0;
            assert(false);
            break;
        }
    } else {
        register int32_t s = sin_pi4_x_sqrt_half(sin_pi4(x, cos_x, break_point));
        register int32_t c = cos_pi4_x_sqrt_half(cos_pi4(x, cos_x, break_point));

        switch (q) {
        case 1:
            r = c + s;
            break;
        case 3:
            r = c - s;
            break;
        case 5:
            r = -(c + s);
            break;
        case 7:
            r = -(c - s);
            break;
        default:
            r = 0;
            assert(false);
            break;
        }
        r = __stdfix_round_s32(r, 16) >> 16;
    }

    assert(-32768 <= r && r <= 32768);
    return r;
}

//! \brief Implementation of cosk() on the bit-representation of fixed point
//! 	values
//! \param[in] x: the value to get the cosine of
//! \return the cosine of the value
static inline int32_t coskbits(int32_t x) {
    register pair_t p = sincos_range_reduction(x);
    register int break_point;
    register int32_t r, cos_x, q;

    //log_info("x = %d, (%k)", x, x);

    x = reduced(p);
    q = quadrant(p);

    //log_info("x  = %d (%r), q = %d", x, x >> 16, q);

    break_point = __stdfix_round_s32(x, 27);

    //log_info("bp = %d (%r)", break_point, break_point >> 16);

    x -= break_point;
    x <<= 5;
    break_point >>= 27;
    cos_x = cos_approx(x);

    //log_info("x' = %d (%r), q = %d, bp = %d", x, x >> 16, q, break_point);

    if (even(q)) {
        switch (q) {
        case 0:
            r = cos_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            r = 32768 - r;
            break;
        case 2:
            r = -sin_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            break;
        case 4:
            r = cos_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            r = r - 32768;
            break;
         case 6:
            r = sin_pi4(x, cos_x, break_point);
            r = __stdfix_round_s32(r, 16) >> 16;
            break;
       default:
            r = 0;
            assert(false);
            break;
        }
    } else {
        register int32_t s = sin_pi4_x_sqrt_half(sin_pi4(x, cos_x, break_point));
        register int32_t c = cos_pi4_x_sqrt_half(cos_pi4(x, cos_x, break_point));

        switch (q) {
        case 1:
            r = c - s;
            break;
        case 3:
            r = -(c + s);
            break;
        case 5:
            r = -(c - s);
            break;
        case 7:
            r = c + s;
            break;
        default:
            r = 0;
            assert(false);
            break;
        }
        r = __stdfix_round_s32(r, 16) >> 16;
    }

    assert(-32768 <= r && r <= 32768);
    return r;
}

accum sink(accum x) {
    return kbits(sinkbits(bitsk(x)));
}

accum cosk(accum x) {
    return kbits(coskbits(bitsk(x)));
}
