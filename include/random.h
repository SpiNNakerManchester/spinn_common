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

/*! \file random.h
 *  \brief Pseudo-random number generators
 *  \author Michael Hopkins
 *  \date February 2014
 *
 * \details
 * These all return well-distributed unsigned 32-bit ints that
 * have been tested rigorously on the DIEHARD, dieharder and TestU01
 * test suites. The Marsaglia KISS generators are slightly faster
 * and the 64-bit one uses the theoretically superior multiplicative
 * algorithm.  The WELL1024a has a cycle length of (2^1024)-1
 *
 * Two versions are available.  A simple version which
 * has a global hidden seed which does not need to be set, and a custom
 * version which allows seeds to be set by the user to either
 *
 * 1. promote independence or
 * 2. guarantee identical behaviour
 *
 * across multiple random number streams.
 *
 * These generators can all be used with the non-uniform RNGs below.
 *
 * \warning
 *     Production code must use the version with explicit seeds to allow for
 *     reproducible random numbers, as this is a critical requirement for
 *     testability and verifiability. Seeds should be supplied from your
 *     Python control code.
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>
#include <stdfix.h>

/*!
 * \name Marsaglia 32-bit KISS uniform random number generator
 *
 * Implementation of a Marsaglia 32-bit KISS generator which uses no multiply
 * instructions
 *
 * 209.9 nanosecs (i.e. 42 ticks) per call
 *
 * Only available in simple form.  Very good for single stream of PRNs.
 *
 * **Cite:**
 * Marsaglia, George. (2011)
 * RNGs with periods exceeding 10 (40million).
 * Message-ID!`603ebe15-a32f-4fbb-ba44-6c73f7919a33@t35g2000yqj.googlegroups.com?`
 * in newsgroups sci.math, comp.lang.c and sci.crypt
 * \{
 */

//! \brief Marsaglia 32-bit KISS generator using no multiplication
//!     instructions.
//! \details Only available in simple form.  Very good for single stream of
//!     PRNs.
//! \return A pseudo-random unsigned 32-bit integer.
uint32_t mars_kiss32(void);

//! \brief Implementation of Marsaglia JKISS RNG uses 64-bit value and 2x
//!     multiplies.
//! \details The best general purpose trade-off between speed,
//!     equidistribution and cycle length.
//! \return A normally distributed 32-bit unsigned integer.
// 219.9 nanosecs (i.e. 44 ticks) per call on ARM with gcc 4.8.1
// Also available in custom seed form.
uint32_t mars_kiss64_simp(void);

// And now... with seeds

//! \brief Seed type for the Marsaglia KISS 64 RNG
typedef uint32_t mars_kiss64_seed_t[4];

//! \brief Validate seed for Marsaglia KISS 64.
//! \note **MUST USE THIS** before passing the seed to mars_kiss64_seed()
//!     the first time.
//! \param[in] seed: The seed to validate
void validate_mars_kiss64_seed(mars_kiss64_seed_t seed);

//! \brief The Marsaglia KISS 64 RNG with custom seed
//! \param[in] seed: The RNG state, including seed value.
//! \return A uniformly randomly distributed 32-bit value.
uint32_t mars_kiss64_seed(mars_kiss64_seed_t seed);
//! \}

//! \name WELL 1024a
//! \brief WELL 1024a long cycle generator (2<sup>1024</sup> - 1) from
//!     L'Ecuyer & Panneton. Better equi-distribution and warm-up
//!     performance than Mersenne Twister, and faster.
//!
//! **Cite:**
//! Panneton, François & L’Ecuyer, Pierre & Matsumoto, Makoto. (2006).
//! Improved long-period generators based on linear recurrences modulo 2.
//! _ACM Trans. Math. Softw._. 32. 1-16.
//! DOI: [10.1145/1132973.1132974](https://doi.org/10.1145/1132973.1132974).
//!
//! \{
//   294.9 nanosecs (i.e. 59 ticks) per call

