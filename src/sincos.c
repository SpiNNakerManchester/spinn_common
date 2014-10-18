/*! \file
 *
 *  \brief Implementation of log for the accum type.
 *
 *  \details The details of the algorithm are from
 *     "Elementary Functions: Algorithms and Implemenation", 2nd edn,
 *     Jean-Michel Muller, Birkhauser, 2006.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
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
 *  \date 30 October, 2014
 *
 */

#include "stdfix-full-iso.h"
#include "debug.h"
#include "arm_acle.h"
#include "pair.h"

pair_t dp (int x, int y) { return (pair (x, y)); }
int df (pair_t p) { return ((int)(fst (p))); }
int ds (pair_t p) { return ((int)(snd (p))); }


//! \brief This function calculates the sin function approximation.
//! \param[in] x is an s0.31 representing a number in the interval
//! [-1/32, 1/32); i.e. INT32_MIN represents -1/32
//! \return A value representing sin (x) in s0.31 format.

static inline int32_t sin_approx (int32_t x)
{   return (x >> 5); }

//! \brief This function calculates the cos function approximation.
//! \param[in] x is an s0.31 representing a number in the interval
//! [-1/32, 1/32); i.e. INT32_MIN represents -1/32
//! \return A value representing (1 - cos (x)) in s0.31 format.

static inline int32_t cos_approx (int32_t x)
{   return (__smultt (x, x) >> 10); }

//! \brief sin_table[k] = sin (k/16) * 2^31

static int32_t sin_table [16]
  = {         0,  134130364,  267736951,  400298032,
      531295957,  660219183,  786564267,  909837834,
     1029558505, 1145258771, 1256486826, 1362808327,
     1463808091, 1559091715, 1648287120, 1731045999};

//! \brief cos_table[k] = (cos (k/16) - 1) * 2^31

static int32_t cos_table [16]
  = {          0,   -4192939,  -16755382,  -37638274,
       -66760066, -104007040, -149233746, -202263575,
      -262889447, -330874618, -405953610, -487833239,
      -576193767, -670690148, -770953377, -876591926};

//! This function calculates the sin function for the interval [0, pi/4)
//! param [in] x A value in the range [-1/32,1/32)
//! param [in] cos_x The value of cos_x (= (1 - cos (x)) * 2^31)
//! param [in] break_point A value in the range 0..13
//! return sin (x + break_point / 16) as s0.31

static inline int32_t sin_pi4 (int32_t x, int32_t cos_x, int break_point)
{
    register int32_t r;

    assert (0 <= break_point && break_point <= 13);

    r = sin_table [break_point];
    r = __smlawt (sin_table [break_point], -(cos_x << 1), r);
    r = __smlawt (cos_table [break_point], x >> 4, r + (x >> 5));

    return (r);
}

//! This function calculates the cos function for the interval [0, pi/4)
//! param [in] x A value in the range [-1/32,1/32)
//! param [in] cos_x The value of cos_x (= (1 - cos (x)) * 2^31)
//! param [in] break_point A value in the range 0..13
//! return 1 - cos (x + break_point / 16) as s0.31

// We are using the identity: cos(x+b) = cos(x)*cos(b) - sin(x)*sin(b)
//
// However, we return 1-cos(x+b)
//         = 1 - cos(x)*cos(b) + sin(x)*sin(b)
//
// Let cx = 1-cos(x), and cb = 1-cos(b)
//
// Then cx * cb = 1 + cos(x)*cos(b) - cos(x) - cos(b)
//
// Thus 1 - cos(x)*cos(b)
//    = -(cx*cb) + cx + cb

static inline int32_t cos_pi4 (int32_t x, int32_t cos_x, int break_point)
{
    register int32_t r;

    assert (0 <= break_point && break_point <= 13);

    r = cos_x - cos_table [break_point];
    r = __smlawt (cos_table [break_point], cos_x << 1, r);
    r = __smlawt (sin_table [break_point], (x >> 4), r);

    return (r);
}



/*uint64_t sincos_range_reduction (int32_t x)
{
    register union {uint64_t r; int32_t[2] v;} t;

    t.v[0] = x;
    t.v[1] = 0;

    return (t.r);
}
*/


 // check list:
 //                cos_x _is_ correctly calculated.
 //                x is correctly scaled.
 //                break point is correctly calculated.

static inline int32_t sinkbits (int32_t x)
{
    register int break_point = __stdfix_round_s32 (x, 11);
    register int32_t r, cos_x;

    x -= break_point;
    x  = x << 21;
    break_point
       = break_point >> 11;
    cos_x
       = cos_approx (x);

    r  = sin_pi4 (x, cos_x, break_point);
    r  = __stdfix_round_s32 (r, 16) >> 16;

    return (r);
}

static inline int32_t coskbits (int32_t x)
{
    register int break_point = __stdfix_round_s32 (x, 11);
    register int32_t r, cos_x;

    x -= break_point;
    x  = x << 21;
    break_point
       = break_point >> 11;

    cos_x
       = cos_approx (x);

    r  = cos_pi4 (x, cos_x, break_point);
    r  = __stdfix_round_s32 (r, 16) >> 16;

    return (32768 - r);
}

//! \brief This function calculates the sin function for the
//! accum type (s16.15).
//! \param[in] x is positive value representated as an accum.
//! \return A value representing sin (x) in accum format.

accum sink (accum x)
{   return (kbits (sinkbits (bitsk (x)))); }

//! \brief This function calculates the sin function for the
//! accum type (s16.15).
//! \param[in] x is positive value representated as an accum.
//! \return A value representing sin (x) in accum format.

accum cosk (accum x)
{   return (kbits (coskbits (bitsk (x)))); }
