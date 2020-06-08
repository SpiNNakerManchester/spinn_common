/*
 * Copyright (c) 2019 The University of Manchester
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
 * \brief Fixed-point multiplication with controlled rounding
 *
 * This file complements stdfix-full-iso.h to add various rounding routines
 * and mixed-format fixed-point multiplication routines.
 *
 * Note that GCC currently does not implement rounding on such operations like
 * fixed-point multiplication and conversion (e.g. decimal to fixed-point).
 * Some more background in https://arxiv.org/abs/1904.11263
 *
 * \author mantas.mikaitis@manchester.ac.uk
 * \date 2019 August.
 */

#ifndef __ROUND_H__
#define __ROUND_H__

#include <stdfix-full-iso.h>
#include <random.h>
#include <sark.h>
#include "utils.h"

#include <stdfix.h>

//! Choose random number generator that is used for the multiplications with
//! stochastic rounding
#if 0
#define RANDOM() mars_kiss64_simp()
#elif 0
#define RANDOM() mars_kiss32()
#elif 1
#define RANDOM() sark_rand()
#else
#define RANDOM() cheap_generator()
#endif

//! The rounding mode of all multiplications
#ifdef FLOATING_POINT
#define ROUNDING_NONE_GCC
#else
//#define ROUNDING_NONE_GCC
//#define ROUNDING_NONE_CUSTOM
//#define ROUNDING_NEAREST
#define ROUNDING_STOCHASTIC
#endif

//! \name High-level interface macros
//! Set up various multiplication macros depending on the rounding method
//! chosen above. These are the highest level of macros that the users
//! should use.
//!
//! #FX_MUL returns an accum, #FX_MUL_SHORT_ACCUM return a short accum,
//! #FX_MUL_FRACT32 return a signed long fract, and #FX_MUL_FRACT16 return
//! a signed fract.
//! \{
#ifdef ROUNDING_NONE_GCC
    #define FX_MUL( x, y ) MULT_NO_ROUND_GCC( x, y )
    #define FX_MUL_SHORT_ACCUM(x, y) MULT_NO_ROUND_GCC( x, y)
    #define FX_MUL_FRACT32( x, y ) MULT_NO_ROUND_GCC( x, y )
    #define FX_MUL_FRACT16( x, y ) MULT_NO_ROUND_GCC( x, y )
#elif defined ROUNDING_NONE_CUSTOM
    #define FX_MUL( x, y ) MULT_NO_ROUND_CUSTOM_ACCUM( x, y )
    #define FX_MUL_SHORT_ACCUM( x, y ) MULT_NO_ROUND_CUSTOM_SHORT_ACCUM( x, y )
    #define FX_MUL_FRACT32( x, y ) MULT_NO_ROUND_CUSTOM_FRACT32( x, y )
    #define FX_MUL_FRACT16( x, y ) MULT_NO_ROUND_CUSTOM_FRACT16( x, y )
#elif defined ROUNDING_NEAREST
    #define FX_MUL( x, y ) MULT_ROUND_NEAREST_ACCUM( x, y )
    #define FX_MUL_SHORT_ACCUM( x, y ) MULT_ROUND_NEAREST_SHORT_ACCUM( x, y )
    #define FX_MUL_FRACT32( x, y ) MULT_ROUND_NEAREST_FRACT32( x, y )
    #define FX_MUL_FRACT16( x, y ) MULT_ROUND_NEAREST_FRACT16( x, y )
#elif defined ROUNDING_STOCHASTIC
    //! Multiply two `accum` values with rounding
    #define FX_MUL( x, y ) MULT_ROUND_STOCHASTIC_ACCUM( x, y )
    //! Multiply two `short accum` values with rounding
    #define FX_MUL_SHORT_ACCUM( x, y ) MULT_ROUND_STOCHASTIC_SHORT_ACCUM(x, y)
    //! Multiply two `fract` values with rounding
    #define FX_MUL_FRACT32( x, y ) MULT_ROUND_STOCHASTIC_FRACT32( x, y )
    //! Multiply two `short fract` values with rounding
    #define FX_MUL_FRACT16( x, y ) MULT_ROUND_STOCHASTIC_FRACT16( x, y )
#endif
//! \}

//! \name Low-level interface macros
//!
//! The following macros choose a correct multiplication function depending on
//! the types of arguments.
//! \{

