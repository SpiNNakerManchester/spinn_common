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
 *  \date 30 September, 2014
 *
 */

#include "stdfix-full-iso.h"
#include "debug.h"
#include "arm_acle.h"

// ck = 1 + k/64
//
// Thus log_ck[k] = log(1 + k/64) + 12 in u5.27

static uint32_t log_ck[] = {
      1610612736, 1612693673, 1614742838, 1616761188,
      1618749635, 1620709053, 1622640276, 1624544106,
      1626421307, 1628272616, 1630098736, 1631900343,
      1633678087, 1635432592, 1637164458, 1638874261,
      1640562556, 1642229879, 1643876743, 1645503644,
      1647111061, 1648699455, 1650269271, 1651820939,
      1653354872, 1654871473, 1656371128, 1657854212,
      1659321087, 1660772103, 1662207601, 1663627908,
      1665033342, 1666424211, 1667800815, 1669163444,
      1670512377, 1671847888, 1673170241, 1674479692,
      1675776492, 1677060882, 1678333097, 1679593367,
      1680841913, 1682078952, 1683304693, 1684519341,
      1685723096, 1686916150, 1688098693, 1689270907,
      1690432973, 1691585063, 1692727349, 1693859995,
      1694983162, 1696097009, 1697201687, 1698297348,
      1699384138, 1700462197, 1701531667, 1702592682,
      1703645376
};

static int16_t recip_table[] = {
	  0,  -1008,  -1986,  -2934,  -3855,  -4749,  -5617,  -6461,
      -7282,  -8080,  -8856,  -9612, -10348, -11065, -11763, -12444,
     -13107, -13754, -14386, -15002, -15604, -16191, -16765, -17326,
     -17873, -18409, -18933, -19445, -19946, -20436, -20916, -21385,
     -21845, -22296, -22737, -23169, -23593, -24008, -24415, -24815,
     -25206, -25590, -25967, -26337, -26700, -27056, -27406, -27749,
     -28087, -28418, -28744, -29064, -29378, -29687, -29991, -30290,
     -30583, -30872, -31156, -31436, -31711, -31982, -32248, -32510,
     -32768
};

//! \brief This function divides x by k/64.
//! \param[in] x input value
//! \param[in] k the `breakpoint' index.
//! \return x/(k/64)
static inline int32_t divide_ck(
	int32_t x,
	uint32_t k)
{
    assert(-33554432 <= x && x <= 33554432);
    assert(k <= 64);

#ifdef __ARM_FEATURE_DSP
    return __smlawb(x, recip_table[k], x);
#else /*__ARM_FEATURE_DSP*/
    register int64_t r = (int64_t) x;

    r = 2*r + ((r * recip_table[k]) >> 16);
    return (int32_t) r;
#endif /*__ARM_FEATURE_DSP*/
}

static inline uint32_t uint32_round(
	uint32_t r,
	uint32_t n)
{
    return (r >> n) + ((r >> (n-1)) & 0x1);
}

//! \brief This function performs a multiply-accumulate
//! \param[in] r accumulator value
//! \param[in] x one of the factors
//! \return r := r - x * log(2).
static inline int32_t subtract_mult_log2(
	int32_t r,
	int32_t x)
{
#ifdef __ARM_FEATURE_DSP
    return r - x * 93032640;
    // return __smlawb(x << 10, -1453635, r);	// DKF why is this commented out?
#else  /*__ARM_FEATURE_DSP*/
    return r - x * 93032640;
#endif /*__ARM_FEATURE_DSP*/
}

static inline int32_t cubic_term(
	int32_t r)
{
#define CUBIC_TERM_CONSTANT 178958404

#ifdef __ARM_FEATURE_DSP
    register int32_t t = __smultt(r, r);

    t = __smulwt(CUBIC_TERM_CONSTANT * 8, t);
    t = __smulwt(t, r);
    return r + t;
#else  /*!__ARM_FEATURE_DSP*/
    register int64_t t = r*r;

    t = CUBIC_TERM_CONSTANT * (t >> 31);
    t = (t * r) >> 31;
    return r + (t >> 31);
#endif /*__ARM_FEATURE_DSP*/
}

