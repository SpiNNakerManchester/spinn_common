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

// Returns the next highest power of 2 of a value
static inline uint32_t _next_power_of_two(uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

// Returns True if the value is a power of 2
static inline bool _is_power_of_2(uint32_t v) {
    return (v & (v - 1)) == 0;
}

// Returns the index of the next position in the buffer from the given value
static inline uint32_t _circular_buffer_next(
        circular_buffer buffer, uint32_t current) {
    return (current + 1) & buffer->buffer_size;
}

// Returns true if the buffer is not empty
static inline bool _circular_buffer_not_empty(circular_buffer buffer) {
    return buffer->input != buffer->output;
}

// Returns true if the buffer is not full
static inline bool _circular_buffer_not_full(circular_buffer buffer) {
    return ((buffer->input + 1) & buffer->buffer_size) != buffer->output;
}


circular_buffer circular_buffer_initialize(uint32_t size) {
    uint32_t real_size = size;
    if (!_is_power_of_2(real_size)) {
        real_size = _next_power_of_two(size);
        log_warning(
            "Requested size of %u was rounded up to %u", size, real_size - 1);
    }
    circular_buffer buffer = sark_alloc(1, sizeof(struct _circular_buffer));
    if (buffer == NULL) {
        log_error("Cannot allocate space for buffer structure");
        return NULL;
    }
    buffer->buffer = (uint32_t *) sark_alloc(1, real_size * sizeof(uint32_t));
    if (buffer->buffer == NULL) {
        log_error("Cannot allocate space for buffer in circular buffer");
        return NULL;
    }
    buffer->buffer_size = real_size - 1;
    buffer->input = 0;
    buffer->output = 0;
    buffer->overflows = 0;
    return buffer;
}

bool circular_buffer_add(circular_buffer buffer, uint32_t item) {
    bool success = _circular_buffer_not_full(buffer);

    if (success) {
        buffer->buffer[buffer->input] = item;
        buffer->input = _circular_buffer_next(buffer, buffer->input);
    } else {
        buffer->overflows++;
    }

    return success;
}

bool circular_buffer_get_next(circular_buffer buffer, uint32_t* item) {
    bool success = _circular_buffer_not_empty(buffer);

    if (success) {
        *item = buffer->buffer[buffer->output];
        buffer->output = _circular_buffer_next(buffer, buffer->output);
    }

    return success;
}

bool circular_buffer_advance_if_next_equals(
        circular_buffer buffer, uint32_t item) {
    bool success = _circular_buffer_not_empty(buffer);
    if (success) {
        success = (buffer->buffer[buffer->output] == item);
        if (success) {
            buffer->output = _circular_buffer_next(buffer, buffer->output);
        }
    }
    return success;
}

uint32_t circular_buffer_size(circular_buffer buffer) {
    return buffer->input >= buffer->output?
        buffer->input - buffer->output:
        (buffer->input + buffer->buffer_size + 1) - buffer->output;
}

uint32_t circular_buffer_get_n_buffer_overflows(circular_buffer buffer) {
    return buffer->overflows;
}

void circular_buffer_print_buffer(circular_buffer buffer) {
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
