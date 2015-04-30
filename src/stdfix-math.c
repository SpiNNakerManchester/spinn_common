/*! \file
 *
 *  \brief API for fixed point transcendental functions
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2012-14.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 5 December, 2014
 *
 *  DETAILS
 *    Created on       : 5 December 2014
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#include "stdfix-full-iso.h"
#include "debug.h"
#include "arm_acle.h"
#include "pair.h"
#include "polynomial.h"
#include "stdfix-math-impl.h"
#include "sark.h"

/**********************************************************************/
/*                                                                    */
/*                      Error Handling                                */
/*                                                                    */
/**********************************************************************/

static const char* stdfix_string        = "stdfix error: %s %s\n";
static const char* unimplemented_string = "unimplemented";
static const char* negative_string      = "of negative argument";
//static const char* non_positive_string  = "of non-positive argument";
static const char* unknown_string       = "stdfix error: unknown error (%d)\n";
static const char* overflow_string      = "overflow in";
static const char* exp_string  = "exp";
static const char* log_string  = "log";
static const char* sin_string  = "sin";
static const char* cos_string  = "exp";
static const char* sqrt_string = "sqrt";

#define error_msg(s, ...)                                              \
    do { io_printf (IO_BUF, (char*)(s), ##__VA_ARGS__); } while (0)

void stdfix_error (int error)
{
    switch (error) {
    case __EXP_UNIMPLEMENTED:
        error_msg (stdfix_string, exp_string, unimplemented_string);
    break;

    case __LOG_UNIMPLEMENTED:
        error_msg (stdfix_string, log_string, unimplemented_string);
    break;

    case __SIN_UNIMPLEMENTED:
        error_msg (stdfix_string, sin_string, unimplemented_string);
    break;

    case __COS_UNIMPLEMENTED:
        error_msg (stdfix_string, cos_string, unimplemented_string);
    break;

    case __SQRT_UNIMPLEMENTED:
        error_msg (stdfix_string, sqrt_string, unimplemented_string);
    break;

    case SQRT_NEG:
        error_msg (stdfix_string, sqrt_string, negative_string);
    break;

    case EXP_OVERFLOW:
        error_msg (stdfix_string, overflow_string, exp_string);
    break;

    default:
        io_printf (IO_BUF, (char*)(unknown_string), error);
    break;
  }
}


/**********************************************************************/
/*                                                                    */
/*                      Logarithm Function                            */
/*                                                                    */
/**********************************************************************/

// ck = 1 + k/64
//
// Thus log_ck [k] = log (1 + k/64) + 12 in u5.27

static uint32_t log_ck [] =
    { 1610612736, 1612693673, 1614742838, 1616761188,
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
      1703645376};

static int16_t recip_table [] =
    {     0,  -1008,  -1986,  -2934,  -3855,  -4749,  -5617,  -6461,
      -7282,  -8080,  -8856,  -9612, -10348, -11065, -11763, -12444,
     -13107, -13754, -14386, -15002, -15604, -16191, -16765, -17326,
     -17873, -18409, -18933, -19445, -19946, -20436, -20916, -21385,
     -21845, -22296, -22737, -23169, -23593, -24008, -24415, -24815,
     -25206, -25590, -25967, -26337, -26700, -27056, -27406, -27749,
     -28087, -28418, -28744, -29064, -29378, -29687, -29991, -30290,
     -30583, -30872, -31156, -31436, -31711, -31982, -32248, -32510,
     -32768};

//! \brief This function divides x by k/64; it does not use
//! Arm DSP instructions.
//! \param[in] x input value
//! \param[in] k the `breakpoint' index.
//! \return x/(k/64)

static inline int32_t divide_ck_default (int32_t x, uint32_t k)
{
    register int64_t r;

    r = (int64_t)(x);
    r = 2*r + ((r * recip_table [k]) >> 16);

    return ((int32_t)(r));
}
 
//! \brief This function divides x by k/64; it uses the
//! Arm DSP instructions.
//! \param[in] x input value
//! \param[in] k the `breakpoint' index.
//! \return x/(k/64)

static inline int32_t divide_ck_arm (int32_t x, uint32_t k)
{   return (__smlawb (x, recip_table [k], x)); }

//! \brief This function divides x by k/64.
//! \param[in] x input value
//! \param[in] k the `breakpoint' index.
//! \return x/(k/64)

static inline int32_t divide_ck (int32_t x, uint32_t k)
{
    register int32_t r;

    assert (-33554432 <= x && x <= 33554432);
    assert (k <= 64);

#ifdef   __ARM_FEATURE_DSP
    r = divide_ck_arm (x, k);
#else  /*__ARM_FEATURE_DSP*/
    r = divide_ck_default (x, k);
#endif /*__ARM_FEATURE_DSP*/

    return (r);
}

static inline uint32_t uint32_round (uint32_t r, uint32_t n)
{
    r = (r >> n) + ((r >> (n-1)) & 0x1);

    return (r);
}

//! \brief This function performs a multiply-accumulate
//! \param[in] r accumulator value
//! \param[in] x one of the factors
//! \return r := r - x * log (2).

static inline int32_t subtract_mult_log2_default (int32_t r, int32_t x)
{   return (r - x * 93032640); }

//! \brief This function performs a multiply-accumulate
//! \param[in] r accumulator value
//! \param[in] x one of the factors
//! \return r := r - x * log (2).

static inline int32_t subtract_mult_log2_arm (int32_t r, int32_t x)
{   return (__smlawb (x << 10, -1453635, r)); }

//! \brief This function performs a multiply-accumulate
//! \param[in] r accumulator value
//! \param[in] x one of the factors
//! \return r := r - x * log (2).

static inline int32_t subtract_mult_log2 (int32_t r, int32_t x)
#ifdef   __ARM_FEATURE_DSP
{   return (subtract_mult_log2_default (r, x)); }
//{   return (subtract_mult_log2_arm (r, x)); }
#else  /*__ARM_FEATURE_DSP*/
{   return (subtract_mult_log2_default (r, x)); }
#endif /*__ARM_FEATURE_DSP*/

static inline int32_t cubic_term_default (int32_t r)
{
    register int64_t t = r*r;

    t = 178958404 * (t >> 31);
    t = (t * r) >> 31;

    return (r + (t >> 31));
}

static inline int32_t cubic_term_arm (int32_t r)
{
    register int32_t t = __smultt (r, r);

    t = __smulwt (178958404*8, t);
    t = __smulwt (t, r);

    /*    if (t != 0)
        log_info ("%d %d (= %d) ", r, t, cubic_term_default (r));
    else
    log_info (". ");*/

    return (r + t);
}


static inline int32_t cubic_term (int32_t r)
#ifdef   __ARM_FEATURE_DSP
{   return (cubic_term_arm (r)); }
#else  /*__ARM_FEATURE_DSP*/
{   return (cubic_term_default (r)); }
#endif /*__ARM_FEATURE_DSP*/

//! \brief This function calculates a range-reduced log function.
//! \param[in] x is a u0.31 fraction.
//! \return A value representing log (1+x) in u5.27 (or s4.27) format.

static inline uint32_t log_1_2 (uint32_t x)
{
    register uint32_t k = uint32_round (x, 26);
    register union {uint32_t u; int32_t i;} z;
    register int32_t r;

    /*k  = bitsuk (rounduk (ukbits (x) >> 25, 1)) >> 1;
      log_info ("x = %R (%u), k = %u", x >> 16, x, k); */

    assert (k <= 64);

    z.u = x - (k << 26);

    // At this point z.i holds an s0.31 representation of the remainder
    // ... which is known to be in [-1/128, 1/128].

    r = divide_ck (z.i, k);

    // At this point r holds an s0.31 representation of the x/ck

    // The following approximation is outlined on page 72-3 of J-M Muller.
    // But, we have adapted the coefficients using sollya.
    //
    // log(r) = r + 44739601/2^29 * r^3
    //
    // However, since r is very small, the second term is _almost_
    // small enough to be ignored.

#ifndef   FAST_LOG
    r = cubic_term (r);
#endif /* FAST_LOG */

    r = log_ck [k] + (r >> 5);

    return (r);
}

//! \brief This function calculates the internal representation of
//! the log function.
//! \param[in] x is a positive signed accum.
//! \return A value representing log (x).

static inline int32_t logkbits (int32_t x)
{
    register int32_t  r;
    register int      shift = __clz (x);
    register uint32_t z     = ((uint32_t)(x)) << (shift + 1);

    assert (x > 0);

    //log_info ("shift = %u, z = %R (%u), x = %k (%d)", shift, z >> 16, z, x, x);

    r  = (int32_t)(log_1_2 (z));

    //log_info ("r before: %k (%d)", r >> 12, r);

    r  = subtract_mult_log2 (r, shift - 16);

    //log_info ("r shifted: %k (%d)", r >> 12, r);

    r  = uint32_round (r, 12);            // round result from u5.27 to s16.15

    //log_info ("r after:  %k (%d)", r, r);

    r -= 393216;                          // subtract 12 * 2^15

    assert (-340696 <= r && r <= 363409);

    return (r);
}

//! \brief This function calculates the log function for the
//! accum type (s16.15).
//! \param[in] x is positive value representated as an accum.
//! \return A value representing log (x) in accum format.

accum logk (accum x)
{   return (kbits (logkbits (bitsk (x)))); }

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

/**********************************************************************/
/*                                                                    */
/*                      Exponential Function                          */
/*                                                                    */
/**********************************************************************/

//! \brief This exp_hi_table [n] contains the s16.15 value of
//! exp ((n-22)/2). Thus exp_hi_table [22] contains exp (0.0) = 1.0,
//! represented by the bit pattern 0x00007FFF

static int32_t exp_hi_table[]
  = {         1,  //     0.00001670170079024566
              1,  //     0.000027536449349747158
              1,  //     0.000045399929762484854
              2,  //     0.0000748518298877006
              4,  //     0.00012340980408667956
              7,  //     0.00020346836901064417
             11,  //     0.00033546262790251185
             18,  //     0.0005530843701478336
             30,  //     0.0009118819655545162
             49,  //     0.0015034391929775724
             81,  //     0.0024787521766663585
            134,  //     0.004086771438464067
            221,  //     0.006737946999085467
            364,  //     0.011108996538242306
            600,  //     0.01831563888873418
            990,  //     0.0301973834223185
           1631,  //     0.049787068367863944
           2690,  //     0.0820849986238988
           4435,  //     0.1353352832366127
           7312,  //     0.22313016014842982
          12055,  //     0.36787944117144233
          19875,  //     0.6065306597126334
          32768,  //     1.0
          54025,  //     1.6487212707001282
          89073,  //     2.718281828459045
         146856,  //     4.4816890703380645
         242125,  //     7.38905609893065
         399196,  //    12.182493960703473
         658163,  //    20.085536923187668
        1085127,  //    33.11545195869231
        1789072,  //    54.598150033144236
        2949681,  //    90.01713130052181
        4863202,  //   148.4131591025766
        8018065,  //   244.69193226422038
       13219555,  //   403.4287934927351
       21795361,  //   665.1416330443618
       35934475,  //  1096.6331584284585
       59245934,  //  1808.0424144560632
       97680031,  //  2980.9579870417283
      161047145,  //  4914.768840299134
      265521854,  //  8103.083927575384
      437771529,  // 13359.726829661873
      721763231,  // 22026.465794806718
     1189986392,  // 36315.502674246636
     1961955876}; // 59874.14171519782

//! \brief This table represents a low-order ploynomial approximation
//! to 2 * (exp(x) - (1 + x + x^2/2)), for the interval [-0.25, 0.25)
//!
//! It is generated by sollya:
//!
//!   > display = powers!;
//!   > E6 = fpminimax (2*(exp(x)-(1+x+x^2/2)),6,[|31...|],[-1/4;1/4], fixed, absolute);
//!
//!   > E6;
//!
//!      x * (23 * 2^(-31) + x * (7 * 2^(-31) + x * (715824955 * 2^(-31) +
//!          x * (89478197 * 2^(-30) + x * (35884827 * 2^(-31) +
//!              x * (5979315 * 2^(-31)))))))
//!
//! Converting to decimals, we get:
//!
//!   > display=decimal;
//!   > E6;
//!
//!      x * (1.07102096080780029296875e-8 +
//!          x * (3.2596290111541748046875e-9 +
//!              x * (0.3333319700323045253753662109375 +
//!                  x * (8.3333064801990985870361328125e-2 +
//!                      x * (1.67101747356355190277099609375e-2 +
//!                          x * 2.7843355201184749603271484375e-3)))))
//!
//! The quoted error is bounded by:
//!
//!   > supnorm(E6, exp(x)-1, [-1/4;1/4], absolute, 2^(-100));
//!
//!      [3.9143792242792689818577649741072874668644607349907e-10;
//!       3.9143792242792689818577649741102788707046338990456063e-10]
//!
//! IMPORTANT: Note we have omitted the leading power of 0! This means that we
//!            have to remember to multiply the result by x to obtain the
//!            required approximation.

static int exp_lo_table[]
   = {    5979315,  //  0.0027843355201184749603271484375
         35884827,  //  0.0167101747356355190277099609375
        178956394,  //  0.0833330648019909858703613281250
        715824955,  //  0.3333319700323045253753662109375
                7,  //  0.0000000032596290111541748046875
               23}; //  0.0000000107102096080780029296875


//! This function calculates an approximation to 2*(exp(x)-(1+x+x^2/2))
//!
//! It is used in conjunction with mid_approx because we wish to avoid
//! overflow in the polynomial calculation: (1 + x * (...)) which is part
//! of the standard approximation to exp (x).
//!
//! IMPORTANT: Note we have omitted the leading power of 0 in the coeffcients.
//!            This means that we have to multiply the result by x to obtain the
//!            required approximation.
//!
//! \param[in] x A signed int in the low 16 bits of a 32-bit signed int.
//! \return An approximation to 2*(exp(x)-(1+x+x^2/2)) as an s0.31

static inline int32_t exp_lo_approx (int* low_table, int32_t x)
{
    register int32_t r;

    assert (-8192 <= x && x <  8192);             // -0.25 <= x < 0.25

    r = __horner_int_b (low_table, x << 1, 5);
    r = __smulwb (r, x << 1);                     // Notice the multiplication by x

    assert (-10519306 <= r && r < 1192032309);    // -0.004898433481798747 <= r < 0.005550832996846111
 
    return (r);
}

//! This function calculates (2*x + x^2).
//!
//! It is used because we wish to avoid overflow in the polynomial calculation:
//! (1 + x * (...)) which is part of the standard approximation to exp (x).
//!
//! \param[in] x A signed int in the low 16 bits of a 32-bit signed int.
//! \return The value (2*x+x^2) respresented as an s0.31.

static inline int32_t exp_mid_approx (int32_t x)
{
    register int32_t r;

    assert (-8192 <= x && x <  8192);             // -0.25 <= x < 0.25

    r = __smlabb (x, x << 1, x << 17);

    //check (-939524096 <= r && r < 1207959552, "x = %d (-939524096 <= %d < 1207959552)", x, r);

    assert (-939524096 <= r && r < 1207959552);  // -0.4375 <= 2*x + x^2 < 0.5625

    return (r);
} 

//! This function takes as input an signed int -- treated as an s16.15 fixed point value --
//! and calculates a signed int repesenting exp (x), again treated as an s16.15 fixed point value.
//!
//! Note that although the results saturate, the saturation flag is not set.
//!
//! Note: the result is close, but not exact. DRL believes it may be a few bits out.
//!
//! \param[in] x A signed int to be treated as an s16.15 fixed point value.
//! \return The value exp (x), again treated as an s16.15 fixed point value.

static inline int32_t expkbits (int32_t x)
{
    register int32_t r, tmp;
    register int32_t hi, lo;

    if (x < -363408) return (0);
    if (363408 <  x) return (INT32_MAX);

    assert (-363408 <= x && x <= 363408); // -11.09033203125 <= x <= 11.09033203125

    hi = (x + (1 << 13)) >> 14; // hi bits;
    lo = x - (hi << 14);
 
    assert (  -22 <= hi && hi <=   22);
    assert (-8192 <= lo && lo <  8192);

    tmp = exp_hi_table [hi+22];
    r   = tmp;

    r += ((int64_t)(tmp) * (int64_t)(exp_mid_approx (lo) +
                                     exp_lo_approx  (&(exp_lo_table [0]), lo))) >> 32;

    return (r);
}

//! \brief This function calculates the log function for the
//! accum type (s16.15).
//! \param[in] x is positive value representated as an accum.
//! \return A value representing log (x) in accum format.

accum expk (accum x)
{   return (kbits (expkbits (bitsk (x)))); }

/**********************************************************************/
/*                                                                    */
/*                      Square-root Function                          */
/*                                                                    */
/**********************************************************************/

//! This function takes as input an signed int -- treated as an s16.15 fixed point value --
//! and calculates a signed int repesenting exp (x), again treated as an s16.15 fixed point value.
//!
//! Note that although the results saturate, the saturation flag is not set.
//!
//! Note: the result is close, but not exact. DRL believes it may be a few bits out.
//!
//! \param[in] x A signed int to be treated as an s16.15 fixed point value.
//! \return The value exp (x), again treated as an s16.15 fixed point value.

static inline int32_t sqrtkbits (int32_t x)
{
    register int32_t r, l;

    if (x <= 0) return (0);

    assert (0 < x);

    l = logkbits (x);
    r = expkbits (l >> 1);

    // assert (180 <= r && r <= 8388608);

    return (r);
}


accum sqrtk (accum x)
{   return (kbits (sqrtkbits (bitsk (x)))); }
