/*
 * stdfix-math-impl.c
 *
 *  SUMMARY
 *
 *    Scientic functions for ISO fixed point types.
 *
 *  AUTHOR
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  COPYRIGHT
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  DESCRIPTION
 *    
 *
 *  CREATION DATE
 *    25 February, 2014
 *
 *  HISTORY
 * *  DETAILS
 *    Created on       : 25 February 2014
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#ifndef __STDFIX_MATH_IMPL_H__
#define __STDFIX_MATH_IMPL_H__

#include "stdfix-math.h"
#include "utils.h"

#ifndef use
#define use(x) do {} while ((x)!=(x))
#endif /*use*/

void additive_range_reduction (uint32_t  x, uint32_t  div, uint32_t mul,
			       uint32_t* y, uint32_t* rem);

#define SQRT_NEG     100
#define EXP_OVERFLOW 101

void stdfix_error (int error);

#endif/*__STDFIX_MATH_IMPL_H__*/
