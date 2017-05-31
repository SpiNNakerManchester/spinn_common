/*! \file
 *
 *  \brief
 *    For quick changing between host-side and spinnaker side C code printing
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

#define spin1_dma_transfer(tag, s, t, d, l) \
    do { log_info("spin1_dma_transfer (%u, %u)", (s), (t)); } while (0)

#define spin1_trigger_user_event(a, b) \
    do { log_info("spin1_trigger_user_event (%d, %d)", (a), (b)); } while (0)

#define c_main \
    c_main(void); int main(void) { c_main(); return (0); } void c_main

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
