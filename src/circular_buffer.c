/*
 * circular_buffer.c
 *
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
 *
 *    $Log$
 *
 */

#include "circular_buffer.h"

#include <debug.h>

typedef struct _circular_buffer {

    uint32_t* buffer;
    uint32_t buffer_size;

    uint32_t output;
    uint32_t input;
    uint32_t overflows;
} _circular_buffer;

// allocated
//
// Returns the number of buffer slots currently allocated
static inline uint32_t _circular_buffer_allocated(circular_buffer buffer) {
    register uint32_t r = (buffer->input >= buffer->output)? 0:
            buffer->buffer_size;
    r += buffer->input - buffer->output;
    assert(r < buffer->buffer_size);
    return r;
}

// unallocated
//
// Returns the number of buffer slots currently unallocated
static inline uint32_t _circular_buffer_unallocated(circular_buffer buffer) {
    return buffer->buffer_size - _circular_buffer_allocated(buffer);
}

// The following two functions are used to determine whether a
// buffer can have an element extracted/inserted respectively.
static inline bool _circular_buffer_non_empty(circular_buffer buffer) {
    return (_circular_buffer_allocated(buffer) > 0);
}

static inline bool _circular_buffer_non_full(circular_buffer buffer) {
    return (_circular_buffer_unallocated(buffer) > 0);
}

static inline uint32_t _circular_buffer_next(circular_buffer buffer,
                                             uint32_t current) {
    return current + 1 == buffer->buffer_size? 0: current + 1;
}

circular_buffer circular_buffer_initialize(uint32_t size) {
    circular_buffer buffer = sark_alloc(1, sizeof(struct _circular_buffer));
    if (buffer == NULL) {
        log_error("Cannot allocate space for buffer structure");
        return NULL;
    }
    buffer->buffer = (uint32_t *) sark_alloc(1, size * sizeof(uint32_t));
    if (buffer->buffer == NULL) {
        log_error("Cannot allocate space for buffer in circular buffer");
        return NULL;
    }
    buffer->buffer_size = size;
    buffer->input = 0;
    buffer->output = 0;
    buffer->overflows = 0;
    return buffer;
}

bool circular_buffer_add(circular_buffer buffer, uint32_t item) {
    bool success = _circular_buffer_non_full(buffer);

    if (success) {
        buffer->buffer[buffer->input] = item;
        buffer->input = _circular_buffer_next(buffer, buffer->input);
    } else {
        buffer->overflows++;
    }

    return success;
}

bool circular_buffer_get_next(circular_buffer buffer, uint32_t* item) {
    bool success = _circular_buffer_non_empty(buffer);

    if (success) {
        *item = buffer->buffer[buffer->output];
        buffer->output = _circular_buffer_next(buffer, buffer->output);
    }

    return success;
}

bool circular_buffer_advance_if_next_equals(circular_buffer buffer,
                                            uint32_t item) {
    bool success = _circular_buffer_non_empty(buffer);
    if (success) {
        uint32_t next = _circular_buffer_next(buffer, buffer->output);
        success = (buffer->buffer[next] == item);

        if (success) {
            buffer->output = next;
        }
    }

    return success;
}

// The following two functions are used to access the locally declared
// variables.
uint32_t circular_buffer_get_n_buffer_overflows(circular_buffer buffer) {
    return buffer->overflows;
}

void circular_buffer_print_buffer(circular_buffer buffer) {
    uint32_t n = _circular_buffer_allocated(buffer);
    uint32_t a;

    io_printf(IO_BUF, "buffer: input = %3u, output = %3u elements = %3u\n",
              buffer->input, buffer->output, n);
    io_printf(IO_BUF, "------------------------------------------------\n");

    for (; n > 0; n--) {
        a = (buffer->input + n) % buffer->buffer_size;
        io_printf(IO_BUF, "    %3u: %08x\n", a, buffer->buffer[a]);
    }

    io_printf(IO_BUF, "------------------------------------------------\n");
}