//! \brief Multiply with whatever the compiler deigns to pick for us
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_NO_ROUND_GCC(x, y) ( (x) * (y) )

//! \brief Multiply fixed point numbers to make s1615; no rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_NO_ROUND_CUSTOM_ACCUM(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s1615) && 		\
        __builtin_types_compatible_p(__typeof__(y), s1615)) {		\
        result = (kbits(__stdfix_smul_k(bitsk(temp0), bitsk(temp1))));	\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031))) {	\
        result = (accum_times_long_fract(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_long_fract(temp1, temp0));		\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032))) {	\
        result = (accum_times_u_long_fract(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_u_long_fract(temp1, temp0));		\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s87; no rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_NO_ROUND_CUSTOM_SHORT_ACCUM(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    short accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s87) &&		\
        __builtin_types_compatible_p(__typeof__(y), s87)) {		\
        result = (hkbits(__stdfix_smul_hk(bitshk(temp0), bitshk(temp1)))); \
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015))) {	\
        result = (short_accum_times_fract(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_fract(temp1, temp0));		\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016))) {	\
        result = (short_accum_times_u_fract(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_u_fract(temp1, temp0));		\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s031; no rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_NO_ROUND_CUSTOM_FRACT32(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    long fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u032) &&		\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (ulrbits(__stdfix_smul_ulr(bitsulr(temp0),		\
				bitsulr(temp1))));			\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (long_fract_times_u_long_fract(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031)) {	\
        result = (long_fract_times_u_long_fract(temp1, temp0));		\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s015; no rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_NO_ROUND_CUSTOM_FRACT16(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u016) &&		\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (urbits(__stdfix_smul_ur(bitsur(temp0), bitsur(temp1)))); \
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (fract_times_u_fract(temp0, temp1));			\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015)) {	\
        result = (fract_times_u_fract(temp1, temp0));			\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s1615; round to nearest
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_NEAREST_ACCUM(x, y)					\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s1615) &&		\
        __builtin_types_compatible_p(__typeof__(y), s1615)) {		\
        result =  (kbits(__stdfix_smul_k_round_nearest(bitsk(temp0),	\
				bitsk(temp1))));			\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031))) {	\
        result =  (accum_times_long_fract_nearest(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_long_fract_nearest(temp1, temp0));	\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032))) {	\
        result =  (accum_times_u_long_fract_nearest(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_u_long_fract_nearest(temp1, temp0));	\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s87; round to nearest
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_NEAREST_SHORT_ACCUM(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    short accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s87) &&		\
        __builtin_types_compatible_p(__typeof__(y), s87)) {		\
        result = (hkbits(__stdfix_smul_hk_round_nearest(bitshk(temp0),	\
                bitshk(temp1))));					\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015))) {	\
        result = (short_accum_times_fract_nearest(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_fract_nearest(temp1, temp0));	\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016))) {	\
        result = (short_accum_times_u_fract_nearest(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_u_fract_nearest(temp1, temp0));	\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s031; round to nearest
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_NEAREST_FRACT32(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    long fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u032) &&		\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (lrbits(__stdfix_smul_ulr_round_nearest(bitsulr(temp0), \
                bitsulr(temp1))));					\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (long_fract_times_u_long_fract_nearest(temp0, temp1)); \
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031)) {	\
        result = (long_fract_times_u_long_fract_nearest(temp1, temp0)); \
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s015; round to nearest
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_NEAREST_FRACT16(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u016) &&		\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (rbits(__stdfix_smul_ur_round_nearest(bitsur(temp0),	\
 				bitsur(temp1))));			\
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (fract_times_u_fract_nearest(temp0, temp1));		\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015)) {	\
        result = (fract_times_u_fract_nearest(temp1, temp0));		\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s1615; stochastic rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_STOCHASTIC_ACCUM(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s1615) &&		\
        __builtin_types_compatible_p(__typeof__(y), s1615)) {		\
        result = (kbits(__stdfix_smul_k_round_stochastic(bitsk(temp0),	\
			 	 bitsk(temp1))));			\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031))) {	\
        result = (accum_times_long_fract_stochastic(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_long_fract_stochastic(temp1, temp0));	\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s1615) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032))) {	\
        result = (accum_times_u_long_fract_stochastic(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s1615)) {	\
        result = (accum_times_u_long_fract_stochastic(temp1, temp0));	\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s87; stochastic rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_STOCHASTIC_SHORT_ACCUM(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    short accum result;							\
    if (__builtin_types_compatible_p(__typeof__(x), s87) &&		\
        __builtin_types_compatible_p(__typeof__(y), s87)) {		\
        result = (hkbits(__stdfix_smul_hk_round_stochastic(bitshk(temp0), \
				bitshk(temp1))));			\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015))) {	\
        result = (short_accum_times_fract_stochastic(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_fract_stochastic(temp1, temp0));	\
    } else if ((__builtin_types_compatible_p(__typeof__(x), s87) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016))) {	\
        result = (short_accum_times_u_fract_stochastic(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s87)) {	\
        result = (short_accum_times_u_fract_stochastic(temp1, temp0));	\
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s031; stochastic rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_STOCHASTIC_FRACT32(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    long fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u032) &&		\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (lrbits(__stdfix_smul_ulr_round_stochastic(bitsulr(temp0), \
				bitsulr(temp1))));			\
    } else if (__builtin_types_compatible_p(__typeof__(x), s031) &&	\
               __builtin_types_compatible_p(__typeof__(y), u032)) {	\
        result = (long_fract_times_u_long_fract_stochastic(temp0, temp1)); \
    } else if (__builtin_types_compatible_p(__typeof__(x), u032) &&	\
               __builtin_types_compatible_p(__typeof__(y), s031)) {	\
        result = (long_fract_times_u_long_fract_stochastic(temp1, temp0)); \
    } else {								\
    }									\
    result;								\
    })

