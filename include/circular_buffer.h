#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _circular_buffer* circular_buffer;

// This function initializes a circular buffer.
circular_buffer circular_buffer_initialize(uint32_t size);

bool circular_buffer_add(circular_buffer buffer, uint32_t item);

bool circular_buffer_get_next(circular_buffer buffer, uint32_t* item);

bool circular_buffer_advance_if_next_equals(
    circular_buffer buffer, uint32_t item);

uint32_t circular_buffer_get_n_buffer_overflows(circular_buffer buffer);

#endif // _CIRCULAR_BUFFER_H_
