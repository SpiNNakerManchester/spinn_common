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

//! \file
//! \brief Fast circular buffer
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

//! Implementation of a circular buffer
typedef struct _circular_buffer {
    //! The size of the buffer. One less than a power of two.
    uint32_t buffer_size;
    //! The index of the next position in the buffer to read from.
    uint32_t output;
    //! The index of the next position in the buffer to write to.
    uint32_t input;
    //! \brief The number of times an insertion has failed due to the buffer
    //!     being full.
    uint32_t overflows;
    //! The buffer itself.
    uint32_t buffer[];
} _circular_buffer;

//! The public interface type is a pointer to the implementation
typedef _circular_buffer *circular_buffer;

//! \brief Get the index of the next position in the buffer from the given
//!     value.
//! \param[in] buffer: The buffer.
//! \param[in] current: The index to get the next one after.
//! \return The next index after the given one.
static inline uint32_t _circular_buffer_next(
        circular_buffer buffer, uint32_t current) {
    return (current + 1) & buffer->buffer_size;
}

//! \brief Get whether the buffer is not empty.
//! \param[in] buffer: The buffer.
//! \return Whether the buffer has anything in it.
static inline bool _circular_buffer_not_empty(circular_buffer buffer) {
    return buffer->input != buffer->output;
}

//! \brief Get whether the buffer is able to accept more values
//! \param[in] buffer: The buffer.
//! \param[in] next: The next position in the buffer
//! \return Whether the buffer has room to take another value.
static inline bool _circular_buffer_not_full(
        circular_buffer buffer, uint32_t next) {
    return next != buffer->output;
}

//! \brief Create a new FIFO circular buffer of at least the given size. For
//!     efficiency, the buffer can be bigger than requested.
//! \param[in] size: The minimum number of elements in the buffer to be created
//! \return A reference to the created buffer.
circular_buffer circular_buffer_initialize(uint32_t size);

//! \brief Add an item to an existing buffer.
//! \param[in] buffer: The buffer struct to add to
//! \param[in] item: The item to add.
//! \return Whether the item was added (it fails if the buffer is full).
static inline bool circular_buffer_add(circular_buffer buffer, uint32_t item) {
    uint32_t next = _circular_buffer_next(buffer, buffer->input);
    bool success = _circular_buffer_not_full(buffer, next);

    if (success) {
	    buffer->buffer[buffer->input] = item;
	    buffer->input = next;
    } else {
	    buffer->overflows++;
    }

    return success;
}

//! \brief Get the next item from an existing buffer.
//! \param[in] buffer: The buffer to get the next item from.
//! \param[out] item: The retrieved item.
//! \return Whether an item was retrieved.
static inline bool circular_buffer_get_next(
        circular_buffer buffer, uint32_t *item) {
    bool success = _circular_buffer_not_empty(buffer);

    if (success) {
	    *item = buffer->buffer[buffer->output];
	    buffer->output = _circular_buffer_next(buffer, buffer->output);
    }

    return success;
}

//! \brief Advance the buffer if the next item is equal to the given value.
//! \param[in] buffer: The buffer to advance
//! \param[in] item: The item to check
//! \return Whether the buffer was advanced.
static inline bool circular_buffer_advance_if_next_equals(
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

//! \brief Get the size of the buffer.
//! \param[in] buffer: The buffer to get the size of
//! \return The number of elements currently in the buffer
static inline uint32_t circular_buffer_size(circular_buffer buffer) {
    return buffer->input >= buffer->output
	    ? buffer->input - buffer->output
	    : (buffer->input + buffer->buffer_size + 1) - buffer->output;
}

//! \brief Get the number of overflows that have occurred when adding to
//!     the buffer.
//! \param[in] buffer: The buffer to check for overflows
//! \return The number of times add was called and returned False
static inline uint32_t circular_buffer_get_n_buffer_overflows(
	    circular_buffer buffer) {
    return buffer->overflows;
}

//! \brief Clear the circular buffer.
//! \param[in] buffer: The buffer to clear
static inline void circular_buffer_clear(circular_buffer buffer) {
    buffer->input = 0;
    buffer->output = 0;
}

//! \brief Print the contents of the buffer.
//! \details Do not use if the sark `IO_BUF` is being used for binary data.
//! \param[in] buffer: The buffer to print
void circular_buffer_print_buffer(circular_buffer buffer);

//---------------------------------------
// Synaptic rewiring support functions
//---------------------------------------
//! \brief Get the input index.
//! \param[in] buffer: The buffer.
//! \return The index that the next value to be put into the buffer will be
//!     placed at.
static inline uint32_t circular_buffer_input(circular_buffer buffer) {
    return buffer->input;
}

//! \brief Get the output index.
//! \param[in] buffer: The buffer.
//! \return The index that the next value to be removed from the buffer
//!     is/will be at.
static inline uint32_t circular_buffer_output(circular_buffer buffer) {
    return buffer->output;
}

//! \brief Get the buffer size.
//! \param[in] buffer: The buffer.
//! \return The real size of the buffer.
static inline uint32_t circular_buffer_real_size(circular_buffer buffer) {
    return buffer->buffer_size;
}

//! \brief Get the buffer contents at a particular index.
//! \param[in] buffer: The buffer.
//! \param[in] index: The index to use. Note that the index is not limited to
//!     the size of the buffer.
//! \return The contents of the buffer at a particular index.
static inline uint32_t circular_buffer_value_at_index(
        circular_buffer buffer, uint32_t index) {
    return buffer->buffer[index & buffer->buffer_size];
}

#endif // _CIRCULAR_BUFFER_H_
