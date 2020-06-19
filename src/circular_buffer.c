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

/*! \file circular_buffer.c
 * \brief Support functions for fast circular buffer implementation
 * \details
 *    The essential feature of the buffer used in this implementation is that it
 *    requires no critical-section interlocking --- PROVIDED THERE ARE ONLY TWO
 *    PROCESSES: a producer/consumer pair. If this is changed, then a more
 *    intricate implementation will probably be required, involving the use
 *    of enable/disable interrupts.
 *
 * \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 * \copyright
 *    &copy; Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date
 *    10 December, 2013
 */

#include "circular_buffer.h"
#include "utils.h"

#include "spin-print.h"

circular_buffer circular_buffer_initialize(
	uint32_t size)
{
    uint32_t real_size = size;
    if (!is_power_of_2(real_size)) {
	real_size = next_power_of_2(size);
    }

    circular_buffer buffer = sark_alloc(1,
	    sizeof(_circular_buffer) + real_size * sizeof(uint32_t));
    if (buffer == NULL) {
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
