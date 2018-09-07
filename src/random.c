/*! \file random.c
 *  \brief pseudo-random number generators
 *  \author Michael Hopkins
 *  \date February 2014
 */

#include "random.h"
#include "normal.h"

#include "stdfix-full-iso.h"
#include "stdfix-exp.h"

/*
  By Michael Hopkins, Manchester University

  For all of these, to get a high resolution fixed-point number in (0,1) that
  uses the entire generator space use ulrbits() around return value to give a
  32-bit unsigned long fract

  Also other options to get a 16-bit unsigned fract by taking 16 MSBs of result
  and urbits()

  If floating point is available then multiply by 2.32830643653869628906e-10

  spin1_rand() takes 134.9 nanosecs (i.e. 27 ticks) per call

  WELL 1024a long cycle generator (2^1024 - 1) from L'Ecuyer & Panneton

  Better equi-distribution and warm-up performance than Mersenne Twister, and
  faster

  294.9 nanosecs (i.e. 59 ticks) per call
*/
#define R 32

// NB the global state vector for WELL1024a_simp() is defined here as a file
// static
static uint32_t STATE[R];

#define W	32
#define M1	3
#define M2	24
#define M3	10

#define MAT0POS(t, v)	((v) ^ ((v)>>(t)))
#define MAT0NEG(t, v)	((v) ^ ((v)<<(-(t))))
#define Identity(v)	(v)

#define V0	STATE[state_i                   ]
#define VM1	STATE[(state_i+M1) & 0x0000001fU]
#define VM2	STATE[(state_i+M2) & 0x0000001fU]
#define VM3	STATE[(state_i+M3) & 0x0000001fU]
#define VRm1	STATE[(state_i+31) & 0x0000001fU]
#define newV0	STATE[(state_i+31) & 0x0000001fU]
#define newV1	STATE[state_i                   ]

// the initialiser function that MUST BE CALLED before WELL1024a_simp() is used
void init_WELL1024a_simp(void)
{
    // state_i = 0;
    for (int j = 0; j < R; j++) {
	STATE[j] = mars_kiss64_simp();
                         // init state vector with good generator
	// io_printf(IO_STD, " %u , ", STATE[j]);
    }
    // io_printf(IO_STD, "\n");
}

// the WELL generator itself
uint32_t WELL1024a_simp(void)
{
    static uint32_t state_i = 0;
    uint32_t z0, z1, z2;

    z0    = VRm1;
    z1    = Identity(V0)      ^ MAT0POS(  8, VM1);
    z2    = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = z1                ^ z2;
    newV0 = MAT0NEG(-11, z0)  ^ MAT0NEG(-7, z1)    ^ MAT0NEG(-13, z2);
    state_i = (state_i + 31) & 0x0000001fU;
    return STATE[state_i];
}

// MUST USE THIS before working with proposed seed for the first time
void validate_WELL1024a_seed(
	WELL1024a_seed_t seed)
{
    seed[33] = 0;
}

// the WELL generator for custom seed - STATE in function argument replaces
// file static version
uint32_t WELL1024a_seed(
	WELL1024a_seed_t STATE)
{
#define state_i STATE[33] // need to store original static variable as extra element of seed

    uint32_t z0, z1, z2;

    z0    = VRm1;
    z1    = Identity(V0)      ^ MAT0POS(  8, VM1);
    z2    = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = z1                ^ z2;
    newV0 = MAT0NEG(-11, z0)  ^ MAT0NEG(-7, z1)   ^ MAT0NEG(-13, z2);
    state_i = (state_i + 31) & 0x0000001fU;
    return STATE[state_i];

#undef state_i
}

#undef W
#undef R
#undef M1
#undef M2
#undef M3

#undef MAT0POS
#undef MAT0NEG
#undef Identity

#undef V0
#undef VM1
#undef VM2
#undef VM3
#undef VRm1
#undef newV0
#undef newV1

// end of L'Ecuyer WELL1024a definition

/*
   Implementation of a Marsaglia 32-bit KISS generator which uses no multiply
   instructions

   209.9 nanosecs (i.e. 42 ticks) per call
*/
uint32_t mars_kiss32(void)
{
    static uint32_t
        x = 123456789,
        y = 234567891,
        z = 345678912,
        w = 456789123,
        c = 0; /* Seed variables */
    int32_t t;

    y ^= y << 5;
    y ^= y >> 7;
    y ^= y << 22;
    t = z + w + c;
    z = w;
    c = t < 0;
    w = t & 2147483647;
    x += 1411392427;
    return (uint32_t) x + y + w;
}

