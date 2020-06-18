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

/*! \file
 *
 *  \brief SpiNNaker debug header file
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
 *    A header file that can be used to incorporate and control debug information.
 *    It is switched ON by default; to switch OFF, the code is compiled with
 *
 *        -DPRODUCTION_CODE
 *
 *    or
 *
 *        -DNDEBUG
 *
 *    By default it is used for SpiNNaker ARM code; it can also be used in
 *    host-side C, by compiling with `-DDEBUG_ON_HOST`
 *
 *  \section EXAMPLES
 *
 *    To use, you must `hash-include' debug.h:
 *
 *    **Assertions:**
 *
 *        assert(0.0 < c && c < 1.0);             // assertion checking
 *
 *    **Checking:**
 *
 *        check(1==1, "1 !=1 !!!!!!");            // assertion with message
 *        xp = malloc(n); check_memory(xp);       // checks malloc is non-null
 *
 *    **Sentinels:**
 *
 *        switch(n) {
 *        ...
 *        default: sentinel("switch: out of range"); // used for control flow checks
 *        .... }
 *
 *    **SpiNNaker memory checking:**
 *
 *    If we are running on SpiNNaker hardware, we have the following
 *    additional checks available:
 *
 *        check_itcm(addr);
 *        check_dtcm(addr);
 *        check_sysram(addr);
 *        check_sdram(addr);
 *
 *    **Controlling the volume of logging information:**
 *
 *        -DNO_DEBUG_INFO            Switches OFF the [INFO] information
 *        -D'DEBUG_LOG(n)=(n>10)'    Switches OFF [ERROR]s with number less than or equal 10
 *        -D'DEBUG_WARN(n)=(n>5)'    Switches OFF [WARNING]s with number less than or equal 5
 *
 *    By default all information is printed.
 *
 *    There is no way to switch off [ASSERT]s except by using either of the
 *    compilation flags: `-DPRODUCTION_CODE` or `-DNDEBUG`
 */

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "spin-print.h"

//! \brief This macro is intended to mimic the behaviour of 'C's exit
//! 	system call.
//! \param[in] n: Value to pass to exit()
#define abort(n)	do { exit(n); } while (0)

//! \brief The log level for errors
#define LOG_ERROR 10

//! \brief The log level for warnings
#define LOG_WARNING 20

//! \brief The log level for information
#define LOG_INFO 30

//! \brief The log level for debugging
#define LOG_DEBUG 40

// Define the log level if not already defined
#ifndef LOG_LEVEL
#if defined(PRODUCTION_CODE) || defined(NDEBUG)
#define LOG_LEVEL LOG_INFO
#else // PRODUCTION_CODE
//! The logging level
#define LOG_LEVEL LOG_DEBUG
#endif // PRODUCTION_CODE
#endif // LOG_LEVEL

#if !(defined(PRODUCTION_CODE) || defined(NDEBUG))

//! \brief This macro prints out a check message to the log file.
//! \param[in] condition The condition being tested.
//! \param[in] message The message to be printed if the condition is false
#define check(condition, message, ...) \
    do {								\
        if (!(condition)) {						\
            io_printf(IO_BUF, "[CHECK]    ", message, ##__VA_ARGS__);	\
        } 								\
    } while (0)

//! \brief This macro prints out a sentinel message to the log file and aborts
//! \param[in] message The message to be printed if execution reaches this point
#define sentinel(message, ...) \
    do {								\
        io_printf(IO_BUF, "[SENTINEL] ", message, ##__VA_ARGS__);	\
        abort(0);							\
    } while (0)

//! \brief This macro performs an assertion check on a condition and aborts if
//!        the condition is not met
//! \param[in] assertion The condition being tested.
#define assert(assertion) \
    do {								\
        if (!(assertion)) {						\
            io_printf(IO_BUF, "[ASSERT]   ", "assertion check fails!");	\
            abort(0);							\
        }								\
    } while (0)

//! \brief This macro performs an assertion check on a condition and aborts if
//!        the condition is not met
//! \param[in] assertion The condition being tested.
//! \param[in] message The message to be printed if the condition is false
#define assert_info(assertion, message, ...) \
    do {                                                                \
        if (!(assertion)) {                                             \
            io_printf(IO_BUF, "[ASSERT]   ", message, ##__VA_ARGS__);    \
            abort(0);                                                   \
        }                                                               \
    } while (0)

#else  /* PRODUCTION_CODE */
#define check(a, s, ...)	skip()
#define sentinel(s, ...)	skip()
#define assert(a)		skip()
#define assert_info(a, m, ...)	skip()
#endif /* PRODUCTION_CODE */

//! \brief This function returns the unsigned integer associated with a
//!     pointer address.
//! \param[in] ptr: The pointer whose address is required.
//! \return The value as an unsigned integer.
static inline unsigned int __addr__(void *ptr)
{
    return (unsigned int) ptr;
}

//! \brief This macro tests whether a pointer returned by malloc is null.
//! \param[in] a The address returned by malloc.
#define check_memory(a)		check((a), "Out of memory")

#ifndef DEBUG_ON_HOST
//! \brief This macro tests whether a pointer's address lies in itcm.
//! \param[in] a: The pointer.
#define check_itcm(a) \
    check((ITCM_BASE   <= __addr__(a) && __addr__(a) < ITCM_TOP),       \
          "%x is not in ITCM", (a))

//! \brief This macro tests whether a pointer's address lies in dtcm.
//! \param[in] a: The pointer.
#define check_dtcm(a) \
    check((DTCM_BASE   <= __addr__(a) && __addr__(a) < DTCM_TOP),       \
          "%x is not in DTCM", (a))

//! \brief This macro tests whether a pointer's address lies in sysram.
//! \param[in] a: The pointer.
#define check_sysram(a) \
    check((SYSRAM_BASE <= __addr__(a) && __addr__(a) < SYSRAM_TOP),     \
          "%x is not in sysRAM", (a))

//! \brief This macro tests whether a pointer's address lies in sdram.
//! \param[in] a: The pointer.
#define check_sdram(a) \
    check((SDRAM_BASE  <= __addr__(a) && __addr__(a) < SDRAM_TOP),      \
          "%x is not in sdram", (a))

#else  /* DEBUG_ON_HOST */
#define check_itcm(a)		skip()
#define check_dtcm(a)		skip()
#define check_sysram(a)		skip()
#define check_sdram(a)		skip()
#endif /* DEBUG_ON_HOST */

#endif /* __ASSERT_H__ */
