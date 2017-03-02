//! \file cmsis.h
//! \brief These built-in functions are available for the ARM family
//! of processors (CMSIS)

#ifndef __CMSIS_H__
#define __CMSIS_H__

#include <stdint.h>
#include <stdbool.h>

#define __ARM_Q_FLAG 27

static inline uint32_t __get_CONTROL(void)
{
    uint32_t r;

    asm volatile("mrs %[r], cpsr" : [r] "=r" (r) : : "cc");

    return r;
}

static inline void __set_CONTROL(
		uint32_t r)
{
	asm volatile("msr cpsr_f, %[r]" : : [r] "r" (r) : "cc");
}

#endif /*__CMSIS_H__*/
