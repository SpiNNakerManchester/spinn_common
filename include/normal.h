/*
 * Copyright (c) 2013-2023 The University of Manchester
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
//!     PRNG value, \p x, and returns a normally distributed s16.15 in `int`
//!     representation.
//! \details Given an 16-bit signed integer value, representing p - 0.5,
//!     return the cumulative normal value associated with that probability.
//! \param[in] x: A uniformly distributed 16-bit PRNG, in the bottom
//!     16 bits of \p x.
//! \return A normally distributed int representation of an s16.15 PRNG.
int __norminv_rbits(int x);

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed 16-bit PRNG in
//!     `int` representation.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//!     16 bits of \p x.
//! \return A normally distributed int representation of an s16.15 PRNG.
static inline int __norminv_t_bits(int x) {
    return __norminv_rbits(x >> 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed 16-bit PRNG in
//!     `int` representation.
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
//! \param[in] x: A uniformly distributed unsigned ::u016.
//! \return A normally distributed ::s1615 PRNG.
static inline s1615 norminv_ur(u016 x) {
    union { unsigned int u; int s; } tmp;

    tmp.u = bitsur(x);
    return kbits(__norminv_rbits(tmp.s + INT16_MIN));
}

//! \brief This function takes a uniformly distributed 16-bit
//!     PRNG value, \p x, and returns a normally distributed ::s1615 PRNG.
//! \param[in] x: A uniformly distributed unsigned ::u032.
//! \return A normally distributed ::s1615 PRNG.
UNIMPLEMENTED s1615 norminv_ulr(u032 x);

//! \brief This function takes a uniformly distributed
//!     PRNG value, \p f, and returns a normally distributed ::s1615 PRNG.
//! \param[in] f: A uniformly distributed unsigned ::u016 or ::u032.
//! \return A normally distributed ::s1615 PRNG.
#define norminv_fx(f) \
    _Generic((f), \
	u016:     norminv_ur(f), \
	u032:     norminv_ulr(f), \
	default:  __builtin_trap() \
    )

#endif /*__NORMAL_H__*/
