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

static inline uint32_t __arm_add (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("add %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM adc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y+carry.

static inline uint32_t __arm_adc (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("adc %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM adds instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_adds (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("adds %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}

//! \brief This function gives access to the ARM adcs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x+y+carry, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_adcs (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("adcs %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}

//! \brief This function gives access to the ARM sub instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y.

static inline uint32_t __arm_sub (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("sub %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM sbc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y-carry.

static inline uint32_t __arm_sbc (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("sbc %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM subs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_subs (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("subs %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}

//! \brief This function gives access to the ARM sbcs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns x-y-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_sbcs (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("sbcs %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}

//! \brief This function gives access to the ARM rsb instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x

static inline uint32_t __arm_rsb (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("rsb %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM rsc instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry.

static inline uint32_t __arm_rsc (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("rsc %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );

    return (r);
}

//! \brief This function gives access to the ARM rsbs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_rsbs (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("rsbs %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}

//! \brief This function gives access to the ARM adcs instruction.
//! \param[in] x An unsigned 32-bit integer.
//! \param[in] y An unsigned 32-bit integer.
//! \return The function returns y-x-carry, and sets the flag bits
//! (including carry).

static inline uint32_t __arm_rscs (uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("rscs %[r], %[x], %[y]"
                  : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : "cc");

    return (r);
}





//
// \example
//
// #define MUTEX_LOCKED   ((uint32_t)true )
// #define MUTEX_UNLOCKED ((uint32_t)false)
//
// typedef uint32_t* mutex_t;
//
// void lock_mutex (mutex_t mutex)
// {
//   uint32_t lock, check;
//
//   do {
//     lock = MUTEX_LOCKED;
//     __arm_swp (check, lock, (uint32_t*)mutex);
//   } while (check == lock);
//
// }
//
// void unlock_mutex (mutex_t mutex)
// {
//   *((uint32_t*)mutex) = MUTEX_UNLOCKED;
// }


//! \brief This function gives access to the ARM swp instruction.
//! \param[in,out] x The 
//! \param[in] y The 
//! \param[in] address The address of the mutex word.

static inline void __arm_swp  (uint32_t x, uint32_t y,
                               volatile uint32_t* address)
{
    asm volatile ("swp %[x], %[y] [%[a]]"
                  : [x] "+r" (x), [y] "r" (y) : [a] "r" (address) : );
}

//! \brief This function gives access to the ARM swpb instruction.
//! \param[in,out] x The read value of the mutex byte.
//! \param[in] y The byte value to be checked against.
//! \param[in] address The address of the mutex byte.

static inline void __arm_swpb (uint8_t x, uint8_t y,
                               volatile uint8_t* address)
{
    asm volatile ("swpb %[x], %[y] [%[a]]"
                  : [x] "+r" (x), [y] "r" (y) : [a] "r" (address) : );
}

/*
static inline void __builtin_arm_wfi  (void)
{ asm volatile ("wfi"  :  :  : "cc");}

static inline void __builtin_arm_wfe  (void)
{ asm volatile ("wfe"  :  :  : "cc");}
*/

#else  /*__arm__*/
#include <static_assert.h>
static_assert(false, "Attempt to use arm intrinsics on non-arm architecture");
#endif /*__arm__*/
#endif /*__ARM_H__*/
