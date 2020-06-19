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

/*! \file
 *
 *  \brief Utility functions for SpiNNaker
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    &copy; Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

//! \brief This function returns the most significant 32-bit word of a 64-bit
//!     unsigned integer.
//! \param[in] x: The 64-bit number
//! \return The most significant 32-bits of x.
static inline uint32_t __hi(uint64_t x) {
    return (uint32_t) (x >> 32);
}

//! \brief This function returns the least significant 32-bit word of a 64-bit
//!     unsigned integer.
//! \param[in] x: The 64-bit number
//! \return The least significant 32-bits of x.
static inline uint32_t __lo(uint64_t x) {
    return __hi(x << 32);
}

//! \brief The function treats the 64-bit number as if it were a 32-bit integer
//!     and a 32-bit fraction, rounding the fractional part.
//! \param[in] x: The 64-bit number
//! \return The rounded result.
static inline uint64_t round64(uint64_t x) {
    uint64_t r = (uint64_t) __hi(x);

    if (__lo(x) >= INT32_MAX) {
	r++;
    }
    return r;
}

//! \brief The function scales the 64-bit number \p x, treating \p y as if it
//!     were an unsigned long fract, rounding the fractional part.
//! \param[in] x: A 64-bit unsigned integer.
//! \param[in] y: A 32-bit unsigned integer treated as if it is an
//!     unsigned long fract.
//! \return The rounded result.
static inline uint64_t scale64(
	uint64_t x,
	uint32_t y)
{
    uint64_t r = round64((uint64_t) __lo(x) * (uint64_t) y);

    r += (uint64_t) __hi(x) * (uint64_t) y;
    return r;
}

//! \brief The function scales the 32-bit number \p x, treating \p y as if it
//!     were an unsigned long fract, rounding the fractional part.
//! \param[in] x: A 32-bit unsigned integer.
//! \param[in] y: A 32-bit unsigned integer treated as if it is an
//!     unsigned long fract.
//! \return The rounded result.
static inline uint32_t scale32(
	uint32_t x,
	uint32_t y)
{
    return (uint32_t) round64((uint64_t) x * (uint64_t) y);
}

//! \brief Returns the log of the next highest power of 2 of a value
//! \param[in] v: A 32-bit unsigned integer
//! \return The log of the next highest power of 2
static inline uint32_t log_next_power_of_2(uint32_t v) {
    return 32 - __builtin_clz(v);
}

//! \brief Returns the integer part of the log to base 2 of a number
//! \param[in] v: A 32-bit unsigned integer
//! \return The log to the base 2
static inline uint32_t ilog_2(uint32_t v) {
    return 31 - __builtin_clz(v);
}

//! \brief Returns the next highest power of 2 of a value
//! \param[in] v: A 32-bit unsigned integer
//! \return The next highest power of 2
static inline uint32_t next_power_of_2(uint32_t v) {
    return 1 << log_next_power_of_2(v);
}

//! \brief Returns True if the value is a power of 2
//! \param[in] v: A 32-bit unsigned integer
//! \return True if the value is a power of 2, False if not
static inline bool is_power_of_2(uint32_t v) {
    return (v & (v - 1)) == 0;
}

#endif /*__UTILS_H__*/
