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

/*! \file
 * \brief These built-in functions are available for the ARM family of
 * processors
 *
 * This header file implements the ARM C Language Extension 2.0, available at:
 *
 *  http://infocenter.arm.com/help/topic/com.arm.doc.ihi0053c/IHI0053C_acle_2_0.pdf
 */


// 6.2 Testing for ARM C Language Extensions

// __ARM_ACLE is defined to the version of this specification implemented, as
// 100*major version + minor_version. An implementation implementing version
// 1.1 (this version) of the ACLE specification will define __ARM_ACLE as 101.

#ifndef __ARM_ACLE_GCC_H
#define __ARM_ACLE_GCC_H
//! \brief This value is defined whenever the ARM C Language Extensions
//! are available.

#define __ARM_ACLE 200

//! \brief This indicates that we may use a few useful extensions to ARM's
//! ACLE. Without them gcc may not be able to exploit all available
//! instructions.

#define __ARM_ACLE_EXTENSIONS

#include <stdint.h>
#include <stdbool.h>
#include "static-assert.h"
#include "cmsis.h"

#ifndef  __GNUC__
static_assert(false,
	"Attempt to compile arm_acle_gcc.h with a non-gcc compiler");
#endif

#ifdef __STRICT_ANSI__
static_assert(false,
	"Attempt to compile arm_acle_gcc.h with strict ANSI compliance");
#endif

#if !(defined (__arm__) || defined (__thumb__))
static_assert(false,
	"Attempt to compile arm_acle_gcc.h for non-arm architecture");
#endif

#ifndef UNUSED
#define UNUSED      __attribute__((__unused__))
#endif

// Following are all pre-defined at this stage, either above, or by GCC

//#undef __ARM_ACLE                    // indicates ACLE implemented
//#undef __ARM_FEATURE_DSP             // DSP instructions (ARM v5E)
//#undef __ARM_FEATURE_UNALIGNED       // hardware support for unaligned access
//#undef __ARM_PCS                     // ARM procedure call standard
//#undef __ARM_PCS_VFP                 // ARM PCS hardware FP variant in use
//#undef __ARM_FEATURE_CLZ             // CLZ instruction

//     Macro name                       Meaning
#undef __ARM_32BIT_STATE             // code is for 32-bit state (new in 1.1)
#undef __ARM_ALIGN_MAX_PWR           // log of maximum alignment of static object
#undef __ARM_ALIGN_MAX_STACK_PWR     // log of maximum alignment of stack object
#undef __ARM_ARCH                    // ARM architecture level
#undef __ARM_ARCH_ISA_ARM            // ARM instruction set present
#undef __ARM_ARCH_ISA_THUMB          // Thumb instruction set present
#undef __ARM_ARCH_PROFILE            // architecture profile
#undef __ARM_BIG_ENDIAN              // memory is big-endian
#undef __ARM_FEATURE_FMA             // floating-point fused multiply-accumulate
#undef __ARM_FEATURE_IDIV            // integer divide in hardware (new in 1.1)
#undef __ARM_FEATURE_LDREX           // load/store exclusive instructions
#undef __ARM_FEATURE_QBIT            // Q (saturation) flag
#undef __ARM_FEATURE_SAT             // width-specified saturation instruction
#undef __ARM_FEATURE_SIMD32          // 32-bit SIMD instructions (ARM v6)
#undef __ARM_FP                      // hardware floating-point
#undef __ARM_FP_FAST                 // accuracy-losing optimizations
#undef __ARM_FP_FENV_ROUNDING        // rounding is configurable at runtime
#undef __ARM_FP16_FORMAT_ALTERNATIVE // 16-bit floating-point, alternative format
#undef __ARM_FP16_FORMAT_IEEE        // 16-bit floating-point, IEEE format
#undef __ARM_NEON                    // Advanced SIMD (NEON) extension
#undef __ARM_NEON_FP                 // Advanced SIMD (NEON) floating-point
#undef __ARM_SIZEOF_MINIMAL_ENUM     // size of minimal enumeration type: 1 or 4
#undef __ARM_SIZEOF_WCHAR_T          // size of wchar_t: 2 or 4
#undef __ARM_WMMX                    // Wireless MMX extension

//! \def __ARM_ALIGN_MAX_PWR
//! \brief log of maximum alignment of static object

#define __ARM_ALIGN_MAX_PWR 2

//! \def __ARM_ALIGN_MAX_STACK_PWR
//! \brief log of maximum alignment of stack object

#define __ARM_ALIGN_MAX_STACK_PWR 2

//! \def __ARM_FEATURE_CLZ
//! \brief CLZ instruction
//! \def __ARM_FEATURE_FMA
//! \brief floating-point fused multiply-accumulate
//! \def __ARM_FEATURE_IDIV
//! \brief integer divide in hardware (new in 1.1)
//! \def __ARM_FEATURE_LDREX
//! \brief load/store exclusive instructions
//! \def __ARM_FEATURE_QBIT
//! \brief Q (saturation) flag
//! \def __ARM_FEATURE_SAT
//! \brief width-specified saturation instruction
//! \def __ARM_FEATURE_SIMD32
//! \brief 32-bit SIMD instructions (ARM v6)
//! \def __ARM_FP
//! \brief hardware floating-point
//! \def __ARM_FP_FAST
//! \brief accuracy-losing optimizations
//! \def __ARM_FP_FENV_ROUNDING
//! \brief rounding is configurable at runtiume
//! \def __ARM_FP16_FORMAT_ALTERNATIVE
//! \brief 16-bit floating-point, alternative format
//! \def __ARM_FP16_FORMAT_IEEE
//! \brief 16-bit floating-point, IEEE format
//! \def __ARM_NEON
//! \brief Advanced SIMD (NEON) extension
//! \def __ARM_NEON_FP
//! \brief Advanced SIMD (NEON) floating-point
//! \def __ARM_SIZEOF_MINIMAL_ENUM
//! \brief size of minimal enumeration type: 1 or 4
//! \def __ARM_SIZEOF_WCHAR_T
//! \brief size of wchar_t: 2 or 4
//! \def __ARM_WMMX
//! \brief Wireless MMX extension

// We can now define generic features

#ifdef   __thumb__
#ifdef   __thumb2__
#define  __ARM_ARCH_ISA_THUMB 2
#else /*!__thumb2__*/
#define  __ARM_ARCH_ISA_THUMB 1
#endif /*__thumb2__*/
#endif /*__thumb__*/

#define __ARM_SIZEOF_WCHAR_T __SIZEOF_WCHAR_T__

#ifdef   __ARM_ARCH_EXT_IDIV__
#define  __ARM_FEATURE_IDIV   __ARM_ARCH_EXT_IDIV__
#endif

#ifdef   __IWMMXT__
#define  __ARM_WMMX          __IWMMXT__
#endif

// 6.3 Endianness
//
// __ARM_BIG_ENDIAN is defined as 1 if data is stored by default in big-endian
// format. If the macro is not set, data is stored in little-endian format.
// (Aside: the “mixed-endian” format for double-precision numbers, used on some
// very old ARM FPU implementations, is not supported by ACLE or the ARM ABI.)

#if defined (__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
//! \brief Memory is big-endian
#define __ARM_BIG_ENDIAN 1
#endif

// 6.4 ARM and Thumb instruction set architecture and features
//
// References to “the target architecture” refer to the target as configured
// in the tools, for example by appropriate command-line options. This may be
// a subset or intersection of actual targets, in order to produce a binary
// that runs on more than one real architecture. For example, use of specific
// features may be disabled. In some cases, hardware features may be accessible
// from only one or other of ARM or Thumb instruction state. For example, in
// the v5TE and v6 architectures, “DSP” instructions and (where available) VFP
// instructions, are only accessible in ARM state, while in the v7-R
// architecture, hardware integer divide is only accessible from Thumb state.
// Where both states are available, the implementation should set feature test
// macros indicating that the hardware feature is accessible. To provide access
// to the hardware feature, an implementation might override the programmer's
// preference for target instruction set, or generate an interworking call to a
// helper function. This mechanism is outside the scope of ACLE. In cases where
// the implementation is given a hard requirement to use only one state (e.g.
// to support validation, or post-processing) then it should set feature test
// macros only for the hardware features available in that state – as if
// compiling for a core where the other instruction set was not present.
//
// An implementation that allows a user to indicate which functions go into
// which state (either as a hard requirement or a preference) is not required
// to change the settings of architectural feature test macros.

// 6.4.1 ARM/Thumb instruction set architecture

//! \brief __ARM_ARCH is defined as an integer value indicating the current ARM
//! instruction set architecture (e.g. 7 for the ARM v7-A architecture
//! implemented by Cortex-A8 or the ARM v7-M architecture implemented by
//! Cortex-M3). Since ACLE only supports the ARM architecture, this macro would
//! always be defined in an ACLE implementation.
//!
//! Note that the __ARM_ARCH macro is defined even for cores which only support
//! the Thumb instruction set.

