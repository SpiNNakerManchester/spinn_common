/*! \file
 *
 *  \brief It is sometimes useful to return two values from a function.
 *    By exploiting the EABI for 64-bit integer values, we can
 *    efficiently return two 32-bit values, in `C'.
 *
 *  \details The returned values are in r0, and r1 respectively.
 *    This obviates the need to use pointers to return pairs
 *    of values.
 *
 *  \author Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *   \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2014.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date
 *    27 April, 2014
 *
 */

#ifndef __PAIR_H__
#define __PAIR_H__

#include "stdint.h"

//! \brief Give a useful name to the return type, indicating it's use.

typedef uint64_t pair_t;

//! \brief Create a pair from two generic 32-bit objects.
//! \param[in] x is a void* object
//! \param[in] y is a void* object
//! \return A pair of values as a 64-bit unsigned int (pair_t).

static inline pair_t __pair(void* x, void* y)
{
    register union {
	pair_t r1;
	struct { void* lo; void* hi; } r2;
    } tmp;

    tmp.r2.lo = x;
    tmp.r2.hi = y;

    return tmp.r1;
}

//! \brief Return the first component of a pair.
//! \param[in] p is a pair_t object
//! \return The first component: n.b. this is void*, so will need casting.

static inline void* fst(pair_t p)
{
    register union {
	pair_t r1;
	struct { void* lo; void* hi; } r2;
    } tmp;

    tmp.r1 = p;
    return tmp.r2.lo;
}

//! \brief Return the second component of a pair.
//! \param[in] p is a pair_t object
//! \return The second component: n.b. this is void*, so will need casting.

static inline void* snd(pair_t p)
{
    register union {
	pair_t r1;
	struct { void* lo; void* hi; } r2;
    } tmp;

    tmp.r1 = p;

    return tmp.r2.hi;
}

//! \brief Create a pair, with automatic casting.

#define pair(x, y)	__pair((void*)(x), (void*)(y))

#endif  /*__PAIR_H__*/
