#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct _circular_buffer* circular_buffer;

//! \brief Creates a new FIFO circular buffer of at least the given size.  For
//!        efficiency, the buffer can be bigger than requested
//! \param[in] size The minimum number of elements in the buffer to be created
//! \return A struct representing the created buffer
circular_buffer circular_buffer_initialize(uint32_t size);

//! \brief Adds an item to an existing buffer
//! \param[in] buffer The buffer struct to add to
//! \param[in] item The item to add
//! \return True if the item was added, False if the buffer was full
bool circular_buffer_add(circular_buffer buffer, uint32_t item);

//! \brief Get the next item from an existing buffer
//! \param[in] buffer The buffer to get the next item from
//! \param[out] item  A pointer to receive the next item
//! \return True if an item was retrieved, False if the buffer was empty
bool circular_buffer_get_next(circular_buffer buffer, uint32_t* item);

//! \brief Advances the buffer if the next item is equal to the given value
//! \param[in] buffer The buffer to advance
//! \param[in] The item to check
//! \return True if the buffer was advanced, False otherwise
bool circular_buffer_advance_if_next_equals(
    circular_buffer buffer, uint32_t item);

//! \brief Gets the size of the buffer
//! \param[in] buffer The buffer to get the size of
//! \return The number of elements currently in the buffer
uint32_t circular_buffer_size(circular_buffer buffer);

//! \brief Gets the number of overflows that have occurred when adding to
//!        the buffer
//! \param[in] buffer The buffer to check for overflows
//! \return The number of times add was called and returned False
uint32_t circular_buffer_get_n_buffer_overflows(circular_buffer buffer);

//! \brief Prints the contents of the buffer
//! \param[in] The buffer to print
void circular_buffer_print_buffer(circular_buffer buffer);

#endif // _CIRCULAR_BUFFER_H_