#ifndef __ARM_ARCH
#if     defined(__ARM_ARCH_2__)
#define  __ARM_ARCH 2
#elif   defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define  __ARM_ARCH 3
#elif   defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
#define  __ARM_ARCH 4
#elif defined(__ARM_ARCH_5__)    || defined(__ARM_ARCH_5T__)  ||   \
      defined(__ARM_ARCH_5E__)   || defined(__ARM_ARCH_5TE__) ||   \
      defined(__ARM_ARCH_5TEJ__)
#define  __ARM_ARCH 5
#elif defined(__ARM_ARCH_6__)    || defined(__ARM_ARCH_6T__)  ||   \
      defined(__ARM_ARCH_6J__)   || defined(__ARM_ARCH_6K__)  ||   \
      defined(__ARM_ARCH_6Z__)   || defined(__ARM_ARCH_6ZK__) ||   \
      defined(__ARM_ARCH_6T2__)
#define  __ARM_ARCH 6
#elif defined(__ARM_ARCH_7__)    || defined(__ARM_ARCH_7A__)  ||   \
      defined(__ARM_ARCH_7R__)   || defined(__ARM_ARCH_7M__)  ||   \
      defined(__ARM_ARCH_7S__)
#define  __ARM_ARCH 7
#elif defined(__ARM_ARCH_8__)
#define  __ARM_ARCH 8
#else
static_assert(false, "ARM architecture not recognized");
#endif
#endif

//! \brief __ARM_ARCH_ISA_ARM is defined to 1 if the core supports the ARM
//! instruction set. It is not defined for M-profile cores.

#if defined(__ARM_ARCH_7M__)
#undef   __ARM_ARCH_ISA_ARM
#else
#define  __ARM_ARCH_ISA_ARM 1
#endif

//! \brief __ARM_ARCH_ISA_THUMB is defined to 1 if the core supports the
//! original Thumb instruction set (including the v6-M architecture) and 2 if
//! it supports the Thumb-2 instruction set as found in the v6T2 architecture
//! and all v7 architectures.

#if   defined(__ARM_ARCH_5T__)    || defined(__ARM_ARCH_5TE__) || \
      defined(__ARM_ARCH_5TEJ__)  || defined(__ARM_ARCH_6T__)
#define  __ARM_ARCH_ISA_THUMB 1
#elif defined(__ARM_ARCH_6T2__)   || defined(__ARM_ARCH_7__)   || \
      defined(__ARM_ARCH_7A__)    || defined(__ARM_ARCH_7R__)  || \
      defined(__ARM_ARCH_7M__)    || defined(__ARM_ARCH_7S__)
#define  __ARM_ARCH_ISA_THUMB 2
#else
#define  __ARM_ARCH_ISA_THUMB 0
#endif

//! \brief __ARM_32BIT_STATE is defined to 1 if code is being generated for a
//! 32-bit instruction set such as ARM or Thumb.
//! This macro was introduced in ACLE 1.1.

#if defined(__ARM_ARCH_8__)
#define  __ARM_32BIT_STATE 0
#else
#define  __ARM_32BIT_STATE 1
#endif

// 6.4.2 Architectural profile (A, R, M or pre-Cortex)

#if (__ARM_ARCH >= 7)

//! \brief __ARM_ARCH_PROFILE is defined as "A", "R", "M" or "S", or unset, according
//! to the architectural profile of the target. "S" indicates the common subset
//! of "A" and "R".
//!
//! This macro corresponds to the Tag_CPU_arch_profile object build attribute.
//! It may be useful to writers of system code. It is expected in most cases
//! programmers will use more feature-specific tests. The macro is undefined for
//! architectural targets which predate the use of architectural profiles.

#if   defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7S__)
#define  __ARM_ARCH_PROFILE "S"
#elif defined(__ARM_ARCH_7A__)
#define  __ARM_ARCH_PROFILE "A"
#elif defined(__ARM_ARCH_7R__)
#define  __ARM_ARCH_PROFILE "R"
#elif defined(__ARM_ARCH_7M__)
#define  __ARM_ARCH_PROFILE "M"
#elif defined(__ARM_ARCH_8__)
#define  __ARM_ARCH_PROFILE "??"
#else
static_assert(false, "ARM profile not recognized");
#endif

#undef __ARM_ARCH_PROFILE
#endif

// 6.4.3 Unaligned access supported in hardware

// __ARM_FEATURE_UNALIGNED is defined if the target supports unaligned access
// in hardware, at least to the extent of being able to load or store an
// integer word at any alignment with a single instruction. (There may be
// restrictions on load-multiple and floating-point accesses.) Note that
// whether a code generation target permits unaligned access will in general
// depend on the settings of system register bits, so an implementation should
// define this macro to match the user's expectations and intentions. For
// example, a command-line option might be provided to disable the use of
// unaligned access, in which case this macro would not be defined.

// This feature has been set by gcc.

// 6.4.4 LDREX/STREX

// __ARM_FEATURE_LDREX is defined if the load/store-exclusive instructions
// (LDREX/STREX) are supported. Its value is a set of bits indicating available
// widths of the access, as powers of 2. The following bits are used:
//
// Bit Value Access width Instruction
//  0   0x01 byte         LDREXB/STREXB
//  1   0x02 halfword     LDREXH/STREXH
//  2   0x04 word         LDREX/STREX
//  3   0x08 doubleword   LDREXD/STREXD
//
// Other bits are reserved.

//The following values of __ARM_FEATURE_LDREX may occur:
//
// Macro value Access widths                    Example architecture
// (undefined) none                             ARM v5, ARM v6-M
//    0x04     word                             ARM v6
//    0x07     word, halfword, byte             ARM v7-M
//    0x0F     doubleword, word, halfword, byte ARM v6K, ARM v7-A/R
//
// Other values are reserved.
//
// The LDREX/STREX instructions are introduced in recent versions of the ARM
// architecture and supersede the SWP instruction. Where both are available,
// ARM strongly recommends programmers to use LDREX/STREX rather than SWP.
// Note that platforms may choose to make SWP unavailable in user mode and
// emulate it through a trap to a platform routine, or fault it.

#ifndef __ARM_FEATURE_LDREX
#if    defined(__ARM_ARCH_6K__)  || defined(__ARM_ARCH_6ZK__) ||   \
       defined(__ARM_ARCH_7A__)  || defined(__ARM_ARCH_7R__)
//! \brief Indicates that LDREX/STREX instructions are available.
#define __ARM_FEATURE_LDREX 0x0F
#elif  defined(__ARM_ARCH_7M__)
//! \brief Indicates that LDREX/STREX instructions are available.
#define __ARM_FEATURE_LDREX 0x07
#elif (__ARM_ARCH >=6) && !defined(__ARM_ARCH_6M__)
//! \brief Indicates that LDREX/STREX instructions are available.
#define __ARM_FEATURE_LDREX 0x04
#endif
#endif

// 6.4.5 CLZ
//
// __ARM_FEATURE_CLZ is defined to 1 if the CLZ (count leading zeroes)
// instruction is supported in hardware. Note that ACLE provides the __clz()
// family of intrinsics (see 9.2) even when __ARM_FEATURE_CLZ is not defined.

// 6.4.6 Q (saturation) flag
//
// __ARM_FEATURE_QBIT is defined to 1 if the Q (saturation) global flag exists
// and the intrinsics defined in 9.1.1 are available. This flag is used with
// the DSP saturating-arithmetic instructions (such as QADD) and the width-
// specified saturating instructions (SSAT and USAT). Note that either of these
// classes of instructions may exist without the other: for example, v5E has
// only QADD while v7-M has only SSAT.

#if ((defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5TE__) || \
       defined(__ARM_ARCH_5TEJ__) || (__ARM_ARCH == 6) && defined(__arm__)) || \
     (defined(__ARM_ARCH_7M__) && defined(__thumb__)))
//! \brief Indicates that the Q (saturation) global flag exists.
#ifndef __ARM_FEATURE_QBIT
#define __ARM_FEATURE_QBIT
#endif
#endif

// 6.4.7 DSP instructions
//
// __ARM_FEATURE_DSP is defined to 1 if the DSP (v5E) instructions are
// supported and the intrinsics defined in 9.4 are available. These
// instructions include QADD, SMULBB etc. This feature also implies support for
// the Q flag.

#if ((defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) ||    \
       (__ARM_ARCH == 6) && defined(__arm__)) ||                    \
     (defined(__ARM_ARCH_7M__) && defined(__thumb__)))
//! \brief Indicates that the DSP Instructions are available
#ifndef __ARM_FEATURE_DSP
#define __ARM_FEATURE_DSP
#endif
#endif

// 6.4.8 Saturation instructions
//
// __ARM_FEATURE_SAT is defined to 1 if the SSAT and USAT instructions are
// supported and the intrinsics defined in 9.4.1 are available. This feature
// also implies support for the Q flag.

