/*
 * Copyright (c) 2013-2019 The University of Manchester
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

//! \file
//! \brief Normally-distributed random numbers

#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdfix.h>
#include "stdfix-full-iso.h"
#include "assert.h"

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED extern __attribute__((deprecated("Not implemented")))
#endif

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed s16.15.
//! \details Given an 16-bit signed integer value, representing p - 0.5,
//!     return the cumulative normal value associated with that probability.
//! \param[in] x: A uniformly distributed 16-bit PRNG, in the bottom
//!     16 bits of \p x.
//! \return A normally distributed int representation of an s16.15 PRNG.
int __norminv_rbits(int x);

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//!     16 bits of \p x.
//! \return A normally distributed int representation of an s16.15 PRNG.
static inline int __norminv_t_bits(int x) {
    return __norminv_rbits(x >> 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x: A uniformly distributed 16-bit PRNG, in the bottom
//!     16 bits of \p x.
//! \return A normally distributed int representation of an s16.15 PRNG.
static inline int __norminv_b_bits(int x) {
    return __norminv_t_bits(x << 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed ::s1615 PRNG.
//! \param[in] x: A uniformly distributed 16-bit PRNG, in the bottom
//!     16 bits of \p x.
//! \return A normally distributed ::s1615 PRNG.
static inline s1615 norminv_urb(unsigned int x) {
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_b_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed ::s1615 PRNG.
//! \param[in] x: A uniformly distributed 16-bit PRNG, in the top
//!     16 bits of \p x.
//! \return A normally distributed ::s1615 PRNG.
static inline s1615 norminv_urt(unsigned int x) {
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_t_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed ::s1615 PRNG.
//! \param[in] x: A uniformly distributed unsigned fract.
//! \return A normally distributed ::s1615 PRNG.
static inline s1615 norminv_ur(u016 x) {
    union { unsigned int u; int s; } tmp;

    tmp.u = bitsur(x);
    return kbits(__norminv_rbits(tmp.s + INT16_MIN));
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed ::s1615 PRNG.
//! \param[in] x: A uniformly distributed unsigned fract.
//! \return A normally distributed ::s1615 PRNG.
UNIMPLEMENTED s1615 norminv_ulr(u032 x);

//! \brief This function takes a uniformly distributed
//!     PRNG value, \p f, and returns a normally distributed ::s1615 PRNG.
//! \param[in] f: A uniformly distributed unsigned fract.
//! \return A normally distributed ::s1615 PRNG.
#define norminv_fx(f) \
    _Generic((f), \
	u016:     norminv_ur(f), \
	u032:     norminv_ulr(f), \
	default:  __builtin_trap() \
    )

#endif /*__NORMAL_H__*/
