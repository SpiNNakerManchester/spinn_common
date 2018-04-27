/* circular_buffer.c
 *
 *  SUMMARY
 *    The essential feature of the buffer used in this implementation is that it
 *    requires no critical-section interlocking --- PROVIDED THERE ARE ONLY TWO
 *    PROCESSES: a producer/consumer pair. If this is changed, then a more
 *    intricate implementation will probably be required, involving the use
 *    of enable/disable interrupts.
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
 *  CREATION DATE
 *    10 December, 2013
 *
 *  HISTORY
 * *  DETAILS
 *    Created on       : 10 December 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 */

#include "circular_buffer.h"
#include "utils.h"

#include "spin-print.h"
//#include <debug.h>

circular_buffer circular_buffer_initialize(
	uint32_t size)
{
    uint32_t real_size = size;
    if (!is_power_of_2(real_size)) {
	real_size = next_power_of_2(size);
    }

    circular_buffer buffer = sark_alloc(1, sizeof(_circular_buffer));
    if (buffer == NULL) {
	return NULL;
    }

    buffer->buffer = sark_alloc(1, real_size * sizeof(uint32_t));
    if (buffer->buffer == NULL) {
	return NULL;
    }

    buffer->buffer_size = real_size - 1;
    buffer->input = 0;
    buffer->output = 0;
    buffer->overflows = 0;
    return buffer;
}

void circular_buffer_print_buffer(
	circular_buffer buffer)
{
    uint32_t i = buffer->output;

    io_printf(IO_BUF, "[");
    while (i != buffer->input) {
	io_printf(IO_BUF, "%u", buffer->buffer[i]);
	i = (i + 1) & buffer->buffer_size;
	if (i != buffer->input) {
	    io_printf(IO_BUF, ", ");
	}
    }
    io_printf(IO_BUF, "]\n");
}
