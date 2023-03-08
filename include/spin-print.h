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
 *
 *  \brief
 *    For quick changing between host-side and SpiNNaker side C code printing
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
 *  \date
 *    29 July, 2013
 *
 *  HISTORY
 * *  DETAILS
 *    Created on       : 29 July 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 *
 *
 */

#ifndef __SPIN_PRINT_H__
#define __SPIN_PRINT_H__

//! This function is used to represent doing nothing.

static inline void skip(void)
{
    return;
}

#ifdef DEBUG_ON_HOST
#include <stdlib.h>
#include <stdio.h>

//! \brief This macro prints out a message to the log file.
//! \param[in] type The kind of fault detected.
//! \param[in] message The user-defined part of a message.
#define __log_message(type, message, ...) \
    do {							\
	fprintf(stderr, type "(%s:%4d): " message "\n",		\
		__FILENAME__, __LINE__, ##__VA_ARGS__);		\
    } while (0)

//! \brief This macro prints a debug message if level is less than or equal
//!        to the LOG_LEVEL
//! \param[in] level The level of the messsage
//! \param[in] message The user-defined part of the debug message.
#define __log(level, message, ...) \
    do {							\
	if (level <= LOG_LEVEL) {				\
	    __log_message(message, ##__VA_ARGS__);		\
	} 							\
    } while (0)

//! \brief This macro logs information.
//! \param[in] message The user-defined part of the error message.
#define log_info(message, ...) \
    __log(LOG_INFO, "[INFO]    ", message, ##__VA_ARGS__)

#define spin1_dma_transfer(tag, s, t, d, ln) \
    do { log_info("spin1_dma_transfer (%u, %u)", (s), (t)); } while (0)

#define spin1_trigger_user_event(a, b) \
    do { log_info("spin1_trigger_user_event (%d, %d)", (a), (b)); } while (0)

#define c_main \
    c_main(void); int main(void) { c_main(); return 0; } void c_main

#else /* DEBUG_ON_HOST */
#include "sark.h"
#include "spin1_api.h"

//! This function is used to mimic malloc on SpiNNaker
#undef  malloc
#define malloc spin1_malloc
//! This function is used to mimic srandom on SpiNNaker
#undef  srandom
#define srandom spin1_srand
//! This function is used to mimic random on SpiNNaker
#undef  random
#define random spin1_rand
//! This function is used to mimic fprintf on SpiNNaker
#undef  fprintf
#define fprintf io_printf
//! This value is used to mimic stdout on SpiNNaker
#undef  stdout
#define stdout IO_BUF
//! This value is used to mimic stderr on SpiNNaker
#undef  stderr
#define stderr IO_BUF
//! This function is used to mimic printf on SpiNNaker
#undef  printf
#define printf(s, ...) \
    do { io_printf(IO_BUF, s, ##__VA_ARGS__); } while (0)
//! This function is used to mimic putchar on SpiNNaker
#undef  putchar
#define putchar(c) \
    do { io_printf(IO_BUF, "%c", c); } while (0)
//! This function is used to mimic exit on SpiNNaker
#undef  exit
#define exit(n) \
    do { spin1_exit(n); } while (0)

#endif /* DEBUG_ON_HOST */
#endif /* __SPIN_PRINT_H__ */
