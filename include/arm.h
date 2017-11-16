/*! \file
 *
 *  \brief These built-in functions are available for the ARM family of processors
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 *
 *  \details
 *    Created on       : 12 December 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#ifndef __ARM_H__
#define __ARM_H__
#include <stdint.h>

#ifdef __arm__

//! \brief This function gives access to the ARM add instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y.

static inline uint32_t __add(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "add %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM adc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y+carry.

static inline uint32_t __adc(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "adc %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM adds instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y, and sets the flag bits
//! (including carry).

static inline uint32_t __adds(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "adds %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

//! \brief This function gives access to the ARM adcs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y+carry, and sets the flag bits
//! (including carry).

static inline uint32_t __adcs(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "adcs %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

//! \brief This function gives access to the ARM sub instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y.

static inline uint32_t __sub(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "sub %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM sbc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y-carry.

static inline uint32_t __sbc(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "sbc %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM subs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y, and sets the flag bits
//! (including carry).

static inline uint32_t __subs(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "subs %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

//! \brief This function gives access to the ARM sbcs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __sbcs(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "sbcs %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

//! \brief This function gives access to the ARM rsb instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x

static inline uint32_t __rsb(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "rsb %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM rsc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry.

static inline uint32_t __rsc(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "rsc %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! \brief This function gives access to the ARM rsbs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __rsbs(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "rsbs %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

//! \brief This function gives access to the ARM rscs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __rscs(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile(
	    "rscs %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");
    return r;
}

#else  /*__arm__*/
#include <static_assert.h>
static_assert(false, "Attempt to use arm intrinsics on non-arm architecture");
#endif /*__arm__*/
#endif /*__ARM_H__*/
