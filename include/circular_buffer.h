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

#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _circular_buffer {
    uint32_t buffer_size;
    uint32_t output;
    uint32_t input;
    uint32_t overflows;
    uint32_t buffer[];
} _circular_buffer, *circular_buffer;

// Returns the index of the next position in the buffer from the given value
static inline uint32_t _circular_buffer_next(
        circular_buffer buffer,
	uint32_t current)
{
    return (current + 1) & buffer->buffer_size;
}

// Returns true if the buffer is not empty
static inline bool _circular_buffer_not_empty(
	circular_buffer buffer)
{
    return buffer->input != buffer->output;
}

// Returns true if the buffer is not full
static inline bool _circular_buffer_not_full(
	circular_buffer buffer)
{
    return _circular_buffer_next(buffer, buffer->input) != buffer->output;
}

//! \brief Creates a new FIFO circular buffer of at least the given size.  For
//!        efficiency, the buffer can be bigger than requested
//! \param[in] size The minimum number of elements in the buffer to be created
//! \return A struct representing the created buffer
circular_buffer circular_buffer_initialize(uint32_t size);

//! \brief Adds an item to an existing buffer
//! \param[in] buffer The buffer struct to add to
//! \param[in] item The item to add
//! \return True if the item was added, False if the buffer was full
static inline bool circular_buffer_add(
	circular_buffer buffer,
	uint32_t item)
{
    bool success = _circular_buffer_not_full(buffer);

    if (success) {
	buffer->buffer[buffer->input] = item;
	buffer->input = _circular_buffer_next(buffer, buffer->input);
    } else {
	buffer->overflows++;
    }

    return success;
}

//! \brief Get the next item from an existing buffer
//! \param[in] buffer The buffer to get the next item from
//! \param[out] item  A pointer to receive the next item
//! \return True if an item was retrieved, False if the buffer was empty
static inline bool circular_buffer_get_next(
	circular_buffer buffer,
	uint32_t *item)
{
    bool success = _circular_buffer_not_empty(buffer);

    if (success) {
	*item = buffer->buffer[buffer->output];
	buffer->output = _circular_buffer_next(buffer, buffer->output);
    }

    return success;
}

//! \brief Advances the buffer if the next item is equal to the given value
//! \param[in] buffer The buffer to advance
//! \param[in] The item to check
//! \return True if the buffer was advanced, False otherwise
static inline bool circular_buffer_advance_if_next_equals(
        circular_buffer buffer,
	uint32_t item)
{
    bool success = _circular_buffer_not_empty(buffer);
    if (success) {
	success = (buffer->buffer[buffer->output] == item);
	if (success) {
	    buffer->output = _circular_buffer_next(buffer, buffer->output);
	}
    }
    return success;
}

//! \brief Gets the size of the buffer
//! \param[in] buffer The buffer to get the size of
//! \return The number of elements currently in the buffer
static inline uint32_t circular_buffer_size(
	circular_buffer buffer)
{
    return buffer->input >= buffer->output
	    ? buffer->input - buffer->output
	    : (buffer->input + buffer->buffer_size + 1) - buffer->output;
}

//! \brief Gets the number of overflows that have occurred when adding to
//!        the buffer
//! \param[in] buffer The buffer to check for overflows
//! \return The number of times add was called and returned False
static inline uint32_t circular_buffer_get_n_buffer_overflows(
	circular_buffer buffer)
{
    return buffer->overflows;
}

//! \brief clears the circular buffer
//! \param[in] buffer The buffer to clear
static inline void circular_buffer_clear(
	circular_buffer buffer)
{
    buffer->input = 0;
    buffer->output = 0;
}

//! \brief Prints the contents of the buffer.
//! Do not use if the sark IO_BUF is being used for binary data.
//! \param[in] The buffer to print
void circular_buffer_print_buffer(circular_buffer buffer);

//---------------------------------------
// Synaptic rewiring support functions
//---------------------------------------
static inline uint32_t circular_buffer_input(
	circular_buffer buffer)
{
    return buffer->input;
}

static inline uint32_t circular_buffer_output(
	circular_buffer buffer)
{
    return buffer->output;
}

static inline uint32_t circular_buffer_real_size(
	circular_buffer buffer)
{
    return buffer->buffer_size;
}

static inline uint32_t circular_buffer_value_at_index(
	circular_buffer buffer,
	uint32_t index)
{
    return buffer->buffer[index & buffer->buffer_size];
}

#endif // _CIRCULAR_BUFFER_H_