//! \brief The initialiser function that _MUST BE CALLED ONCE_ before
//!     WELLRNG1024a_simp() is used. Global WELL1024a seed is randomised each
//!     time it is called.
void init_WELL1024a_simp(void);

//! \brief The WELL generator itself
//! \return A uniformly randomly distributed 32-bit value.
uint32_t WELL1024a_simp(void);

// Versions with seeds

//! \brief Seed type for the L'Ecuyer WELL 1024a RNG.
//! \details The 33rd element is the static variable used in simple version.
typedef uint32_t WELL1024a_seed_t[33];

//! \brief **MUST USE THIS** before passing the seed to WELL1024a_seed() the
//!     first time.
//! \param[in] seed: The seed to validate
void validate_WELL1024a_seed(WELL1024a_seed_t seed);

//! \brief The WELL 1024a RNG with custom seed.
//! \param[in] seed: The RNG state, including seed value.
//! \return A uniformly randomly distributed 32-bit value.
uint32_t WELL1024a_seed(WELL1024a_seed_t seed);
//! \}

/* **************************************************

	Non-uniform RNGs

	Being added to all the time, and updated for better speed as
	fixed-point transcendentals and optimised multiplies become available

****************************************************/
/*!
 * @name Non-Uniform RNGs
 *
 * Being added to all the time, and updated for better speed as
 * fixed-point transcendentals and optimised multiplies become available
 * @{
 */

//! \brief The interface type to the uniform RNGs.
//!
//! Interface to a generic uniform random number generator is defined as follows
//!
//! - for a simple type call as ( rng_type, NULL, .. )
//! - for a seeded type call as ( rng_type, seed_vector, .. )
//!
//! This will generate some argument type warnings when used with simple
//! URNGs but avoids a lot of code duplication and allows use of any Uniform
//! RNG with any Non-Uniform variates which is valuable flexibility.
typedef uint32_t (*uniform_rng)(uint32_t*);

//! \brief Von Neuman's exponential distribution generator
//! \details from Ripley p.230 and adapted for our types
//!
//!     Mean number of U(0,1) per call = 5.2
//! \param[in] uni_rng: The uniform random number generator
//! \param[in] seed_arg: The seed argument to the uniform RNG.
//! \return Exponentially distributed random number.
s1615 exponential_dist_variate(uniform_rng uni_rng, uint32_t* seed_arg);

//! \brief Returns standard Gaussian deviate
//! \details translation of NR in C version of Box-Muller
//! \param[in] uni_rng: The uniform random number generator
//! \param[in] seed_arg: The seed argument to the uniform RNG.
//! \return Gaussian-distributed random number.
s1615 gaussian_dist_variate(uniform_rng uni_rng, uint32_t* seed_arg);

//! \brief A poisson distributed random variable, given
//!     \f$ \exp(-\lambda) \f$
//! \details This is Knuth's method, and is \f$ O(n) \f$, so don't use for
//!     large \f$ \lambda \f$
//! \param[in] uni_rng: The uniform random number generator
//! \param[in] seed_arg: The seed argument to the uniform RNG.
//! \param[in] lambda: The shape parameter of the distribution,
//!     \f$ \lambda \f$
//! \return Poisson-distributed random number.
uint32_t poisson_dist_variate(
	uniform_rng uni_rng,
	uint32_t*   seed_arg,
	s1615       lambda);

//! \brief Use this version for Poisson with pre-calculated exp(-&lambda;)
//!     for speed gain.
//! \param[in] uni_rng: The uniform random number generator
//! \param[in] seed_arg: The seed argument to the uniform RNG.
//! \param[in] exp_minus_lambda: The shape parameter of the distribution,
//!     \f$ \exp(-\lambda) \f$.
//! \return Poisson-distributed random number.
uint32_t poisson_dist_variate_exp_minus_lambda(
	uniform_rng uni_rng,
	uint32_t*   seed_arg,
	u032        exp_minus_lambda);

//! @}

#endif 	/*__RANDOM_H__*/
