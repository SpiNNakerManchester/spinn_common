#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

// initialize_spike_buffer
//
// This function initializes the input spike buffer.
// It configures:
//    buffer:     the buffer to hold the spikes (initialized with size spaces)
//    input:      index for next spike inserted into buffer
//    output:     index for next spike extracted from buffer
//    overflows:  a counter for the number of times the buffer overflows
//    underflows: a counter for the number of times the buffer underflows
//
// If underflows is ever non-zero, then there is a problem with this code.
bool circular_buffer_initialize(uint32_t size);

bool circular_buffer_add(uint32_t item);

bool circular_buffer_get_next(uint32_t* item);

bool circular_buffer_advance_if_next_equals(uint32_t item);

uint32_t circular_buffer_get_n_buffer_overflows();

void circular_buffer_print_buffer();

#endif // _CIRCULAR_BUFFER_H_