#if (((__ARM_ARCH == 6) && defined(__arm__)) ||                 \
     (defined(__ARM_ARCH_7M__) && defined(__thumb__)))
//! \brief Indicates that the SSAT and USAT Instructions are available
#ifndef __ARM_FEATURE_SAT
#define __ARM_FEATURE_SAT
#endif
#endif

// 6.4.9 32-bit SIMD instructions
//
// __ARM_FEATURE_SIMD32 is defined to 1 if the 32-bit SIMD instructions are
// supported and the intrinsics defined in 9.5 are available. This also implies
// support for the GE global flags which indicate byte-by-byte comparison
// results.

// 6.4.10 Hardware integer divide
//
// __ARM_FEATURE_IDIV is defined to 1 if the target has hardware support for
// 32-bit integer division in all available instruction sets. Signed and
// unsigned versions are both assumed to be available. The intention is to
// allow programmers to choose alternative algorithm implementations depending
// on the likely speed of integer division.
//
// Some older R-profile targets have hardware divide available in the Thumb
// instruction set only. This can be tested for using the following test:
//
// #if __ARM_FEATURE_IDIV || (__ARM_ARCH_PROFILE == ’R’)


// 6.9 Summary of predefined macros
// Macro name Meaning Example See section

//#define __ARM_ALIGN_MAX_PWR log of maximum alignment of static object 20 7.5.2
//#define __ARM_ALIGN_MAX_STACK_PWR log of maximum alignment of stack object 3 7.5.3
//#define __ARM_FEATURE_IDIV integer divide in hardware (new in 1.1) 1 6.4.10
//#define __ARM_FEATURE_QBIT Q (saturation) flag 1 6.4.6, 9.1.1
//#define __ARM_FEATURE_SAT width-specified saturation instructions 1 6.4.8, 9.4.1
//#define __ARM_FEATURE_SIMD32 32-bit SIMD instructions (ARM v6) 1 6.4.8, 9.5
//#define __ARM_FP hardware floating-point 0x0C 6.5.1
//#define __ARM_FP_FAST accuracy-losing optimizations 1 6.6
//#define __ARM_FP_FENV_ROUNDING rounding is configurable at runtiume 1 6.6
//#define __ARM_FP16_FORMAT_ALTERNATIVE 16-bit floating-point, alternative format 1 6.5.2
//#define __ARM_FP16_FORMAT_IEEE 16-bit floating-point, IEEE format 1 6.5.2
//#define __ARM_NEON Advanced SIMD (NEON) extension 1 6.5.4
//#define __ARM_NEON_FP Advanced SIMD (NEON) floating-point 0x04 6.5.5
//#define __ARM_PCS ARM procedure call standard 1 6.7
//#define __ARM_PCS_VFP ARM PCS hardware FP variant in use 1 6.7
//#define __ARM_SIZEOF_MINIMAL_ENUM size of minimal enumeration type: 1 or 4 1 4.1.1
//#define __ARM_SIZEOF_WCHAR_T size of wchar_t: 2 or 4 2 4.1.1
//#define __ARM_WMMX Wireless MMX extension 1 6.5.6

//#define __ARM_FEATURE_CLZ CLZ instruction 1 6.4.5, 9.2
//#define __ARM_FEATURE_DSP DSP instructions (ARM v5E) 1 6.4.6, 9.4
//#define __ARM_FEATURE_FMA floating-point fused multiply-accumulate 1 6.5.3, 9.6

// 8.4 Hints
//
// The intrinsics in this section are available for all targets. They may be
// no-ops (i.e. generate no code, but possibly act as a code motion barrier in
// compilers) on targets where the relevant instructions do not exist. On
// targets where the relevant instructions exist but are implemented as no-ops,
// these intrinsics generate the instructions.

//! This intrinsic expands into an ARM nop instruction.
static inline void __ARM_ACLE_nop(void)
{
    asm volatile("nop" : : : "cc");
}

//! This function implements the ARM wfi instruction.

static inline void __wfi(void)
{
#if defined(__ARM_ARCH_5TE__)
    asm volatile("mcr p15, 0, r0, c7, c0, 4" : : : "cc");
#else
    __ARM_ACLE_nop();
#endif
}

// MCR p15, 0, <Rd>, c7, c0, 4 also arm11 see
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0360f/I1014942.html

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0360f/CHDGIJFB.html
//
// Note For compatibility with existing software, the ARM968E-S processor also
// supports the following instruction for the wait-for-interrupt function:
// MCR p15, 0, <Rd>, c15, c8, 2

// Generates a WFI (wait for interrupt) hint instruction, or nothing. The WFI
// instruction allows (but does not require) the processor to enter a low-power
// state until one of a number of asynchronous events occurs.

//! This function implements the ARM wfe instruction.

static inline void __wfe(void)
{
    __ARM_ACLE_nop();
}

// Generates a WFE (wait for event) hint instruction, or nothing. The WFE
// instruction allows (but does not require) the processor to enter a low-power
// state until some event occurs such as a SEV being issued by another
// processor.

//! This function implements the ARM sev instruction.

static inline void __sev(void)
{
    __ARM_ACLE_nop();
}

// Generates a SEV (send a global event) hint instruction. This causes an event
// to be signaled to all processors in a multiprocessor system. It is a NOP on
// a uniprocessor system.

//! This function implements the ARM sevl instruction.

static inline void __sevl(void)
{
    __ARM_ACLE_nop();
}

// Generates a “send a local event” hint instruction. This causes an event to
// be signaled to only the processor executing this instruction. In a
// multiprocessor system, it is not required to affect the other processors.

//! This function implements the ARM yield instruction.

static inline void __yield(void)
{
    __ARM_ACLE_nop();
}

// Generates a YIELD hint instruction. This enables multithreading software to
// indicate to the hardware that it is performing a task, for example a
// spin-lock, that could be swapped out to improve overall system performance.

//! This function implements the ARM dbg instruction.

static inline void __dbg(
        UNUSED /*constant*/ unsigned int n)
{
    __ARM_ACLE_nop();
}

// Generates a DBG instruction. This provides a hint to debugging and related
// systems. The argument must be a constant integer from 0 to 15 inclusive. See
// implementation documentation for the effect (if any) of this instruction and
// the meaning of the argument. This is available only when compliling for
// AArch32.

// 8.5 Swap

// __swp is available for all targets. This intrinsic expands to a sequence
// equivalent to the deprecated (and possibly unavailable) SWP instruction.

static inline uint32_t __swp_word(
	uint32_t x,
	volatile void *addr)
{
    register uint32_t r = 0;

#ifdef __ARM_FEATURE_LDREX
    do {
	r = *p;
    } while (!__sync_bool_compare_and_swap(addr, r, x));
#else  /*!__ARM_FEATURE_LDREX*/
    asm volatile ("swp %[r], %[x], [ %[addr] ]"
	    : [r] "+r" (r) : [x] "r" (x), [addr] "r" (addr) : );
#endif /*__ARM_FEATURE_LDREX*/

    return r;
}

static inline uint32_t __swp_byte(
	uint32_t x,
	volatile void* addr)
{
    register uint32_t r = 0;

#ifdef __ARM_FEATURE_LDREX
    do {
	r = *p;
    } while (!__sync_bool_compare_and_swap(addr, r, x));
#else  /*!__ARM_FEATURE_LDREX*/
    asm volatile ("swpb %[r], %[x], [ %[addr] ]"
	    : [r] "+r" (r) : [x] "r" (x), [addr] "r" (addr) : );
#endif /*__ARM_FEATURE_LDREX*/

    return r;
}

//! A type-generic macro for the ARM swp instruction (or its replacement)

#define __swp(x, addr)							\
    (( __builtin_types_compatible_p(typeof(*addr), uint32_t))?		\
     __swp_word(x, (void *) addr) :					\
     ((__builtin_types_compatible_p(typeof(*addr), uint8_t))?		\
      __swp_byte(x, (void *)addr) : 0))

// unconditionally stores a new value at the given address, and returns the
// old value.
//
// As with the IA-64/GCC primitives described in 8.2.2, the __swp intrinsic is
// polymorphic. The second argument must provide the address of a byte-sized
// object or an aligned word-sized object and it must be possible to determine
// the size of this object from the argument expression.
//
// This intrinsic is implemented by LDREX/STREX (or LDREXB/STREXB) where
// available, as if by
//
// uint32_t __swp(uint32_t x, volatile uint32_t *p) {
// uint32_t v;
// /* use LDREX/STREX intrinsics not specified by ACLE */
// do v = __ldrex(p); while (__strex(x, p));
// return v;
// }
//
// or alternatively,
//
// uint32_t __swp(uint32_t x, uint32_t *p) {
// uint32_t v;
// /* use IA-64/GCC atomic builtins */
// do v = *p; while (!__sync_bool_compare_and_swap(p, v, x));
// return v;
// }
//
// It is recommended that compilers should produce a downgradeable/upgradeable
// warning on encountering the __swp intrinsic.
//
// Only if load-store exclusive instructions are not available will the
// intrinsic use the SWP/SWPB instructions.
//
// It is strongly recommended to use standard and flexible atomic primitives
// such as those available in the C++ <atomic> header. __swp is provided solely
// to allow straightforward (and possibly automated) replacement of explicit
// use of SWP in inline assembler. SWP is obsolete in the ARM architecture, and
// in recent versions of the architecture, may be configured to be unavailable
// in user-mode. (Aside: unconditional atomic swap is also less powerful as a
// synchronization primitive than load-exclusive/store-conditional.)

