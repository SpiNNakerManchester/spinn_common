
#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdfix.h>
#include "stdfix-full-iso.h"
#include "debug.h"

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.

int __norminv_rbits(int x);
int __norminv_ulrbits(unsigned int x);

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.

static inline int __norminv_t_bits(int x)
{
    return __norminv_rbits(x >> 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed 16-bit PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed int representation of an s16.15 PRNG.

static inline int __norminv_b_bits(int x)
{
    return __norminv_t_bits(x << 16);
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the bottom
//! 16 bits of x.
//! \return A normally distributed accum PRNG.

static inline accum norminv_urb(unsigned int x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_b_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed 16-bit PRNG, in the top
//! 16 bits of x.
//! \return A normally distributed accum PRNG.

static inline accum norminv_urt(unsigned int x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = x;
    return kbits(__norminv_t_bits(tmp.s));
}

//! \brief This function takes a uniformly distributed 16-bit
//! PRNG x, and returns a normally distributed accum PRNG.
//! \param[in] x A uniformly distributed unsigned fract.
//! \return A normally distributed accum PRNG.

static inline accum norminv_ur(unsigned fract x)
{
    union { unsigned int u; int s; } tmp;

    tmp.u = bitsur(x);
    return kbits(__norminv_rbits(tmp.s + INT16_MIN));
}

static inline accum norminv_ulr(unsigned long fract x)
{
    return kbits(__norminv_ulrbits(bitsulr(x)));
}

#define norminv_fx(f) \
    ({	accum tmp;							\
	if (__builtin_types_compatible_p(__typeof__(f), unsigned fract))\
	    tmp = norminv_ur(f);					\
	else if (__builtin_types_compatible_p(__typeof__(f), unsigned long fract)) \
	    tmp = norminv_ulr(f);					\
	else								\
	    abort(1);							\
	tmp;								\
    })

#endif /*__NORMAL_H__*/
