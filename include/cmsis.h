/*
 * Copyright (c) 2013-2019 The University of Manchester
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

//! \file cmsis.h
//! \brief These built-in functions are available for the ARM family
//!     of processors (CMSIS)

#ifndef __CMSIS_H__
#define __CMSIS_H__

#include <stdint.h>
#include <stdbool.h>

//! Saturation flag bit in control register
#define __ARM_Q_FLAG 27

//! \brief Get the control register
//! \return the current value of the control register
static inline uint32_t __get_CONTROL(void)
{
    uint32_t r;

    asm volatile("mrs %[r], cpsr" : [r] "=r" (r) : : "cc");
    return r;
}

//! \brief Set the control register
//! \param[in] r: The value to set
static inline void __set_CONTROL(
	uint32_t r)
{
    asm volatile("msr cpsr_f, %[r]" : : [r] "r" (r) : "cc");
}

#endif /*__CMSIS_H__*/
