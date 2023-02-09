/*
 * Copyright (c) 2013-2023 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
