/*! \file
 *
 *  \brief Bit field manipulation.
 *
 *  \details A bit field is a vector of machine words which is
 *    treated as a vector of bits.
 *
 *    For SpiNNAker each machine word is 32 bits, and so a
 *    bit_field for each neuron (assuming 256 neurons)
 *    would be 8 words long.
 *
 *    The API includes:
 *
 *     - bit_field_test (b, n)
 *         returns true of false depending on whether bit n is set or clear
 *     - bit_field_set (b, n) / bit_field_clear (b, n)
 *         used to set or clear bit n
 *     - not_bit_field (b, s)
 *         logically inverts a bit field of size s.
 *     - and_bit_field / or_bit_field
 *         logically ands/ors two bit_fields together. Requires size.
 *     - clear_bit_field/set_bit_field
 *         Initializes bit_field with all false (= clear) or true (= set).
 *         Requires size.
 *
 *    There are also support functions for:
 *     
 *     - printing
 *     - randomly setting up a bit field
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 *
 *  DETAILS
 *    Created on       : 12 December 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#ifndef __BIT_FIELD_H__
#define __BIT_FIELD_H__

#include <stdint.h>
#include <stdbool.h>

//! \brief The use macro allows us to "pretend" to use a variable in a
//! function; this is useful if we run with -Wextra set for extra warnings.

#ifndef use
#define use(x) do {} while ((x)!=(x))
#endif

//! \brief bit_field_t is an arbitrary length bit field (vector of bits)
//! which is used to compactly represent a large number of boolean operations.
typedef uint32_t* bit_field_t;

#ifndef __SIZE_T__
//! \brief An unsigned integer used for the size of objects.
typedef uint32_t size_t;
//! \brief __SIZE_T__ is defined whenever the size_t type is already defined.
#define __SIZE_T__
#endif /*__SIZE_T__*/

#ifndef __INDEX_T__
//! \brief An unsigned integer used as an index.
typedef uint32_t index_t;
//! \brief __INDEX_T__ is defined whenever the index_t type is already defined.
#define __INDEX_T__
#endif /*__INDEX_T__*/

#ifndef __COUNTER_T__
//! \brief An unsigned integer used as a counter or iterator.
typedef uint32_t counter_t;
//! \brief __COUNTER_T__ is defined whenever the counter_t type is already defined.
#define __COUNTER_T__
#endif /*__COUNTER_T__*/

//! \brief This function tests a particular bit of a bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] n The size of the bit_field.
//! \return The function returns true if the bit is set or false otherwise.

static inline bool bit_field_test (bit_field_t b, index_t n)
{ return ((b [n >> 5] & (1 << (n & 0x1F))) != 0); }

//! \brief This function clears a particular bit of a bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] n The size of the bit_field.

static inline void bit_field_clear (bit_field_t b, index_t n)
{ b [n >> 5] &= ~(1 << (n & 0x1F)); }

//! \brief This function sets a particular bit of a bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] n The bit in the bit_field of interest.

static inline void bit_field_set  (bit_field_t b, index_t n)
{ b [n >> 5] |= (1 << (n & 0x1F)); }

//! \brief This function negates the bits of an entire bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

static inline void not_bit_field (bit_field_t b, size_t s)
{ for ( ; s > 0; s--) b [s-1] = ~ (b [s-1]); }

//! \brief This function ands two bit_fields together.
//! \param[in,out] b1 The sequence of words representing the first bit_field;
//! the result is returned in this parameter.
//! \param[in] b2 The sequence of words representing the second bit_field.
//! \param[in] s The size of the bit_field.

static inline void and_bit_fields (bit_field_t b1, bit_field_t b2, size_t s)
{ for ( ; s > 0; s--) b1 [s-1] &= b2 [s-1]; }

//! \brief This function ors two bit_fields together.
//! \param[in,out] b1 The sequence of words representing the first bit_field;
//! the result is returned in this parameter.
//! \param[in] b2 The sequence of words representing the second bit_field.
//! \param[in] s The size of the bit_field.

static inline void or_bit_fields  (bit_field_t b1, bit_field_t b2, size_t s)
{ for ( ; s > 0; s--) b1 [s-1] |= b2 [s-1]; }

//! \brief This function clears an entire bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

static inline void clear_bit_field (bit_field_t b, size_t s)
{ for ( ; s > 0; s--) b [s-1] = 0; }

//! \brief This function sets an entire bit_field.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

static inline void set_bit_field  (bit_field_t b, size_t s)
{ for ( ; s > 0; s--) b [s-1] = 0xFFFFFFFF; }

//! \brief This function tests whether a bit_field is all zeros.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.
//! \return The function returns true if every bit is zero, or false otherwise.

static inline bool empty_bit_field (bit_field_t b, size_t s)
{
    bool empty = true;

    for ( ; s > 0; s--)
        empty = empty && (b [s-1] == 0);

    return (empty);
}

//! \brief Testing whether a bit_field is non-empty, _i.e._ if there is at
//! least one bit set.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.
//! \return The function returns true if at least one bit is set; otherwise false.

static inline bool nonempty_bit_field  (bit_field_t b, size_t s)
{ return (!empty_bit_field (b, s)); }

//! \brief A function that calculates the size of a bit_field to hold 'bits'
//! bits.
//! \param[in] bits The number of bits required for this bit_field.
//! \return The size (or number of words) in the bit_field.

static inline size_t get_bit_field_size (size_t bits)
{
    // **NOTE** in floating point terms this is ceil(num_neurons / 32)
    const uint32_t bits_to_words_shift = 5;
    const uint32_t bits_to_words_remainder = (1 << bits_to_words_shift) - 1;
  
    // Down shift number of bits to words
    uint32_t words = bits >> bits_to_words_shift;
  
    // If there was a remainder, add an extra word
    if ((bits & bits_to_words_remainder) != 0)
        words++;

    return (words);
}

#ifdef DEBUG
//! \brief Prints a bit_field as ones and zeros.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

void print_bit_field_bits  (bit_field_t b, size_t s);

//! \brief Prints a bit_field as a sequence of hexadecimal characters.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

void print_bit_field       (bit_field_t b, size_t s);

//! \brief Generates a random bit_field for testing purposes.
//! \param[in] b The sequence of words representing a bit_field.
//! \param[in] s The size of the bit_field.

void random_bit_field      (bit_field_t b, size_t s);

#endif /*DEBUG*/
#endif /*__BIT_FIELD_H__*/
