//! \file

#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include "arm_acle.h"

#ifdef __ARM_FEATURE_DSP

//! \brief Horner evaluation of a polynomial of signed accum at
//! a point given by the lower (signed) 16-bits of x.
//! \param[in] a The 32-bit signed polynomial coefficients. These can
//! be treated as either accum or long fract (or any other signed 32-bit
//! quantity.
//! \param[in] x The point at which the polynomial is to be evaluated
//! is given by treating the lower 16-bits as a signed fract.
//! \param[in] n The number of coeficients in the polynomial.
//! \return The result as a signed 32-bit quantity.

static inline int __horner_int_b (int* a, int x, int n)
{
    register int r, t;

    asm volatile ("ldr %[r], [%[a]], #4" 
                  : [r] "=r" (r), [a] "+r" (a) : : );
 
   do {
        asm volatile ("ldr %[t], [%[a]], #4" 
                      : [t] "=r" (t), [a] "+r" (a) : : );
        r = __smlawb (r, x, t);
        n--;
    } while (n > 0);

    return (r);
}

/* Above generates:

00000000 <expk>:
   0:	e59f3024 	ldr	r3, [pc, #36]	; 2c <expk+0x2c>
   4:	e4932004 	ldr	r2, [r3], #4
   8:	e4931004 	ldr	r1, [r3], #4
   c:	e1221082 	smlawb	r2, r2, r0, r1
  10:	e4931004 	ldr	r1, [r3], #4
  14:	e1221082 	smlawb	r2, r2, r0, r1
  18:	e4931004 	ldr	r1, [r3], #4
  1c:	e1221082 	smlawb	r2, r2, r0, r1
  20:	e4931004 	ldr	r1, [r3], #4
  24:	e1201082 	smlawb	r0, r2, r0, r1
  28:	e12fff1e 	bx	lr
  2c:	00000000 	andeq	r0, r0, r0
			2c: R_ARM_ABS32	.data
*/


/*

static inline int __horner_int_b (int* a, int x, int n)
{
    register int r = *a++;

    for ( ; n > 0; n--)
        r = __smlawb (r, x, *a++);

    return (r);
}


00000000 <expk>:
   0:	e59f302c 	ldr	r3, [pc, #44]	; 34 <expk+0x34>
   4:	e893000c 	ldm	r3, {r2, r3}
   8:	e1233082 	smlawb	r3, r2, r0, r3
   c:	e59f2020 	ldr	r2, [pc, #32]	; 34 <expk+0x34>
  10:	e5922008 	ldr	r2, [r2, #8]
  14:	e1232083 	smlawb	r3, r3, r0, r2
  18:	e59f2014 	ldr	r2, [pc, #20]	; 34 <expk+0x34>
  1c:	e592200c 	ldr	r2, [r2, #12]
  20:	e1232083 	smlawb	r3, r3, r0, r2
  24:	e59f2008 	ldr	r2, [pc, #8]	; 34 <expk+0x34>
  28:	e5922010 	ldr	r2, [r2, #16]
  2c:	e1202083 	smlawb	r0, r3, r0, r2
  30:	e12fff1e 	bx	lr
  34:	00000000 	andeq	r0, r0, r0
			34: R_ARM_ABS32	.data

*/

//! \brief Horner evaluation of a polynomial of signed accum at
//! a point given by the upper (signed) 16-bits of x.
//! \param[in] a The 32-bit signed polynomial coefficients. These can
//! be treated as either accum or long fract (or any other signed 32-bit
//! quantity.
//! \param[in] x The point at which the polynomial is to be evaluated
//! is given by treating the upper 16-bits as a signed fract.
//! \param[in] n The number of coeficients in the polynomial.
//! \return The result as a signed 32-bit quantity.

static inline int __horner_int_t (int* a, int x, int n)
{
    register int r = *a++;

    for ( ; n > 0; n--)
        r = __smlawt (r, x, *a++);

    return (r);
}

#else  /*!__ARM_FEATURE_DSP*/

//! \brief Horner evaluation of a polynomial of signed accum at
//! a point given by the lower (signed) 16-bits of x.
//! \param[in] a The 32-bit signed polynomial coefficients. These can
//! be treated as either accum or long fract (or any other signed 32-bit
//! quantity.
//! \param[in] x The point at which the polynomial is to be evaluated
//! is given by treating the lower 16-bits as a signed fract.
//! \param[in] n The number of coeficients in the polynomial.
//! \return The result as a signed 32-bit quantity.

static inline int __horner_r_b (int* a, int x, int n)
{
    register int64_t t = *a++;

    for ( ; n > 0; n--)
        t = (t * (x & 0xFFFF) >> 15) + *a++;

    return ((int)(t & 0xFFFFFFFF));
}

//! \brief Horner evaluation of a polynomial of signed accum at
//! a point given by the upper (signed) 16-bits of x.
//! \param[in] a The 32-bit signed polynomial coefficients. These can
//! be treated as either accum or long fract (or any other signed 32-bit
//! quantity.
//! \param[in] x The point at which the polynomial is to be evaluated
//! is given by treating the upper 16-bits as a signed fract.
//! \param[in] n The number of coeficients in the polynomial.
//! \return The result as a signed 32-bit quantity.

static inline int __horner_r_t (int* a, int x, int n)
{
    register int64_t t = *a++;

    for ( ; n > 0; n--)
        t = (t * (x >> 16) >> 15) + *a++;

    return ((int)(t & 0xFFFFFFFF));
}

#endif /*__ARM_FEATURE_DSP*/

#endif /*__POLYNOMIAL_H__*/
