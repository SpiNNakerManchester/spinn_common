/*
    This file complements stdfix-full-iso.h to add various rounding routines
    and operations with rounding.

    Note that GCC currenly does not implement rounding on such operations like
    fixed-point multiplication.

    Author: mantas.mikaitis@manchester.ac.uk, 2018 May

 */

#include <stdfix-full-iso.h>
#include <random.h>

// Helper macros to work with accums easily
#define MULT_NO_ROUND_GCC(x, y) (x*y)
#define MULT_NO_ROUND_CUSTOM(x,y) (kbits(__stdfix_smul_k(bitsk(x), bitsk(y))))
#define MULT_ROUND_NEAREST(x, y) \
    (kbits(__stdfix_smul_k_round_nearest(bitsk(x), bitsk(y))))
#define MULT_ROUND_STOCHASTIC(x, y) \
    (kbits(__stdfix_smul_k_round_stochastic(bitsk(x), bitsk(y))))

#define STOCHASTIC_ROUND_S32(x, n) \
    (__stdfix_stochastic_round_s32(bitsk(x), n))

#define STOCHASTIC_ROUND_S64(x, n) \
    (__stdfix_stochastic_round_s64(bitslk(x), n))

static inline int64_t __stdfix_stochastic_round_s64(
	int64_t x,
	int n)
{
    register int64_t r;
    register uint64_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss64_simp() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline int32_t __stdfix_stochastic_round_s32(
	int32_t x,
	int n)
{
    register int32_t r;
    register uint32_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss64_simp() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline int16_t __stdfix_stochastic_round_s16(
	int16_t x,
	int n)
{
    register int16_t r;
    register uint16_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss32() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline uint64_t __stdfix_stochastic_round_u64(
	uint64_t x,
	int n)
{
    register uint64_t r;
    register uint64_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss64_simp() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline uint32_t __stdfix_stochastic_round_u32(
	uint32_t x,
	int n)
{
    register uint32_t r;
    register uint32_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss32() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline uint16_t __stdfix_stochastic_round_u16(
	uint16_t x,
	int n)
{
    register uint16_t r;
    register uint16_t p, dropped_bits;

    r = x >> n;
    p = mars_kiss32() & ((0x1 << n) - 1);
    dropped_bits = x & ((0x1 << n) - 1);

    if (p <= dropped_bits)
        return ((r + 0x1) << n);
    else return (r << n);
}

static inline int32_t __stdfix_smul_k_round_stochastic(
    int32_t x,
    int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
        return INT32_MAX;
    }

    return __stdfix_sat_k(
        __stdfix_stochastic_round_s64((__I64(x) * __I64(y)), 15) >> 15);
}

static inline int32_t __stdfix_smul_k_round_nearest(
    int32_t x,
    int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
        return INT32_MAX;
    }

    return __stdfix_sat_k(__stdfix_round_s64((__I64(x) * __I64(y)), 15) >> 15);
}
