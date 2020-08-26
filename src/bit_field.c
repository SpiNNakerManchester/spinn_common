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

/*! \file
 *
 *  \brief Bit field manipulation.
 *
 *  \details A bit field is a vector of machine words which is
 *    treated as a vector of bits.
 *
 *    For SpiNNaker each machine word is 32 bits, and so a
 *    bit_field for each neuron (assuming 256 neurons)
 *    would be 8 words long.
 *
 *    The API includes:
 *
 *     - bit_field_test()
 *         returns true of false depending on whether bit n is set or clear
 *     - bit_field_set() / bit_field_clear()
 *         used to set or clear bit n
 *     - not_bit_field()
 *         logically inverts a bit field of size s.
 *     - and_bit_fields() / or_bit_fields()
 *         logically ands/ors two bit_fields together. Requires size.
 *     - clear_bit_field() / set_bit_field()
 *         Initializes bit_field with all false (= clear) or true (= set).
 *         Requires size.
 *
 *    There are also support functions for:
 *
 *     - printing
 *     - randomly setting up a bit field
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk),
 *    Jamie Knight (knightj@cs.man.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester, Jamie Knight and The University of Manchester,
 *    2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 */

#include "bit_field.h"
#include "sark.h"

//! \brief The use macro allows us to "pretend" to use a variable in a
//!     function; this is useful if we run with -Wextra set for extra
//!     warnings.
//! \param x: The name of the argument to mark as used

#ifndef __use
#define __use(x)      ((void) (x))
#endif

//! \brief This function prints out an individual word of a bit_field,
//!     as a sequence of ones and zeros.
//! \param[in] e The word of a bit_field to be printed.
static inline void print_bit_field_entry(
        uint32_t e)
{
    for (counter_t i = 32 ; i > 0; i--) {
        io_printf(IO_BUF, "%c", ((e & 0x1) == 0) ? ' ' : '1');
        e >>= 1;
    }

    io_printf(IO_BUF, "\n");
}

void print_bit_field_bits(
        const bit_field_t restrict b,
        size_t s)
{
    __use(b);
    __use(s);
#if LOG_LEVEL >= LOG_DEBUG
    for (index_t i = 0; i < s; i++) {
        print_bit_field_entry(b[i]);
    }
#endif // LOG_LEVEL >= LOG_DEBUG
}

void print_bit_field(
        const bit_field_t restrict b,
        size_t s)
{
    __use(b);
    __use(s);
#if LOG_LEVEL >= LOG_DEBUG
    for (index_t i = 0; i < s; i++) {
        io_printf(IO_BUF, "%08x\n", b[i]);
    }
#endif // LOG_LEVEL >= LOG_DEBUG
}

void random_bit_field(
        bit_field_t restrict b,
        size_t s)
{
    __use(b);
    __use(s);
#if LOG_LEVEL >= LOG_DEBUG
    for (index_t i = 0; i < s; i++) {
        b[i] = sark_rand();
    }
#endif // LOG_LEVEL >= LOG_DEBUG
}

bit_field_t bit_field_alloc(uint32_t n_atoms) {
    uint32_t n_words = get_bit_field_size(n_atoms);
    return (bit_field_t) sark_alloc(n_words * sizeof(bit_field_t), 1);
}
