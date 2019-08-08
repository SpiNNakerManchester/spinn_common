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

/*! \file
 *
 *  \brief Implementation of sqrt for the unsigned accum type, returning a
 *  s16.15 result.
 *
 *  \details The details of the algorithm are from
 *     "Elementary Functions: Algorithms and Implemenation", 2nd edn,
 *     Jean-Michel Muller, Birkhauser, 2006.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester, Jamie Knight and The University of Manchester,
 *    2014.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 23 October, 2014
 *
 */

#ifndef __SQRT_H__
#define __SQRT_H__

#include <stdfix.h>
#include "debug.h"
#include "spin-print.h"

#define UNIMPLEMENTED extern __attribute__((deprecated("Not implemented")))

extern accum				sqrtk(accum x);

/*
 * All these functions have no implementation in this library. They will
 * generate a compile-time warning if used, and a link-time error.
 */

UNIMPLEMENTED short fract		sqrthr(short fract x);
UNIMPLEMENTED fract			sqrtr(fract x);
UNIMPLEMENTED long fract		sqrtlr(long fract x);
UNIMPLEMENTED short accum		sqrthk(short accum x);
UNIMPLEMENTED long accum		sqrtlk(long accum x);
UNIMPLEMENTED unsigned short fract	sqrtuhr(unsigned short fract x);
UNIMPLEMENTED unsigned fract		sqrtur(unsigned fract x);
UNIMPLEMENTED unsigned long fract	sqrtulr(unsigned long fract x);
UNIMPLEMENTED unsigned short accum	sqrtuhk(unsigned short accum x);
UNIMPLEMENTED unsigned accum		sqrtuk(unsigned accum x);
UNIMPLEMENTED unsigned long accum	sqrtulk(unsigned long accum x);

#define sqrtfx(f) \
    _Generic((f), \
	s07:   sqrthr(f), \
	s015:  sqrtr(f), \
	s031:  sqrtlr(f), \
	s87:   sqrthk(f), \
	s1615: sqrtk(f), \
	s3231: sqrtlk(f), \
	u08:   sqrtuhr(f), \
	u016:  sqrtur(f), \
	u032:  sqrtulr(f), \
	u88:   sqrtuhk(f), \
	u1616: sqrtuk(f), \
	u3232: sqrtulk(f), \
	default: abort() \
    )

#endif /*__SQRT_H__*/