//! \brief Multiply fixed point numbers to make s015; stochastic rounding
//! \hideinitializer
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
#define MULT_ROUND_STOCHASTIC_FRACT16(x, y)				\
    ({									\
    __typeof__(x) temp0 = (x);						\
    __typeof__(y) temp1 = (y);						\
    fract result;							\
    if (__builtin_types_compatible_p(__typeof__(x), u016) &&		\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (rbits(__stdfix_smul_ur_round_stochastic(bitsur(temp0), \
				bitsur(temp1))));			\
    } else if (__builtin_types_compatible_p(__typeof__(x), s015) &&	\
               __builtin_types_compatible_p(__typeof__(y), u016)) {	\
        result = (fract_times_u_fract_stochastic(temp0, temp1));	\
    } else if (__builtin_types_compatible_p(__typeof__(x), u016) &&	\
               __builtin_types_compatible_p(__typeof__(y), s015)) {	\
        result = (fract_times_u_fract_stochastic(temp1, temp0));	\
    } else {								\
    }									\
    result;								\
    })
//! \}

//! \name High-level rounding macros
//! \{

//! \brief Round a signed 64-bit number stochastically to a given number of
//!     bits and return a 32-bit integer.
//! \hideinitializer
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
#define STOCHASTIC_ROUND_S64(x, n) \
    __stdfix_sat_k(__stdfix_stochastic_round_s64_alternative(x, n) >> n)

//! \brief Round an unsigned 64-bit number stochastically to a given number
//!     of bits and return a 32-bit integer.
//! \hideinitializer
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
#define STOCHASTIC_ROUND_U64(x, n) \
    __stdfix_sat_uk(__stdfix_stochastic_round_u64(x, n) >> n)

//! \brief Round a signed 64-bit number to a nearest given number of bits and
//!     return a 32-bit integer.
//! \hideinitializer
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
#define NEAREST_ROUND_S64(x, n) \
    __stdfix_sat_k(__stdfix_round_s64(x, n) >> n)

//! \brief Round an unsigned 64-bit number to a nearest given number of bits
//!     and return a 32-bit integer.
//! \hideinitializer
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
#define NEAREST_ROUND_U64(x, n) \
    __stdfix_sat_k(__stdfix_round_u64(x, n) >> n)

//! \brief Round a 64-bit number down on a given number of bits and
//!     return a 32-bit integer.
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
//! \hideinitializer
#define ROUND_DOWN_S64(x, n) \
    __stdfix_sat_k(x >> n)
//! \}

//! \brief Cheap PRNG from "Numerical Recipes in C", page 284.
//! \return A random number
//! \warning Use of this function is not advised.
static inline uint32_t cheap_generator(void) {
    static unsigned long idum;
    idum = 1664525L * idum + 1013904223L;

    return idum;
}

