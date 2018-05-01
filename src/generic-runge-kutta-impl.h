/*! \file generic-runge-kutta-impl.h
 * \author David R Lester
 * \date 12/8/2017
 * &copy 2017 The University of Manchester and David R Lester.
 *
 * This short file is the header file for a generic fourth-order Runge-Kutta algorithm
 * to solve initial value problems (IVP) for systems of first-order ODEs.
 * For details (\see https://en.wikipedia.org/wiki/Runge-Kutta_methods).
 */

#ifndef __GENERIC_RUNGE_KUTTA_IMPL
#define __GENERIC_RUNGE_KUTTA_IMPL

#include "generic-runge-kutta.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#define rounddown(x) ((unsigned int)(floor((double)x)))
#define roundup(x)   ((unsigned int)(ceil((double)x)))
#define roundmid(x)  ((unsigned int)(trunc((double)x)))

//! Macro that pretends to use an unused variable, to shut up gcc -Wall -Wextra,
//! which otherwise complains about unused function parameters.

#ifndef use
#define use(x) do {} while ((x) != (x))
#endif

#define FMIN(a,b) ((a)<(b)? (a): (b))
#define FMAX(a,b) ((a)>(b)? (a): (b))
#define FABS(a)   FMAX((a), (-(a)))
#define SIGN(a,b) ((b)==0.0? 0.0: ((b) > 0.0)? FABS((a)): - FABS((a)))

#if (REAL_TYPE == float)
#define POW powf
#else
#define POW pow
#endif

//! A structure for holding the return data from a call to rkqs

typedef struct {real_t hdid; real_t hnext;} rkqs_t;

#endif /*__GENERIC_RUNGE_KUTTA_IMPL*/
