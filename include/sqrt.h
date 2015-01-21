/*! \file
 *
 *  \brief Implementation of exp for the unsigned accum type, returning a
 *  u0.12 result.
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

/*         short fract sqrthr  (         short fract x);
               fract sqrtr   (               fract x);
         long  fract sqrtlr  (         long  fract x);
         short accum sqrthk  (         short accum x);*/
               accum sqrtk   (               accum x);
/*         long  accum sqrtlk  (         long  accum x);
unsigned short fract sqrtuhr (unsigned short fract x);
unsigned       fract sqrtur  (unsigned       fract x);
unsigned long  fract sqrtulr (unsigned long  fract x);
unsigned short accum sqrtuhk (unsigned short accum x);
unsigned       accum sqrtuk  (unsigned       accum x);
unsigned long  accum sqrtulk (unsigned long  accum x);

#define sqrtfx(f)						      \
    ({								      \
	__typeof__ (f) tmp = (f);				      \
	if      (__builtin_types_compatible_p (__typeof__(f), s07))   \
	    tmp = sqrthr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s015))  \
	    tmp = sqrtr (f, n);                                       \
	else if (__builtin_types_compatible_p (__typeof__(f), s031))  \
	    tmp = sqrtlr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s87))   \
	    tmp = sqrthk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s1615)) \
	    tmp = sqrtk (f, n);                                       \
	else if (__builtin_types_compatible_p (__typeof__(f), s3231)) \
	    tmp = sqrtlk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u08))   \
	    tmp = sqrtuhr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u016))  \
	    tmp = sqrtur (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u032))  \
	    tmp = sqrtulr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u88))   \
	    tmp = sqrtuhk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u1616)) \
	    tmp = sqrtuk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u3232)) \
	    tmp = sqrtulk (f, n);				      \
	else							      \
	    abort ();						      \
	tmp;							      \
    })
*/

#endif /*__SQRT_H__*/