/*! \name Stochastic rounding routines.
 *
 * Inputs are signed/unsigned integers of various lengths and the number of
 * bits to round. The output is the rounded integer number.
 *
 * \{
 */

//! \brief Stochastic rounding: signed 64-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static int64_t __stdfix_stochastic_round_s64(
        int64_t x, int n)
{
    register int64_t r;
    register uint64_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() >> (32 - n);
    dropped_bits = x & (((uint64_t) 0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}

//! \brief Alternative algorithm for __stdfix_stochastic_round_s64().
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static int64_t __stdfix_stochastic_round_s64_alternative(
        int64_t x, int n)
{
    register int64_t r;
    register uint64_t p;

    p = RANDOM() & (((uint64_t) 0x1 << n) - 1);
    r = x + p;
    return r;
}

//! \brief Stochastic rounding: signed 32-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static inline int32_t __stdfix_stochastic_round_s32(
        int32_t x, int n)
{
    register int32_t r;
    register uint32_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}

//! \brief Stochastic rounding: signed 16-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static inline int16_t __stdfix_stochastic_round_s16(
        int16_t x, int n)
{
    register int16_t r;
    register uint16_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}

//! \brief Stochastic rounding: unsigned 64-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static inline uint64_t __stdfix_stochastic_round_u64(
        uint64_t x, int n)
{
    register uint64_t r;
    register uint64_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() >> (32 - n);
    dropped_bits = x & (((uint64_t) 0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}

//! \brief Stochastic rounding: unsigned 32-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static inline uint32_t __stdfix_stochastic_round_u32(
        uint32_t x, int n)
{
    register uint32_t r;
    register uint32_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}

//! \brief Stochastic rounding: unsigned 16-bit
//! \param[in] x: The value to have rounding applied to it.
//! \param[in] n: The number of least-significant bits to remove via rounding.
//!     For efficiency, should be a constant.
//! \return The rounded value
static inline uint16_t __stdfix_stochastic_round_u16(
        uint16_t x, int n)
{
    register uint16_t r;
    register uint16_t p, dropped_bits;

    r = x >> n;
    p = RANDOM() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p < dropped_bits) {
        return (r + 0x1) << n;
    } else {
        return r << n;
    }
}
//! \}

/*! \name 32-bit multiplication routines with rounding.
 * \{
 */

//! \brief Multiply two integer representations of accum and return an integer
//! representation of an accum answer, rounded to nearest accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline int32_t __stdfix_smul_k_round_nearest(
        int32_t x, int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
        return INT32_MAX;
    }

    return __stdfix_sat_k(__stdfix_round_s64(
            (__I64(x) * __I64(y)), 15) >> 15);
}

//! \brief Multiply two integer representations of accum and return an integer
//! representation of an accum answer, rounded to a neighbouring accum
//! stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline int32_t __stdfix_smul_k_round_stochastic(
        int32_t x, int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
        return INT32_MAX;
    }

    return __stdfix_sat_k(
            __stdfix_stochastic_round_s64((__I64(x) * __I64(y)), 15) >> 15);
}

//! \brief Multiply two integer representations of unsigned long fract and
//! return an integer representation of a signed long fract answer, rounded to
//! nearest accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline uint32_t __stdfix_smul_ulr_round_nearest(
        uint32_t x, uint32_t y)
{
    return __stdfix_sat_ulr(__stdfix_round_u64(
            (__U64(x) * __U64(y)), 33) >> 33);
}

//! \brief Multiply two integer representations of unsigned long fract and
//! return an integer representation of a signed long fract answer, rounded to
//! a neighbouring signed long fract stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline uint32_t __stdfix_smul_ulr_round_stochastic(
        uint32_t x, uint32_t y)
{
    // Note, we preshift the answer by 1 before rounding, to do without the
    // 64-bit RNG.
    return __stdfix_sat_ulr(
            __stdfix_stochastic_round_u64((__U64(x) * __U64(y)) >> 1, 32) >> 32);
}
//! \}

/*! \name 16-bit multiplications.
 * \{
 */

//! \brief Multiply two integer representations of short accum and return an
//! integer representation of a short accum answer, rounded to a nearest
//! short accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline int32_t __stdfix_smul_hk_round_nearest(
        int32_t x, int32_t y)
{
    if (x == INT16_MIN && y == INT16_MIN) { // special case for -1.0*-1.0
        return INT16_MAX;
    }

    return __stdfix_sat_hk(__stdfix_round_s32((__I32(x) * __I32(y)), 7) >> 7);
}

