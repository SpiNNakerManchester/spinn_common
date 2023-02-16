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
 *    Copyright &copy; Dave Lester and The University of Manchester, 2014.
 *    All rights reserved. <br>
 *    SpiNNaker Project <br>
 *    Advanced Processor Technologies Group <br>
 *    School of Computer Science <br>
 *    The University of Manchester <br>
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

//! The implementation of a pair
typedef union {
    //! The paired value
    pair_t pair;
    struct {
	//! Low word of the pair
	void *lo;
	//! High word of the pair
	void *hi;
    };
} pair_union_t;

//! \brief Create a pair from two generic 32-bit objects.
//! \param[in] x is a void* object
//! \param[in] y is a void* object
//! \return A pair of values as a 64-bit unsigned int (pair_t).
static inline pair_t __pair(
	void *x,
	void *y)
{
    register pair_union_t tmp;

    tmp.lo = x;
    tmp.hi = y;
    return tmp.pair;
}

//! \brief Return the first component of a pair.
//! \param[in] p is a pair_t object
//! \return The first component: n.b. this is void*, so will need casting.
static inline void *fst(
	pair_t p)
{
    register pair_union_t tmp;

    tmp.pair = p;
    return tmp.lo;
}

//! \brief Return the second component of a pair.
//! \param[in] p is a pair_t object
//! \return The second component: n.b. this is void*, so will need casting.
static inline void *snd(
	pair_t p)
{
    register pair_union_t tmp;

    tmp.pair = p;
    return tmp.hi;
}

//! \brief Create a pair, with automatic casting.
#define pair(x, y)	__pair((void*)(x), (void*)(y))

#endif  /*__PAIR_H__*/
