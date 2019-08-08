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

#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdfix.h>
#include "stdfix-full-iso.h"
#include "assert.h"

#define UNIMPLEMENTED extern __attribute__((deprecated("Not implemented")))

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.

int __norminv_rbits(int x);

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.
static inline int __norminv_t_bits(
	int x)
{
    return __norminv_rbits(x >> 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.
static inline int __norminv_b_bits(
	int x)
{
    return __norminv_t_bits(x << 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed accum PRNG.
static inline accum norminv_urb(
	unsigned int x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_b_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//! 16 bits of x.
//! \return A normally distributed accum PRNG.
static inline accum norminv_urt(
	unsigned int x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_t_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed unsigned fract.
//! \return A normally distributed accum PRNG.
static inline accum norminv_ur(
	unsigned fract x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = bitsur(x);
    return kbits(__norminv_rbits(tmp.s + INT16_MIN));
}

UNIMPLEMENTED accum norminv_ulr(unsigned long fract x);

#define norminv_fx(f) \
    _Generic((f), \
	unsigned fract:      norminv_ur(f), \
	unsigned long fract: norminv_ulr(f), \
	default:             abort(1) \
    )

#endif /*__NORMAL_H__*/