/*
   Implementation of Marsaglia JKISS RNG uses 64-bit value and 2x multiplies

   219.9 nanosecs (i.e. 44 ticks) per call
*/
uint32_t mars_kiss64_simp(void)
{
    static uint32_t
        x = 123456789,
        y = 987654321,
        z = 43219876,
        c = 6543217; /* Seed variables */
    uint64_t t;

    x = 314527869 * x + 1234567;
    y ^= y << 5;
    y ^= y >> 7;
    y ^= y << 22;
    t = 4294584393ULL * z + c;
    c = t >> 32;
    z = t;

    return (uint32_t) x + y + z;
}

/*
   Custom seed version of above - need to create & validate 128 bit seed
   before use
*/
uint32_t mars_kiss64_seed(
	mars_kiss64_seed_t seed)
{
#define x seed[0]
#define y seed[1]
#define z seed[2]
#define c seed[3]

    uint64_t t;

    x = 314527869 * x + 1234567;
    y ^= y << 5;
    y ^= y >> 7;
    y ^= y << 22;
    t = 4294584393ULL * z + c;
    c = t >> 32;
    z = t;

    return (uint32_t) x + y + z;

#undef x
#undef y
#undef z
#undef c
}

// validate seed for Marsaglia KISS 64 before it is first used
void validate_mars_kiss64_seed(
	mars_kiss64_seed_t seed)
{
    if (seed[1] == 0) {
	seed[1] = 13031301;
	// y (<- seed[2]) can't be zero so set to arbitrary non-zero if so
    }

    seed[3] = seed[3] % 698769068 + 1;
    // avoid z=c=0 and make < 698769069
}

/***************************************************

    Non-uniform RNGs

    Being added to all the time, and updated for better speed as
    fixed-point transcendentals and optimized multiplies become available

****************************************************/
/*
 * 	Von Neumann's exponential distribution generator
 *
 * 	from Ripley p.230 and adapted for our types
 *
 * 	Mean number of U(0,1) per call = 5.2
 */
accum exponential_dist_variate(
	uniform_rng uni_rng,
	uint32_t* seed_arg)
{
    /*
     * DKF: Converted this function to use conventional C structured
     * programming. This moves the assignment of the register holding the
     * 1.0k to before the loop and alters the register assignment, but
     * otherwise leaves the code functionally identical.
     *
     * TODO: DRL and MH need to investigate whether this matters. It depends
     * on the expected number of additions of that value, with a likely
     * (minor) net performance gain when the expectation exceeds 1, and is a
     * loss when the expectation is less than 1. It's whether the cost of
     * setting a register up front is small enough that it's beats the
     * probabilistic number of sets of a register further down the code.
     *
     * ASM generated using 'goto's:
     *   0:   e92d47f0        push    {r4, r5, r6, r7, r8, r9, sl, lr}
     *   4:   e1a07000        mov     r7, r0
     *   8:   e1a06001        mov     r6, r1
     *   c:   e3a08000        mov     r8, #0
     *  10:   e1a00006        mov     r0, r6
     *  14:   e12fff37        blx     r7
     *  18:   e1a09000        mov     r9, r0
     *  1c:   e1a04000        mov     r4, r0
     *  20:   ea000003        b       34 <exponential_dist_variate+0x34>
     *  24:   e12fff37        blx     r7
     *  28:   e1550000        cmp     r5, r0
     *  2c:   e1a04000        mov     r4, r0
     *  30:   9a000009        bls     5c <exponential_dist_variate+0x5c>
     *  34:   e1a00006        mov     r0, r6
     *  38:   e12fff37        blx     r7
     *  3c:   e1a05000        mov     r5, r0
     *  40:   e1540005        cmp     r4, r5
     *  44:   e1a00006        mov     r0, r6
     *  48:   2afffff5        bcs     24 <exponential_dist_variate+0x24>
     *  4c:   e1a00009        mov     r0, r9
     *  50:   ebfffffe        bl      0 <__gnu_fractusqsa>
     *  54:   e0800008        add     r0, r0, r8
     *  58:   e8bd87f0        pop     {r4, r5, r6, r7, r8, r9, sl, pc}
     *  5c:   e3a03902        mov     r3, #32768      ; 0x8000
     *  60:   e0888003        add     r8, r8, r3
     *  64:   eaffffe9        b       10 <exponential_dist_variate+0x10>
     *
     * ASM generated using 'while' and 'do/while':
     *   0:   e92d47f0        push    {r4, r5, r6, r7, r8, r9, sl, lr}
     *   4:   e1a07000        mov     r7, r0
     *   8:   e1a06001        mov     r6, r1
     *   c:   e3a08000        mov     r8, #0
     *  10:   e3a0a902        mov     sl, #32768      ; 0x8000
     *  14:   e1a00006        mov     r0, r6
     *  18:   e12fff37        blx     r7
     *  1c:   e1a09000        mov     r9, r0
     *  20:   e1a04000        mov     r4, r0
     *  24:   ea000003        b       38 <exponential_dist_variate+0x38>
     *  28:   e12fff37        blx     r7
     *  2c:   e1550000        cmp     r5, r0
     *  30:   e1a04000        mov     r4, r0
     *  34:   9a000009        bls     60 <exponential_dist_variate+0x60>
     *  38:   e1a00006        mov     r0, r6
     *  3c:   e12fff37        blx     r7
     *  40:   e1a05000        mov     r5, r0
     *  44:   e1540005        cmp     r4, r5
     *  48:   e1a00006        mov     r0, r6
     *  4c:   2afffff5        bcs     28 <exponential_dist_variate+0x28>
     *  50:   e1a00009        mov     r0, r9
     *  54:   ebfffffe        bl      0 <__gnu_fractusqsa>
     *  58:   e0800008        add     r0, r0, r8
     *  5c:   e8bd87f0        pop     {r4, r5, r6, r7, r8, r9, sl, pc}
     *  60:   e088800a        add     r8, r8, sl
     *  64:   eaffffea        b       14 <exponential_dist_variate+0x14>
     */
    accum A = 0.0k;
    uint32_t U, U0, USTAR;

    while (true) {
	U = uni_rng(seed_arg);
	U0 = U;

	do {
	    USTAR = uni_rng(seed_arg);
	    if (U < USTAR) {
		return A + (accum) ulrbits(U0);
                   // accum + (accum)[ unsigned long fract <= uint32_t ]
	    }

	    U = uni_rng(seed_arg);
	} while (U < USTAR);

	A += 1.0k;
    }
}