//! \brief Multiply two integer representations of short accum and return an
//! integer representation of a short accum answer, rounded to a neighbouring
//! short accum stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline int32_t __stdfix_smul_hk_round_stochastic(
        int32_t x, int32_t y)
{
    if (x == INT16_MIN && y == INT16_MIN) { // special case for -1.0*-1.0
        return INT16_MAX;
    }

    return __stdfix_sat_hk(
            __stdfix_stochastic_round_s32((__I32(x) * __I32(y)), 7) >> 7);
}

//! \brief Multiply two integer represetations of unsigned fract and return an
//! integer representation of a signed fract answer, rounded to a nearest
//! signed fract.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline uint32_t __stdfix_smul_ur_round_nearest(
        uint32_t x,
        uint32_t y)
{
    return __stdfix_sat_ur(
            __stdfix_round_u32((__U32(x) * __U32(y)), 17) >> 17);
}

//! \brief Multiply two integer represetations of unsigned fract and return an
//! integer representation of a signed fract answer, rounded to a neighbouring
//! signed fract stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline uint32_t __stdfix_smul_ur_round_stochastic(
        uint32_t x,
        uint32_t y)
{
    return __stdfix_sat_ur(
            __stdfix_stochastic_round_u32((__U32(x) * __U32(y)), 17) >> 17);
}
//! \}

/*! \name Mixed format multiplications
 *
 * The following functions do mixed format multiplications. Most of them
 * are reworked from Dave Lester's original implementation
 * accum_times_long_fract() and so have a different format than a set of
 * functions above.
 *
 * \todo Make these functions take integer represetations and use the naming
 * conventions to be consistent with the functions above and in
 * stdfix-full-iso.h
 *
 * \{
 */

//! \brief Multiply an accum and a signed long fract and return an accum answer,
//! without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_long_fract(accum x, long fract y)
{
    register int64_t r;

    r = (int64_t)(bitsk (x)) * (int64_t)(bitslr (y));

    return (kbits (((int32_t)(r >> 31))));
}

//! \brief Multiply an accum and a signed long fract and return an accum answer
//! rounded to the nearest accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_long_fract_nearest(accum x, long fract y)
{
    register int64_t r;

    r = (int64_t)(bitsk (x)) * (int64_t)(bitslr (y));

    return (kbits (((int32_t)(r >> 31) + ((int32_t)(r >> 30) & 1))));
}

//! \brief Multiply accum and a signed long fract and return an accum answer
//! rounded to a neighbouring accum stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_long_fract_stochastic(accum x, long fract y)
{
    register int64_t r;

    r = __stdfix_stochastic_round_s64(
            (int64_t)(bitsk (x)) * (int64_t)(bitslr (y)), 31);

    return (kbits (((int32_t)(r >> 31))));
}

//! \brief Multiply accum and an unsigned long fract and return an accum answer,
//! without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_u_long_fract(accum x, unsigned long fract y)
{
    register int64_t r;

    r = (int64_t)(bitsk (x)) * (uint64_t)(bitsulr (y));

    return (kbits (((int32_t)(r >> 32))));
}

//! \brief Multiply accum and an unsigned long fract and return an accum answer
//! rounded to the nearest accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_u_long_fract_nearest(
        accum x, unsigned long fract y)
{
    register int64_t r;

    r = (int64_t)(bitsk (x)) * (uint64_t)(bitsulr (y));

    return (kbits (((int32_t)(r >> 32) + ((int32_t)(r >> 31) & 1))));
}

//! \brief Multiply accum and an unsigned long fract and return an accum answer
//! rounded to a neighbouring accum stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline accum accum_times_u_long_fract_stochastic(
        accum x, unsigned long fract y)
{
    register int64_t r;

    r = __stdfix_stochastic_round_s64(
            (int64_t)(bitsk (x)) * (uint64_t)(bitsulr (y)), 32);

    return kbits((int32_t) (r >> 32));
}

//! \brief Multiply a signed long fract and an unsigned long fract and return a
//! signed long fract answer, without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline long fract long_fract_times_u_long_fract(
        long fract x, unsigned long fract y)
{
    register int64_t r;

    r = (int64_t) bitslr(x) * (uint64_t) bitsulr(y);

    return lrbits((int32_t) (r >> 32));
}