// 8.7 NOP

#ifndef  __nop
//! This intrinsic expands into an ARM nop instruction.
#define  __nop	__ARM_ACLE_nop
#endif  /*__nop*/

// generates an unspecified no-op instruction. Note that not all architectures
// provide a distinguished NOP instruction. On those that do, it is unspecified
// whether this intrinsic generates it or another instruction. It is not
// guaranteed that inserting this instruction will increase execution time.


// 9.1.1 The Q (saturation) flag

// The Q flag is a cumulative („sticky‟) saturation bit in the APSR
// (Application Program Status Register) indicating that an operation
// saturated, or in some cases, overflowed. It is set on saturation by most
// intrinsics in the DSP and SIMD intrinsic sets, though some SIMD intrinsics
// feature saturating operations which do not set the Q flag.
//
// [AAPCS 5.1.1] states:
//
//    The N, Z, C, V and Q flags (bits 27-31) and the GE[3:0] bits (bits 16-19)
//    are undefined on entry to or return from a public interface.
//
// Note that this does not state that these bits (in particular the Q flag) are
// undefined across any C/C++ function call boundary – only across a “public
// interface”. The Q and GE bits could be manipulated in well-defined ways by
// local functions, for example when constructing functions to be used in DSP
// algorithms.
//
// Implementations must avoid introducing instructions (such as SSAT/USAT, or
// SMLABB) which affect the Q flag, if the programmer is testing whether the Q
// flag was set by explicit use of intrinsics and if the implementation‟s
// introduction of an instruction may affect the value seen. The implementation
// might choose to model the definition and use (liveness) of the Q flag in the
// way that it models the liveness of any visible variable, or it might
// suppress introduction of Q-affecting instructions in any routine in which
// the Q flag is tested.
//
// ACLE does not define how or whether the Q flag is preserved across function
// call boundaries. (This is seen as an area for future specification.)
//
// In general, the Q flag should appear to C/C++ code in a similar way to the
// standard floating-point cumulative exception flags, as global (or
// thread-local) state that can be tested, set or reset through an API.


#ifdef  __ARM_FEATURE_QBIT
// Returns 1 if the Q flag is set, 0 if not.

//! This function returns the Q flag value
//! \return The returned value is true (!=0) if the Q flag has been set, and
//! false (=0) otherwise.

static inline int __saturation_occurred(void)
{
    return (__get_CONTROL() & (1 << __ARM_Q_FLAG)) != 0;
}

// Sets or resets the Q flag according to the LSB of the value.
// __set_saturation_occurred(0) might be used before performing a sequence of
// operations after which the Q flag is tested. (In general, the Q flag cannot
// be assumed to be unset at the start of a function.)

//! This function allows the Q flag to be set
//! \param[in] q If the lowest bit of q is zero then the Q flag is cleared, if
//! instead the lowest bit is one, then the Q flag is set.

static inline void __set_saturation_occurred(int q)
{
    register uint32_t r = __get_CONTROL() & ~((1 <<__ARM_Q_FLAG) * (q & 1));

    __set_CONTROL(r);
}

// This intrinsic is a hint and may be ignored. It indicates to the compiler
// that the value of the Q flag is not live (needed) at or subsequent to the
// program point at which the intrinsic occurs. It may allow the compiler to
// remove preceding instructions, or to change the instruction sequence in
// such a way as to result in a different value of the Q flag. (A specific
// example is that it may recognize clipping idioms in C code and implement
// them with an instruction such as SSAT that may set the Q flag.)

//! This function is a hint and may be ignored.

static inline void __ignore_saturation(void) {}

//! This function is an addition which resets the Q-bit to 0, and then
//! returns the value of the Q-bit (which ought to be 0).

static inline int __reset_and_saturation_occurred(void)
{
    __set_saturation_occurred(0);

    return __saturation_occurred();
}
#endif //__ARM_FEATURE_QBIT

// 9.1.2 The GE flags
//
// The GE (Greater than or Equal to) flags are four bits in the APSR. They are
// used with the 32-bit SIMD intrinsics described in section 9.5.
//
// There are four GE flags, one for each 8-bit lane of a 32-bit SIMD operation.
// Certain non-saturating 32-bit SIMD intrinsics set the GE bits to indicate
// overflow of addition or subtraction. For 4x8-bit operations the GE bits are
// set one for each byte. For 2x16-bit operations the GE bits are paired
// together, one for the high halfword and the other pair for the low halfword.
// The only supported way to read or use the GE bits (in this specification)
// is by using the __sel intrinsic.

// 9.1.3 Floating-point environment
//
// An implementation should implement the features of <fenv.h> for accessing
// the floating-point runtime environment. Programmers should use this rather
// than accessing the VFP FPSCR directly. For example, on a target supporting
// VFP the cumulative exception flags (IXC, OFC etc.) can be read from the
// FPSCR by using the fetestexcept() function, and the rounding mode (RMode)
// bits can be read using the fegetround() function.
//
// ACLE does not support changing the DN, FZ or AHP bits at runtime.
//
// VFP “short vector” mode (enabled by setting the Stride and Len bits) is
// deprecated, and is unavailable on later VFP implementations. ACLE
// provides no support for this mode.

// 9.2 Miscellaneous data-processing intrinsics

// The following intrinsics perform general data-processing operations. They
// have no effect on global state.
//
// [Note: documentation of the __nop intrinsic has moved to 8.7.]
//
// The 64-bit versions of these intrinsics ("ll" suffix) are new in ACLE 1.1.
// For completeness and to aid portability between LP64 and LLP64 models,
// ACLE 1.1 also defines intrinsics with "l" suffix.


// rotates the argument x right by y bits. y can take any value. These
// intrinsics are available on all targets.

//! This macro rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The size of the rotation.
//! \return The rotated value of x.

#define __ror(x, y) \
    (__builtin_constant_p(y) ? __ror_c((x), ((y)&0x1F)) : __ror_v((x), (y)))

#ifdef __thumb__

//! This helper function rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The compile-time constant size of the rotation.
//! \pre y must be a value between 0 and 31 (decimal)
//! \return The rotated value of x.