//! \brief This function calculates a range-reduced log function.
//! \param[in] x is a u0.31 fraction.
//! \return A value representing log(1+x) in u5.27 (or s4.27) format.

static inline uint32_t log_1_2(
	uint32_t x)
{
    register uint32_t k = uint32_round(x, 26);
    register union { uint32_t u; int32_t i; } z;
    register int32_t r;

/*
    k = bitsuk(rounduk(ukbits(x) >> 25, 1)) >> 1;
    log_info("x = %R (%u), k = %u", x >> 16, x, k);
*/

    assert(k <= 64);

    z.u = x - (k << 26);

    // At this point z.i holds an s0.31 representation of the remainder
    // ... which is known to be in [-1/128, 1/128].

    r = divide_ck(z.i, k);

    // At this point r holds an s0.31 representation of the x/ck

    // The following approximation is outlined on page 72-3 of J-M Muller.
    // But, we have adapted the coefficients using sollya.
    //
    // log(r) = r + 44739601/2^29 * r^3
    //
    // However, since r is very small, the second term is _almost_
    // small enough to be ignored.

#ifndef FAST_LOG
    r = cubic_term(r);
#endif /* FAST_LOG */
    r = log_ck[k] + (r >> 5);
    return r;
}

//! \brief This function calculates the internal representation of
//! the log function.
//! \param[in] x is a positive signed accum.
//! \return A value representing log(x).
static inline int32_t logkbits(
	int32_t x)
{
    register int32_t r;
    register int shift = __clz(x);
    register uint32_t z = ((uint32_t) x) << (shift + 1);

    assert(x > 0);

    //log_info("shift = %u, z = %R (%u), x = %k (%d)", shift, z >> 16, z, x, x);

    r = (int32_t) log_1_2(z);

    //log_info("r before: %k (%d)", r >> 12, r);

    r = subtract_mult_log2(r, shift - 16);

    //log_info("r shifted: %k (%d)", r >> 12, r);

    r = uint32_round(r, 12);		// round result from u5.27 to s16.15

    //log_info("r after:  %k (%d)", r, r);

    r -= 393216;			// subtract 12 * 2^15
    assert(-340696 <= r && r <= 363409);
    return r;
}

//! \brief This function calculates the log function for the
//! accum type (s16.15).
//! \param[in] x is positive value represented as an accum.
//! \return A value representing log(x) in accum format.

accum logk(
	accum x)
{
    return kbits(logkbits(bitsk(x)));
}

/* Disassembly of compiled code:

Disassembly of section .text:

00000000 <logk>:
   0:	e16f2f10 	clz	r2, r0
   4:	e2823001 	add	r3, r2, #1
   8:	e1a00310 	lsl	r0, r0, r3
   c:	e1a03ca0 	lsr	r3, r0, #25
  10:	e2033001 	and	r3, r3, #1
  14:	e0833d20 	add	r3, r3, r0, lsr #26
  18:	e59fc03c 	ldr	ip, [pc, #60]	; 5c <logk+0x5c>
  1c:	e1a01083 	lsl	r1, r3, #1
  20:	e0400d03 	sub	r0, r0, r3, lsl #26
  24:	e19c10f1 	ldrsh	r1, [ip, r1]
  28:	e1200180 	smlawb	r0, r0, r1, r0
  2c:	e08c3103 	add	r3, ip, r3, lsl #2
  30:	e5933084 	ldr	r3, [r3, #132]	; 0x84
  34:	e2422010 	sub	r2, r2, #16
  38:	e08302c0 	add	r0, r3, r0, asr #5
  3c:	e1a02502 	lsl	r2, r2, #10
  40:	e59f3018 	ldr	r3, [pc, #24]	; 60 <logk+0x60>
  44:	e1200382 	smlawb	r0, r2, r3, r0
  48:	e1a035a0 	lsr	r3, r0, #11
  4c:	e2033001 	and	r3, r3, #1
  50:	e0830620 	add	r0, r3, r0, lsr #12
  54:	e2400806 	sub	r0, r0, #393216	; 0x60000
  58:	e12fff1e 	bx	lr
  5c:	00000000 	andeq	r0, r0, r0
			5c: R_ARM_ABS32	.rodata
  60:	ffe9d1bd 			; <UNDEFINED> instruction: 0xffe9d1bd

*/
