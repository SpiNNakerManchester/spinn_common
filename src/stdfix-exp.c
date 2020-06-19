/*
 * Copyright (c) 2014-2019 The University of Manchester
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file stdfix-exp.c
 *
 * \brief
 *    An implementation of exponential for fixpoint accum
 *
 * \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 * \copyright
 *    &copy; Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 * \date
 *    20 February, 2014
 */

#include "stdfix-exp.h"
#include "utils.h"

//#include "stdfix-exp-impl.h"
//#include "debug.h"

#ifndef NO_INLINE
#define NO_INLINE	__attribute__((noinline))
#endif

// The following array has entry [n]
//! Series to support exponential: upper coefficients
static uint64_t __exp_hi[26] = {
   9708,  26389,          71733,         194991,          530041,         1440801,
        3916503,       10646160,       28939262,        78665070,       213833830,
      581260615,     1580030169,     4294967296,     11674931555,     31735754293,
    86266724208,   234497268814,   637429664642,   1732713474316,   4710003551159,
 12803117065094, 34802480465680, 94602950235157, 257157480542844, 699026506411923
};

//! Series to support exponential: middle coefficients
static uint32_t __expm1_mid[16] = {
	    0,  260218914,  504671961,  734314346,
    950043403, 1152702096, 1343082321, 1521927990,
   1689937949, 1847768698, 1996036966, 2135322113,
   2266168400, 2389087112, 2504558555, 2613033936
};

//! Series to support exponential: lower coefficients
static uint32_t __exp_series[3] = { 5294, 4293434720, 2081624032 };

// the following calculates a series expansion
// for 1 - exp(-x/2^15) for x in [0..2^11 - 1]

//! \brief Multiplies a coefficient and the variable term correctly and
//! 	efficiently, implementing multiplication of `accum` values.
//! \param[in] c: a bit-cast `accum`
//! \param[in] x: a bit-cast `accum`
//! \return The product, \p c &times; \p x, a bit-cast `accum`
static inline uint32_t coef_mult(
	uint32_t c,
	uint32_t x)
{
    uint64_t tmp = ((uint64_t) c * (uint64_t) x) >> 32;

    return (uint32_t) tmp;
}

//! \brief Exponential series, part of implementation of expk()
//! \param[in] x: The argument
//! \return the result
static inline uint32_t exp_series(uint32_t x) {
    uint32_t tmp;

    //log_info("x = %u", x);
    tmp = __exp_series[1] - coef_mult(__exp_series[2], x);

    //log_info(". tmp = %u", tmp);
    tmp = coef_mult(tmp, x);
    //log_info(".. tmp = %u", tmp);
    tmp += __exp_series[0];
    //tmp = tmp >> 4;
    //log_info("... tmp = %u (= %16.8K)", tmp, ukbits(tmp >> 16));

    return tmp;
}

NO_INLINE accum expk(
	accum x)
{
    int_k_t n = bitsk(x);
    int_k_t r;
    int32_t z, f;
    uint32_t y;
    uint64_t tmp1;

    if (363408 < n) {			// overflow saturation
	r = INT32_MAX;
    } else if (n < -340695) {		// overflow saturation
	r = 0;
    } else {
	z = (int32_t)(n) >> 15;		// truncated integer part.
	f = (int32_t)(n) - (z << 15);	// fractional remainder

	if (f > 0) {
	    z++;
	    f = 32768 - f;

	    tmp1 = __exp_hi[13 + z];
	    tmp1 -= scale64(tmp1, __expm1_mid[f >> 11]);

	    y = ((uint32_t) (f & 0x7FF)) << 17;

	    tmp1 -= scale64(tmp1, exp_series(y));
	} else {			// (f == 0)
	    tmp1 = __exp_hi[13 + z];
	}

	r += 1 << 16;
	r = (int_k_t) (tmp1 >> 17);
    }

    return kbits(r);
}
