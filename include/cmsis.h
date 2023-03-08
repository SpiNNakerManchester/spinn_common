/*
 * Copyright (c) 2013 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