static inline uint32_t __ror_c(uint32_t x, uint32_t y)
{
    register uint32_t r;

    switch (y) {
    case 0x00:
        r = x;
        break;
    case 0x01:
        asm volatile ("rors %[r], %[x], #0x01" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x02:
        asm volatile ("rors %[r], %[x], #0x02" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x03:
        asm volatile ("rors %[r], %[x], #0x03" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x04:
        asm volatile ("rors %[r], %[x], #0x04" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x05:
        asm volatile ("rors %[r], %[x], #0x05" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x06:
        asm volatile ("rors %[r], %[x], #0x06" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x07:
        asm volatile ("rors %[r], %[x], #0x07" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x08:
        asm volatile ("rors %[r], %[x], #0x08" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x09:
        asm volatile ("rors %[r], %[x], #0x09" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0A:
        asm volatile ("rors %[r], %[x], #0x0A" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0B:
        asm volatile ("rors %[r], %[x], #0x0B" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0C:
        asm volatile ("rors %[r], %[x], #0x0C" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0D:
        asm volatile ("rors %[r], %[x], #0x0D" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0E:
        asm volatile ("rors %[r], %[x], #0x0E" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x0F:
        asm volatile ("rors %[r], %[x], #0x0F" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x10:
        asm volatile ("rors %[r], %[x], #0x10" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x11:
        asm volatile ("rors %[r], %[x], #0x11" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x12:
        asm volatile ("rors %[r], %[x], #0x12" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x13:
        asm volatile ("rors %[r], %[x], #0x13" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x14:
        asm volatile ("rors %[r], %[x], #0x14" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x15:
        asm volatile ("rors %[r], %[x], #0x15" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x16:
        asm volatile ("rors %[r], %[x], #0x16" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x17:
        asm volatile ("rors %[r], %[x], #0x17" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x18:
        asm volatile ("rors %[r], %[x], #0x18" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x19:
        asm volatile ("rors %[r], %[x], #0x19" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1A:
        asm volatile ("rors %[r], %[x], #0x1A" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1B:
        asm volatile ("rors %[r], %[x], #0x1B" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1C:
        asm volatile ("rors %[r], %[x], #0x1C" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1D:
        asm volatile ("rors %[r], %[x], #0x1D" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1E:
        asm volatile ("rors %[r], %[x], #0x1E" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    case 0x1F:
        asm volatile ("rors %[r], %[x], #0x1F" : [r] "=r" (r) : [x] "r" (x) :);
        break;
    default:
        r = x;
        break;
    }

    return r;
}

//! This helper function rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The non-compile-time constant size of the rotation.
//! \return The rotated value of x.

static inline uint32_t __ror_v(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("rors %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y & 0xFF) : );

    return r;
}

#else /*!__thumb__*/

//! This helper function rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The compile-time constant size of the rotation.
//! \pre y must be a value between 0 and 31 (decimal)
//! \return The rotated value of x.

static inline uint32_t __ror_c(uint32_t x, uint32_t y)
{
    register uint32_t r;

    switch (y) {
    case 0x00:
        r = x;
        break;
    case 0x01:
        asm volatile ("ror %[r], %[x], #0x01" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x02:
        asm volatile ("ror %[r], %[x], #0x02" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x03:
        asm volatile ("ror %[r], %[x], #0x03" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x04:
        asm volatile ("ror %[r], %[x], #0x04" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x05:
        asm volatile ("ror %[r], %[x], #0x05" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x06:
        asm volatile ("ror %[r], %[x], #0x06" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x07:
        asm volatile ("ror %[r], %[x], #0x07" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x08:
        asm volatile ("ror %[r], %[x], #0x08" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x09:
        asm volatile ("ror %[r], %[x], #0x09" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0A:
        asm volatile ("ror %[r], %[x], #0x0A" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0B:
        asm volatile ("ror %[r], %[x], #0x0B" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0C:
        asm volatile ("ror %[r], %[x], #0x0C" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0D:
        asm volatile ("ror %[r], %[x], #0x0D" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0E:
        asm volatile ("ror %[r], %[x], #0x0E" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x0F:
        asm volatile ("ror %[r], %[x], #0x0F" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x10:
        asm volatile ("ror %[r], %[x], #0x10" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x11:
        asm volatile ("ror %[r], %[x], #0x11" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x12:
        asm volatile ("ror %[r], %[x], #0x12" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x13:
        asm volatile ("ror %[r], %[x], #0x13" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x14:
        asm volatile ("ror %[r], %[x], #0x14" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x15:
        asm volatile ("ror %[r], %[x], #0x15" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x16:
        asm volatile ("ror %[r], %[x], #0x16" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x17:
        asm volatile ("ror %[r], %[x], #0x17" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x18:
        asm volatile ("ror %[r], %[x], #0x18" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x19:
        asm volatile ("ror %[r], %[x], #0x19" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1A:
        asm volatile ("ror %[r], %[x], #0x1A" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1B:
        asm volatile ("ror %[r], %[x], #0x1B" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1C:
        asm volatile ("ror %[r], %[x], #0x1C" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1D:
        asm volatile ("ror %[r], %[x], #0x1D" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1E:
        asm volatile ("ror %[r], %[x], #0x1E" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    case 0x1F:
        asm volatile ("ror %[r], %[x], #0x1F" : [r] "=r" (r) : [x] "r" (x) : );
        break;
    default:
        r = x;
        break;
    }

    return r;
}

//! This helper function rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The non-compile-time constant size of the rotation.
//! \return The rotated value of x.

static inline uint32_t __ror_v(uint32_t x, uint32_t y)
{
    register uint32_t r;

    asm volatile ("ror %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y & 0xFF) : );

    return r;
}
#endif /*__thumb__*/

// rorll(x, n)
// = [x_{n-1}, x_{n-2} ... x_0, x_63, x_62 ... x_{n+1}, x_n]
//
// if n < 32 then
// ror(xlo, n) = [x_{n-1},    x_{n-2}    ... x_0,  x_31 ...x_{n+1},    x_n]
// ror(xhi, n) = [x_{n+32-1}, x_{n+32-2} ... x_32, x_63 ...x_{n+32+1}, x_{n+32}]

//! This macro rotates the 64-bit argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The size of the rotation.
//! \return The rotated value of x.

static inline uint64_t __rorll(
	uint64_t x,
	uint32_t y)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } s;
	uint64_t r;
    } z;
    register uint32_t t;
    register uint32_t lo_mask = (1 << (32-y)) - 1;         // y zeros followed by 32-y ones
    register uint32_t hi_mask = ((1 << y) - 1) << (32-y);  // 32-y ones followed by y zeros

    z.r = x;

    z.s.lo = __ror(z.s.lo, y);
    t      = __ror(z.s.hi, y);

    z.s.hi = (z.s.lo & hi_mask) | (t & lo_mask);
    z.s.lo = (t & hi_mask) | (z.s.lo & lo_mask);

    if ((y & 0x20) != 0) {
        t      = z.s.lo;
        z.s.lo = z.s.hi;
        z.s.hi = t;
    }

    return z.r;
}

//! This macro rotates the argument x right by y bits.
//! \param[in] x The value to be rotated.
//! \param[in] y The size of the rotation.
//! \return The rotated value of x.

#define __rorl(x, y) \
    ((__builtin_types_compatible_p(unsigned long, uint32_t)) ?	\
	    __ror((x), (y)) : __rorll((x), (y)))

// returns the number of leading zero bits in x. When x is zero it returns the
// argument width, i.e. 32 or 64. These intrinsics are available on all
// targets. On targets without the CLZ instruction it should be implemented as
// an instruction sequence or a call to such a sequence. A suitable sequence
// can be found in [Warren] (fig. 5-7). Hardware support for these intrinsics
// is indicated by __ARM_FEATURE_CLZ.

//! This function counts the number of leading zeros in a word.
//! \param[in] x An unsigned integer.
//! \return The number of leading zeros in x.

static inline unsigned int __clz(
	uint32_t x)
{
    return (unsigned int) __builtin_clz(x);
}

//! This function counts the number of leading zeros in a long word.
//! \param[in] x An unsigned long integer.
//! \return The number of leading zeros in x.

static inline unsigned int __clzl(
	unsigned long x)
{
    return (unsigned int) __builtin_clzl(x);
}

//! This function counts the number of leading zeros in a long long word.
//! \param[in] x An unsigned long long integer.
//! \return The number of leading zeros in x.

static inline unsigned int __clzll(
	uint64_t x)
{
    return (unsigned int) __builtin_clzll(x);
}

//! This function counts the number of leading sign-bits in a word.
//! \param[in] x An unsigned integer.
//! \return The number of leading sign-bits in x.

static inline unsigned int __cls(
	uint32_t x)
{
    return (unsigned int) __builtin_clrsb(x);
}

//! This function counts the number of leading sign-bits in a long word.
//! \param[in] x An unsigned long integer.
//! \return The number of leading sign-bits in x.

static inline unsigned int __clsl(
	unsigned long x)
{
    return (unsigned int) __builtin_clrsbl(x);
}

//! This function counts the number of leading zeros in a long long word.
//! \param[in] x An unsigned long long integer.
//! \return The number of leading sign-bits in x.

static inline unsigned int __clsll(
	uint64_t x)
{
    return (unsigned int) __builtin_clrsbll(x);
}

// returns the number of leading sign bits in x. When x is zero it returns the
// argument width, i.e. 32 or 64. These intrinsics are available on all
// targets. On targets without the CLZ instruction it should be implemented as
// an instruction sequence or a call to such a sequence. Fast hardware
// implementation (using a CLS instruction or a short code sequence involving
// the CLZ instruction) is indicated by __ARM_FEATURE_CLZ. New in ACLE 1.1.


// reverses the byte order within a word or doubleword. These intrinsics are
// available on all targets and should be expanded to an efficient straight-
// line code sequence on targets without byte reversal instructions.

//! This function reverses the byte order in a word.
//! \param[in] x The word to be byte-order reversed.
//! \return The byte-order reversed result.

static inline uint32_t __rev(
	uint32_t x)
{
    register union {
	uint32_t u;
	int32_t s;
    } t;

    t.s = __builtin_bswap32(x);
    return t.u;
}

//! This function reverses the byte order in a long long word.
//! \param[in] x The long long word to be byte-order reversed.
//! \return The byte-order reversed result.

static inline uint64_t __revll(
	uint64_t x)
{
    register union {
	uint64_t u;
	int64_t s;
    } t;

    t.s = __builtin_bswap64(x);
    return t.u;
}

//! This macro provides a byte order reversal function for unsigned long
//! integers.
//! \param[in] x The item to be byte-order reversed.
//! \return The byte-order reversed result.

#define __revl(x) \
    ((__builtin_types_compatible_p(unsigned long, uint32_t)) ?	\
	    __rev(x) : __revll(x))

// reverses the byte order within each halfword of a word. For example,
// 0x12345678 becomes 0x34127856. These intrinsics are available on all targets
// and should be expanded to an efficient straight-line code sequence on
// targets without byte reversal instructions.

//! This function reverses each byte in a half-word order for unsigned integers.
//! \param[in] x The item to be reversed.
//! \return The reversed result.

static inline uint32_t __rev16(
	uint32_t x)
{
    register union {
	struct { uint8_t b0; uint8_t b1; uint8_t b2; uint8_t b3; } bs;
	uint32_t w;
    } r, t;

    t.w = x;
    r.bs.b0 = t.bs.b1;
    r.bs.b1 = t.bs.b0;
    r.bs.b2 = t.bs.b3;
    r.bs.b3 = t.bs.b2;
    return r.w;
}

//! This function reverses each byte in a half-word order for unsigned
//! long long integers.
//! \param[in] x The item to be reversed.
//! \return The reversed result.

static inline uint64_t __rev16ll(
	uint64_t x)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } w;
	uint64_t dw;
    } r, t;

    t.dw = x;
    r.w.lo = __rev16(t.w.lo);
    r.w.hi = __rev16(t.w.hi);
    return r.dw;
}

//! This macro provides a half-word order reversal function for unsigned
//! long integers.
//! \param[in] x The item to be reversed.
//! \return The reversed result.

#define __rev16l(x) \
    ((__builtin_types_compatible_p(unsigned long, uint32_t)) ?	\
	    __rev16(x) : __rev16ll(x))

// reverses the byte order in a 16-bit value and returns the (sign-extended)
// result. For example, 0x00000080 becomes 0xFFFF8000. This intrinsic is
// available on all targets and should be expanded to an efficient straight-
// line code sequence on targets without byte reversal instructions.

//! This function byte-reverses a half-word.
//! \param[in] x The item to be reversed.
//! \return The reversed result.

static inline int16_t __revsh(
	int16_t x)
{
    register union {
	struct { uint8_t lo; uint8_t hi; } bs;
	int16_t hw;
    } r, t;

    t.hw = x;
    r.bs.lo = t.bs.hi;
    r.bs.hi = t.bs.lo;
    return r.hw;
}

// reverses the bits in x. These intrinsics are only available on targets with
// the RBIT instruction.

#if (__ARM_ARCH >= 6 && __ARM_ARCH_ISA_THUMB >= 2) || __ARM_ARCH >= 7
 /* RBIT is available */
static inline uint32_t __rbit(
	uint32_t x)
{
    register uint32_t r;

    asm volatile("rbit %[r], %[x]" : [r] "=r" (r) : [x] "r" (x) : );
    return r;
}

static inline uint64_t __rbitll(
	uint64_t x)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } w;
	uint64_t dw;
    } r;

    r.dw = x;
    r.w.lo = __rbit(r.w.hi);
    r.w.hi = __rbit(r.w.lo);
    return r.dw;
}

#define __rbitl(x) \
    ((__builtin_types_compatible_p(unsigned long, uint32_t)) ?	\
	    __rbit(x) : __rbitll(x))
#endif /* RBIT is available */

// 9.3 16-bit multiplications

#ifdef __ARM_FEATURE_DSP

//! This function multiplies two 16-bit signed integers, each from the lower
//! half word
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smulbb(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smulbb %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function multiplies two 16-bit signed integers.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smulbt(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smulbt %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function multiplies two 16-bit signed integers.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smultb(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smultb %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function multiplies two 16-bit signed integers.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smultt(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smultt %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function multiplies a 32-bit signed integer by the lower 16-bit
//! signed integer of the second argument, giving the topmost 32-bits of the
//! answer.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smulwb(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smulwb %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function multiplies a 32-bit signed integer by the higher 16-bit
//! signed integer of the second argument, giving the topmost 32-bits of the
//! answer.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return signed result.

static inline int32_t __smulwt(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("smulwt %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}
#endif /*__ARM_FEATURE_DSP*/

// 9.4 Saturating intrinsics

// 9.4.1 Width-specified saturation intrinsics

#ifdef __ARM_FEATURE_SAT

static inline uint32_t __ssat_c(
	uint32_t x,
	uint32_t n)
{
    register uint32_t r;

    if (__builtin_constant_p(n)) {
        switch (n) {
        case 0x00:
            r = x;
            break;
        case 0x01:
            asm volatile ("ssat %[r], #0x01, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x02:
            asm volatile ("ssat %[r], #0x02, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x03:
            asm volatile ("ssat %[r], #0x03, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x04:
            asm volatile ("ssat %[r], #0x04, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x05:
            asm volatile ("ssat %[r], #0x05, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x06:
            asm volatile ("ssat %[r], #0x06, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x07:
            asm volatile ("ssat %[r], #0x07, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x08:
            asm volatile ("ssat %[r], #0x08, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x09:
            asm volatile ("ssat %[r], #0x09, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0A:
            asm volatile ("ssat %[r], #0x0A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0B:
            asm volatile ("ssat %[r], #0x0B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0C:
            asm volatile ("ssat %[r], #0x0C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0D:
            asm volatile ("ssat %[r], #0x0D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0E:
            asm volatile ("ssat %[r], #0x0E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0F:
            asm volatile ("ssat %[r], #0x0F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x10:
            asm volatile ("ssat %[r], #0x10, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x11:
            asm volatile ("ssat %[r], #0x11, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x12:
            asm volatile ("ssat %[r], #0x12, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x13:
            asm volatile ("ssat %[r], #0x13, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x14:
            asm volatile ("ssat %[r], #0x14, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x15:
            asm volatile ("ssat %[r], #0x15, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x16:
            asm volatile ("ssat %[r], #0x16, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x17:
            asm volatile ("ssat %[r], #0x17, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x18:
            asm volatile ("ssat %[r], #0x18, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x19:
            asm volatile ("ssat %[r], #0x19, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1A:
            asm volatile ("ssat %[r], #0x1A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1B:
            asm volatile ("ssat %[r], #0x1B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1C:
            asm volatile ("ssat %[r], #0x1C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1D:
            asm volatile ("ssat %[r], #0x1D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1E:
            asm volatile ("ssat %[r], #0x1E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1F:
            asm volatile ("ssat %[r], #0x1F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        default:
            r = x;
            break;
        }
    } else {
        r = x;
    }
    return r;
}

static inline uint32_t __usat_c(
	uint32_t x,
	uint32_t n)
{
    register uint32_t r;

    if (__builtin_constant_p(n)) {
        switch (n) {
        case 0x00:
            r = x;
            break;
        case 0x01:
            asm volatile ("usat %[r], #0x01, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x02:
            asm volatile ("usat %[r], #0x02, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x03:
            asm volatile ("usat %[r], #0x03, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x04:
            asm volatile ("usat %[r], #0x04, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x05:
            asm volatile ("usat %[r], #0x05, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x06:
            asm volatile ("usat %[r], #0x06, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x07:
            asm volatile ("usat %[r], #0x07, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x08:
            asm volatile ("usat %[r], #0x08, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x09:
            asm volatile ("usat %[r], #0x09, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0A:
            asm volatile ("usat %[r], #0x0A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0B:
            asm volatile ("usat %[r], #0x0B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0C:
            asm volatile ("usat %[r], #0x0C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0D:
            asm volatile ("usat %[r], #0x0D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0E:
            asm volatile ("usat %[r], #0x0E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0F:
            asm volatile ("usat %[r], #0x0F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x10:
            asm volatile ("usat %[r], #0x10, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x11:
            asm volatile ("usat %[r], #0x11, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x12:
            asm volatile ("usat %[r], #0x12, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x13:
            asm volatile ("usat %[r], #0x13, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x14:
            asm volatile ("usat %[r], #0x14, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x15:
            asm volatile ("usat %[r], #0x15, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x16:
            asm volatile ("usat %[r], #0x16, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x17:
            asm volatile ("usat %[r], #0x17, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x18:
            asm volatile ("usat %[r], #0x18, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x19:
            asm volatile ("usat %[r], #0x19, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1A:
            asm volatile ("usat %[r], #0x1A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1B:
            asm volatile ("usat %[r], #0x1B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1C:
            asm volatile ("usat %[r], #0x1C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1D:
            asm volatile ("usat %[r], #0x1D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1E:
            asm volatile ("usat %[r], #0x1E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1F:
            asm volatile ("usat %[r], #0x1F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        default:
            r = x;
            break;
        }
    } else {
        r = x;
    }
    return r;
}

#define __ssat(x, n) __ssat_c(x, n)
#define __usat(x, n) __usat_c(x, n)

#endif /*__ARM_FEATURE_SAT*/

// 9.4.2 Saturating addition and subtraction intrinsics

#ifdef __ARM_FEATURE_DSP

//! This function adds two 32-bit signed integers, saturating the result.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return x+y.

static inline int32_t __qadd(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("qadd %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function adds two 32-bit signed integers, saturating the result.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return x-y.

static inline int32_t __qsub(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("qsub %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function doubles the 32-bit signed integer, saturating the result.
//! \param[in] x first argument.
//! \return 2*x.

static inline int32_t __qdbl(
	int32_t x)
{
    register int32_t r;

    asm volatile("qadd %[r], %[x], %[x]"
	    : [r] "=r" (r) : [x] "r" (x) : );
    return r;
}

#ifdef __ARM_ACLE_EXTENSIONS

//! This function adds two 32-bit signed integers, saturating the result.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return x+2*y.

static inline int32_t __qdadd(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("qdadd %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}

//! This function subtracts two 32-bit signed integers, saturating the result.
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \return x-2*y.

static inline int32_t __qdsub(
	int32_t x,
	int32_t y)
{
    register int32_t r;

    asm volatile("qdsub %[r], %[x], %[y]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y) : );
    return r;
}
#endif /*__ARM_ACLE_EXTENSIONS*/
#endif /*__ARM_FEATURE_DSP*/

// 9.4.3 Accumulating multiplications
#ifdef __ARM_FEATURE_DSP

//! This function performs a 16x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies two 16-bit signed integers, the low halfwords of the first two
//! operands, and adds to the third operand. Sets the Q flag if the addition
//! overflows. (Note that the addition is the usual 32-bit modulo addition which
//! wraps on overflow, not a saturating addition. The multiplication cannot
//! overflow.)
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlabb(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlabb %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

//! This function performs a 16x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies the low halfword of the first operand and the high halfword of
// the second operand, and adds to the third operand, as for __smlabb.
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlabt(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlabt %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

//! This function performs a 16x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies the high halfword of the first operand and the low halfword of
//! the second operand, and adds to the third operand, as for __smlabb.
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlatb(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlatb %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

//! This function performs a 16x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies the high halfwords of the first two operands and adds to the
//! third operand, as for __smlabb.
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlatt(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlatt %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

//! This function performs a 32x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies the 32-bit signed first operand with the low halfword (as a
//! 16-bit signed integer) of the second operand. Adds the top 32 bits of the
//! 48-bit product to the third operand. Sets the Q flag if the addition
//! overflows. (See note for __smlabb.)
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlawb(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlawb %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

//! This function performs a 32x16 multiply-accumulate, saturating the addition.
//!
//! Multiplies the 32-bit signed first operand with the high halfword (as a
//! 16-bit signed integer) of the second operand and adds the top 32 bits of
//! the 48-bit result to the third operand as for __smlawb.
//!
//! \param[in] x first argument.
//! \param[in] y second argument.
//! \param[in] acc accumulation argument.
//! \return x*y+acc.

static inline int32_t __smlawt(
	int32_t x,
	int32_t y,
	int32_t acc)
{
    register int32_t r;

    asm volatile("smlawt %[r], %[x], %[y], %[a]"
	    : [r] "=r" (r) : [x] "r" (x), [y] "r" (y), [a] "r" (acc) : );
    return r;
}

#ifdef __ARM_ACLE_EXTENSIONS

//! This function performs a 32x32 multiply-accumulate.
//!
//! Multiplies the low halfwords (each treated as a 16-bit signed integer) of
//! the second and third operands and adds to the accumulator held in the first
//! argument.
//!
//! \param[in] acc accumulation argument.
//! \param[in] x second argument.
//! \param[in] y third argument.
//! \return x*y+acc.

static inline int64_t __smlalbb(
	int64_t acc,
	int32_t x,
	int32_t y)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } s_rep;
	int64_t i_rep;
    } r;

    r.i_rep = acc;
    asm volatile("smlalbb %[r_lo], %[r_hi], %[x], %[y]"
	    : [r_lo] "+r" (r.s_rep.lo), [r_hi] "+r" (r.s_rep.hi)
	    : [x] "r" (x), [y] "r" (y) : );
    return r.i_rep;
}

//! This function performs a 32x32 multiply-accumulate.
//!
//! Multiplies the two halfwords (each treated as a 16-bit signed integer) of
//! the second and third operands and adds to the accumulator held in the first
//! argument. The low part of the second argument, and high part of the third
//! argument are used.
//!
//! \param[in] acc accumulation argument.
//! \param[in] x second argument.
//! \param[in] y third argument.
//! \return x*y+acc.

static inline int64_t __smlalbt(
	int64_t acc,
	int32_t x,
	int32_t y)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } s_rep;
	int64_t i_rep;
    } r;

    r.i_rep = acc;
    asm volatile("smlalbt %[r_lo], %[r_hi], %[x], %[y]"
	    : [r_lo] "+r" (r.s_rep.lo), [r_hi] "+r" (r.s_rep.hi)
	    : [x] "r" (x), [y] "r" (y) : );
    return r.i_rep;
}

//! This function performs a 32x32 multiply-accumulate.
//!
//! Multiplies the two halfwords (each treated as a 16-bit signed integer) of
//! the second and third operands and adds to the accumulator held in the first
//! argument. The high part of the second argument, and low part of the third
//! argument are used.
//!
//! \param[in] acc accumulation argument.
//! \param[in] x second argument.
//! \param[in] y third argument.
//! \return x*y+acc.

static inline int64_t __smlaltb(
	int64_t acc,
	int32_t x,
	int32_t y)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } s_rep;
	int64_t i_rep;
    } r;

    r.i_rep = acc;
    asm volatile("smlaltb %[r_lo], %[r_hi], %[x], %[y]"
	    : [r_lo] "+r" (r.s_rep.lo), [r_hi] "+r" (r.s_rep.hi)
	    : [x] "r" (x), [y] "r" (y) : );
    return r.i_rep;
}

//! This function performs a 32x32 multiply-accumulate.
//!
//! Multiplies the high halfwords (each treated as a 16-bit signed integer) of
//! the second and third operands and adds to the accumulator held in the first
//! argument
//!
//! \param[in] acc accumulation argument.
//! \param[in] x second argument.
//! \param[in] y third argument.
//! \return x*y+acc.

static inline int64_t __smlaltt(
	int64_t acc,
	int32_t x,
	int32_t y)
{
    register union {
	struct { uint32_t lo; uint32_t hi; } s_rep;
	int64_t i_rep;
    } r;

    r.i_rep = acc;
    asm volatile("smlaltt %[r_lo], %[r_hi], %[x], %[y]"
	    : [r_lo] "+r" (r.s_rep.lo), [r_hi] "+r" (r.s_rep.hi)
	    : [x] "r" (x), [y] "r" (y) : );
    return r.i_rep;
}

#endif /*__ARM_ACLE_EXTENSIONS*/
#endif /*__ARM_FEATURE_DSP*/

// 9.5   32-bit SIMD intrinsics
// 9.5.2 Data types for 32-bit SIMD intrinsics

#ifdef __ARM_FEATURE_SIMD32
typedef int32_t  int16x2_t;
typedef uint32_t uint16x2_t;
typedef int32_t  int8x4_t;
typedef uint32_t uint8x4_t;
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.3 Use of the Q flag by 32-bit SIMD intrinsics

// 9.5.4 Parallel 16-bit saturation

#ifdef __ARM_FEATURE_SIMD32
// Available on 6, and 7em architectures

static inline int16x2_t __ssat16_c(
	int16x2_t x,
	uint32_t n)
{
    register int16x2_t r;

    if (__builtin_constant_p(n)) {
        switch (n) {
        case 0x00:
            r = x;
            break;
        case 0x01:
            asm volatile ("ssat16 %[r], #0x01, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x02:
            asm volatile ("ssat16 %[r], #0x02, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x03:
            asm volatile ("ssat16 %[r], #0x03, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x04:
            asm volatile ("ssat16 %[r], #0x04, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x05:
            asm volatile ("ssat16 %[r], #0x05, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x06:
            asm volatile ("ssat16 %[r], #0x06, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x07:
            asm volatile ("ssat16 %[r], #0x07, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x08:
            asm volatile ("ssat16 %[r], #0x08, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x09:
            asm volatile ("ssat16 %[r], #0x09, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0A:
            asm volatile ("ssat16 %[r], #0x0A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0B:
            asm volatile ("ssat16 %[r], #0x0B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0C:
            asm volatile ("ssat16 %[r], #0x0C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0D:
            asm volatile ("ssat16 %[r], #0x0D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0E:
            asm volatile ("ssat16 %[r], #0x0E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0F:
            asm volatile ("ssat16 %[r], #0x0F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x10:
            asm volatile ("ssat16 %[r], #0x10, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x11:
            asm volatile ("ssat16 %[r], #0x11, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x12:
            asm volatile ("ssat16 %[r], #0x12, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x13:
            asm volatile ("ssat16 %[r], #0x13, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x14:
            asm volatile ("ssat16 %[r], #0x14, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x15:
            asm volatile ("ssat16 %[r], #0x15, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x16:
            asm volatile ("ssat16 %[r], #0x16, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x17:
            asm volatile ("ssat16 %[r], #0x17, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x18:
            asm volatile ("ssat16 %[r], #0x18, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x19:
            asm volatile ("ssat16 %[r], #0x19, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1A:
            asm volatile ("ssat16 %[r], #0x1A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1B:
            asm volatile ("ssat16 %[r], #0x1B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1C:
            asm volatile ("ssat16 %[r], #0x1C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1D:
            asm volatile ("ssat16 %[r], #0x1D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1E:
            asm volatile ("ssat16 %[r], #0x1E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1F:
            asm volatile ("ssat16 %[r], #0x1F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        default:
            r = x;
            break;
        }
    } else {
        r = x;
    }
    return r;
}

static inline int16x2_t __usat16_c(
	int16x2_t x,
	uint32_t n)
{
    register int16x2_t r;

    if (__builtin_constant_p(n)) {
        switch (n) {
        case 0x00:
            r = x;
            break;
        case 0x01:
            asm volatile ("usat16 %[r], #0x01, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x02:
            asm volatile ("usat16 %[r], #0x02, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x03:
            asm volatile ("usat16 %[r], #0x03, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x04:
            asm volatile ("usat16 %[r], #0x04, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x05:
            asm volatile ("usat16 %[r], #0x05, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x06:
            asm volatile ("usat16 %[r], #0x06, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x07:
            asm volatile ("usat16 %[r], #0x07, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x08:
            asm volatile ("usat16 %[r], #0x08, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x09:
            asm volatile ("usat16 %[r], #0x09, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0A:
            asm volatile ("usat16 %[r], #0x0A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0B:
            asm volatile ("usat16 %[r], #0x0B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0C:
            asm volatile ("usat16 %[r], #0x0C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0D:
            asm volatile ("usat16 %[r], #0x0D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0E:
            asm volatile ("usat16 %[r], #0x0E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x0F:
            asm volatile ("usat16 %[r], #0x0F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x10:
            asm volatile ("usat16 %[r], #0x10, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x11:
            asm volatile ("usat16 %[r], #0x11, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x12:
            asm volatile ("usat16 %[r], #0x12, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x13:
            asm volatile ("usat16 %[r], #0x13, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x14:
            asm volatile ("usat16 %[r], #0x14, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x15:
            asm volatile ("usat16 %[r], #0x15, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x16:
            asm volatile ("usat16 %[r], #0x16, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x17:
            asm volatile ("usat16 %[r], #0x17, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x18:
            asm volatile ("usat16 %[r], #0x18, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x19:
            asm volatile ("usat16 %[r], #0x19, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1A:
            asm volatile ("usat16 %[r], #0x1A, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1B:
            asm volatile ("usat16 %[r], #0x1B, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1C:
            asm volatile ("usat16 %[r], #0x1C, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1D:
            asm volatile ("usat16 %[r], #0x1D, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1E:
            asm volatile ("usat16 %[r], #0x1E, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        case 0x1F:
            asm volatile ("usat16 %[r], #0x1F, %[x]" : [r] "=r" (r) : [x] "r" (x) : );
            break;
        default:
            r = x;
            break;
        }
    } else {
        r = x;
    }
    return r;
}

#define __ssat16(x, n) __ssat16_c(x, n)
#define __usat16(x, n) __usat16_c(x, n)

// int16x2_t __ssat16(int16x2_t, /*constant*/ unsigned int)
// int16x2_t __usat16(int16x2_t, /*constant*/ unsigned int);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.5 Packing and unpacking
#ifdef __ARM_FEATURE_SIMD32
int16x2_t  __sxtab16 (int16x2_t, int8x4_t);
int16x2_t  __sxtb16  (int8x4_t);
uint16x2_t __uxtab16 (uint16x2_t, uint8x4_t);
uint16x2_t __uxtb16  (uint8x4_t);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.6 Parallel selection
#ifdef __ARM_FEATURE_SIMD32
uint8x4_t __sel(uint8x4_t, uint8x4_t);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.7 Parallel 8-bit addition and subtraction

// These intrinsics are available when __ARM_FEATURE_SIMD32 is defined. Each
// intrinsic performs 8-bit parallel addition or subtraction. In some cases
// the result may be halved or saturated.

#ifdef __ARM_FEATURE_SIMD32
int8x4_t  __qadd8(int8x4_t, int8x4_t);
int8x4_t  __qsub8(int8x4_t, int8x4_t);
int8x4_t  __sadd8(int8x4_t, int8x4_t);
int8x4_t  __shadd8(int8x4_t, int8x4_t);
int8x4_t  __shsub8(int8x4_t, int8x4_t);
int8x4_t  __ssub8(int8x4_t, int8x4_t);
uint8x4_t __uadd8(uint8x4_t, uint8x4_t);
uint8x4_t __uhadd8(uint8x4_t, uint8x4_t);
uint8x4_t __uhsub8(uint8x4_t, uint8x4_t);
uint8x4_t __uqadd8(uint8x4_t, uint8x4_t);
uint8x4_t __uqsub8(uint8x4_t, uint8x4_t);
uint8x4_t __usub8(uint8x4_t, uint8x4_t);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.8 Sum of 8-bit absolute differences

// These intrinsics are available when __ARM_FEATURE_SIMD32 is defined. They
// perform an 8-bit sum-of-absolute differences operation, typically used in
// motion estimation.

#ifdef __ARM_FEATURE_SIMD32

// Performs 4x8-bit unsigned subtraction, and adds the absolute values of the
// differences together, returning the result as a single unsigned integer.

uint32_t __usad8(uint8x4_t, uint8x4_t);

// Performs 4x8-bit unsigned subtraction, adds the absolute values of the
// differences together, and adds the result to the third operand.

uint32_t __usada8(uint8x4_t, uint8x4_t, uint32_t);

#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.9 Parallel 16-bit addition and subtraction
#ifdef __ARM_FEATURE_SIMD32
int16x2_t  __qadd16  (int16x2_t, int16x2_t);
int16x2_t  __qasx    (int16x2_t, int16x2_t);
int16x2_t  __qsax    (int16x2_t, int16x2_t);
int16x2_t  __qsub16  (int16x2_t, int16x2_t);
int16x2_t  __sadd16  (int16x2_t, int16x2_t);
int16x2_t  __sasx    (int16x2_t, int16x2_t);
int16x2_t  __shadd16 (int16x2_t, int16x2_t);
int16x2_t  __shasx   (int16x2_t, int16x2_t);
int16x2_t  __shsax   (int16x2_t, int16x2_t);
int16x2_t  __shsub16 (int16x2_t, int16x2_t);
int16x2_t  __ssax    (int16x2_t, int16x2_t);
int16x2_t  __ssub16  (int16x2_t, int16x2_t);
uint16x2_t __uadd16  (uint16x2_t, uint16x2_t);
uint16x2_t __uasx    (uint16x2_t, uint16x2_t);
uint16x2_t __uhadd16 (uint16x2_t, uint16x2_t);
uint16x2_t __uhasx   (uint16x2_t, uint16x2_t);
uint16x2_t __uhsax   (uint16x2_t, uint16x2_t);
uint16x2_t __uhsub16 (uint16x2_t, uint16x2_t);
uint16x2_t __uqadd16 (uint16x2_t, uint16x2_t);
uint16x2_t __uqasx   (uint16x2_t, uint16x2_t);
uint16x2_t __uqsax   (uint16x2_t, uint16x2_t);
uint16x2_t __uqsub16 (uint16x2_t, uint16x2_t);
uint16x2_t __usax    (uint16x2_t, uint16x2_t);
uint16x2_t __usub16  (uint16x2_t, uint16x2_t);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.5.10 Parallel 16-bit multiplication
#ifdef __ARM_FEATURE_SIMD32
int32_t __smlad   (int16x2_t, int16x2_t, int32_t);
int32_t __smladx  (int16x2_t, int16x2_t, int32_t);
int64_t __smlald  (int16x2_t, int16x2_t, int64_t);
int64_t __smlaldx (int16x2_t, int16x2_t, int64_t);
int32_t __smlsd   (int16x2_t, int16x2_t, int32_t);
int32_t __smlsdx  (int16x2_t, int16x2_t, int32_t);
int64_t __smlsld  (int16x2_t, int16x2_t, int64_t);
int64_t __smlsldx (int16x2_t, int16x2_t, int64_t);
int32_t __smuad   (int16x2_t, int16x2_t);
int32_t __smuadx  (int16x2_t, int16x2_t);
int32_t __smusd   (int16x2_t, int16x2_t);
int32_t __smusdx  (int16x2_t, int16x2_t);
#endif /*__ARM_FEATURE_SIMD32*/

// 9.6 Floating-point data-processing intrinsics

#if (__ARM_FP & 0x08) != 0
double __sqrt (double x);
#endif /*__ARM_FP*/
#if (__ARM_FP & 0x04) != 0
float __sqrtf (float x);
#endif /*__ARM_FP*/

#ifdef __ARM_FEATURE_FMA
#if (__ARM_FP & 0x08) != 0
double __fma (double x, double y, double z);
#endif /*__ARM_FP*/
#if (__ARM_FP & 0x04) != 0
float __fmaf (float x, float y, float z);
#endif /*__ARM_FP*/
#endif /*__ARM_FEATURE_FMA*/

#ifdef __cplusplus
extern "C" {
#endif
  // C++ external declarations go here

#ifdef __cplusplus
}
#endif

#endif /*__ARM_ACLE_GCC*/
