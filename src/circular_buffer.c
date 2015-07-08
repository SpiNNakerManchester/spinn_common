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

static uint32_t* buffer;
static uint32_t buffer_size;

static uint32_t output;
static uint32_t input;
static uint32_t overflows;

static inline uint32_t _buffer_diff() {
    register uint32_t r = (input >= output)? 0: buffer_size;
    r += input - output;
    assert(r < buffer_size);
    return r;
}

// unallocated
//
// Returns the number of buffer slots currently unallocated
static inline uint32_t _unallocated() {
    return _buffer_diff();
}

// allocated
//
// Returns the number of buffer slots currently allocated
static inline uint32_t _allocated() {
    return (buffer_size - _buffer_diff () - 1);
}

// The following two functions are used to determine whether a
// buffer can have an element extracted/inserted respectively.
static inline bool _non_empty() {
    return (_allocated() > 0);
}

static inline bool _non_full() {
    return (_unallocated() > 0);
}

static inline uint32_t _next(uint32_t current) {
    return current - 1 == 0? buffer_size - 1: current - 1;
}

bool circular_buffer_initialize(uint32_t size) {
    buffer = (uint32_t *) sark_alloc(1, size * sizeof(uint32_t));
    if (buffer == NULL) {
        log_error("Cannot allocate in circular buffer");
        return false;
    }
    buffer_size = size;
    input = size - 1;
    output = 0;
    overflows = 0;
    return true;
}

bool circular_buffer_add(uint32_t item) {
    bool success = _non_full();

    if (success) {
        buffer[input] = item;
        input = _next(input);
    } else {
        overflows++;
    }

    return success;
}

bool circular_buffer_get_next(uint32_t* item) {

    // A failure here indicates that we're extracting from an empty buffer.
    assert(_non_empty());

    output = _next(output);
    *item = buffer[output];

    return true;
}

bool circular_buffer_advance_if_next_equals(uint32_t item) {
    bool success = _non_empty();
    if (success) {
        uint32_t next = _next(output);
        success = (buffer[next] == item);

        if (success) {
            output = next;
        }
    }

    return (success);
}

// The following two functions are used to access the locally declared
// variables.
uint32_t circular_buffer_get_n_buffer_overflows() {
    return overflows;
}

void circular_buffer_print_buffer() {
    uint32_t n = _allocated();
    uint32_t a;

    io_printf(IO_BUF, "buffer: input = %3u, output = %3u elements = %3u\n",
              input, output, n);
    io_printf(IO_BUF, "------------------------------------------------\n");

    for (; n > 0; n--) {
        a = (input + n) % buffer_size;
        io_printf(IO_BUF, "    %3u: %08x\n", a, buffer[a]);
    }

    io_printf(IO_BUF, "------------------------------------------------\n");
}