//! \brief Multiply a signed long fract and an unsigned long fract and return a
//! signed long fract answer rounded to the nearest signed long fract.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline long fract long_fract_times_u_long_fract_nearest(
        long fract x, unsigned long fract y)
{
    register int64_t r;

    r = (int64_t)(bitslr (x)) * (uint64_t) bitsulr(y);

    return lrbits(((int32_t)(r >> 32) + ((int32_t)(r >> 31) & 1)));
}

//! \brief Multiply a signed long fract and an unsigned long fract and return a
//! signed long fract answer rounded to a signed long fract stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline long fract long_fract_times_u_long_fract_stochastic(
        long fract x, unsigned long fract y)
{
    register int64_t r;

    r = __stdfix_stochastic_round_s64(
            (int64_t) bitslr(x) * (uint64_t) bitsulr(y), 32);

    return lrbits((int32_t) (r >> 32));
}

/* ---------------------------------------------------------------------
 * 16-bit multiplies
 */

//! \brief Multiply two short accums and return a short accum answer without
//! rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_fract(short accum x, fract y)
{
    register int32_t r;

    r = (int32_t) bitshk(x) * (int32_t) bitsr(y);

    return hkbits((int32_t) (r >> 15));
}

//! \brief Multiply two short accums and return a short accum answer rounded to
//! the nearest short accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_fract_nearest(
        short accum x, fract y)
{
    register int32_t r;

    r = (int32_t) bitshk(x) * (int32_t) bitsr(y);

    return hkbits((int32_t) (r >> 15) + ((int32_t) (r >> 14) & 1));
}

//! \brief Multiply two short accums and return a short accum answer rounded to
//! a neighbouring short accum stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_fract_stochastic(
        short accum x, fract y)
{
    register int32_t r;

    r = __stdfix_stochastic_round_s32(
            (int32_t) bitshk(x) * (int32_t) bitsr(y), 15);

    return hkbits((int32_t) (r >> 15));
}

//! \brief Multiply a short accum and an unsigned fract and return a short
//! accum answer without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_u_fract(
        short accum x, unsigned fract y)
{
    register int32_t r;

    r = (int32_t) bitshk(x) * (uint32_t) bitsur(y);

    return hkbits((int32_t) (r >> 16));
}

//! \brief Multiply a short accum and an unsigned fract and return a short
//! accum answer rounded to the nearest short accum.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_u_fract_nearest(
        short accum x, unsigned fract y)
{
    register int32_t r;

    r = (int32_t) bitshk(x) * (uint32_t) bitsur(y);

    return hkbits((int32_t) (r >> 16) + ((int32_t) (r >> 15) & 1));
}

//! \brief Multiply a short accum and an unsigned fract and return a short
//! accum answer rounded to a neighbouring short accum stochastically.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline short accum short_accum_times_u_fract_stochastic(
        short accum x, unsigned fract y)
{
    register int32_t r;

    r = __stdfix_stochastic_round_s32(
            (int32_t) bitshk(x) * (int32_t) bitsur(y), 16);

    return hkbits((int32_t) (r >> 16));
}

//! \brief Multiply an unsigned fract and a signed fract and return a signed
//! fract answer without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline fract fract_times_u_fract(
        fract x, unsigned fract y)
{
    register int32_t r;

    r = (int32_t) bitsr(x) * (uint32_t) bitsur(y);

    return rbits((int32_t) (r >> 16));
}

//! \brief Multiply an unsigned fract and a signed fract and return a signed
//! fract answer rounded to a nearest signed fract.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline fract fract_times_u_fract_nearest(
        fract x, unsigned fract y)
{
    register int32_t r;

    r = (int32_t) bitsr(x) * (uint32_t) bitsur(y);

    return rbits((int32_t) (r >> 16) + ((int32_t) (r >> 15) & 1));
}

//! \brief Multiply an unsigned fract and a signed fract and return a signed
//! fract answer without rounding.
//! \param[in] x: The first value
//! \param[in] y: The second value
//! \return The product.
static inline fract fract_times_u_fract_stochastic(
        fract x, unsigned fract y)
{
    register int32_t r;

    r = __stdfix_stochastic_round_s32(
            (int32_t) bitsr(x) * (int32_t) bitsur(y), 16);

    return rbits((int32_t) (r >> 16));
}
//! \}

#endif