// Returns standard gaussian deviate
accum gaussian_dist_variate(
	uniform_rng uni_rng,
	uint32_t* seed_arg)
{
    uint32_t U = uni_rng(seed_arg);
    return norminv_urt(U);
}

typedef unsigned long fract ulf_t;	// DKF type introduced to make Eclipse moan less

//! \brief A poisson distributed random variable, given exp (-lambda). This is
//! Knuth's method, and is \f$O(n)\f$, so don't use for large lambda
//! \param[in] uni_rng A uniformly distributed random number generator.
//! \param[in] seed_arg The seed state for the random number sequence.
//! \param[in] lambda The parameter \f$\lambda\f$ of the distribution.
//! \return An unsigned integer which is poisson-distributed.
uint32_t poisson_dist_variate(
	uniform_rng uni_rng,
	uint32_t* seed_arg,
	accum lambda)
{
    ulf_t exp_minus_lambda = (ulf_t) expk(-lambda);

    //! \remark DRL thinks the above cast (from s16.15 to u0.32) might be wasteful.

    return poisson_dist_variate_exp_minus_lambda(uni_rng, seed_arg, exp_minus_lambda);
}

//! \brief A poisson distributed random variable, given exp (-lambda).
//! \param[in] uni_rng A uniformly distributed random number generator.
//! \param[in] seed_arg The seed state for the random number sequence.
//! \param[in] exp_minus_lambda \f$\exp(-\lambda)\f$.
//! \return An unsigned integer which is poisson-distributed.
uint32_t poisson_dist_variate_exp_minus_lambda(
	uniform_rng uni_rng,
	uint32_t* seed_arg,
	ulf_t exp_minus_lambda)
{
    ulf_t p = 1.0ulr;
    uint32_t k = 0;

    //! \remark DRL thinks this can be simplified to the following code:

    //!     while (p > exp_minus_lambda) {
    //!         p *= ulrbits(uni_rng(seed_arg));
    //!         k++;
    //!     }
    //!
    //!     return k;

    do {
	k++;
	p = p * ulrbits(uni_rng(seed_arg));
    } while (p > exp_minus_lambda);
    return k - 1;
}
