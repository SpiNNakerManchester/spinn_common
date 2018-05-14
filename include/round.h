#include <stdfix-full-iso.h>
#include <random.h>

#define MULT_ROUND_NEAREST(x, y) (kbits(__stdfix_smul_k_round_nearest(bitsk(x), bitsk(y))))
#define MULT_ROUND_STOCHASTIC(x, y) (kbits(__stdfix_smul_k_round_stochastic(bitsk(x), bitsk(y))))

//! \brief This function stochastically rounds the input signed 64-bit integer
//! to a number
//! of bits, returning a 64-bit integer.
//! \param[in] f A 64-bit number to be rounded.
//! \param[in] n An int.
//! \return The f rounded to the nearest n bits stochastically.

static inline int64_t __stdfix_stochastic_round_s64(
	int64_t x,
	int n)
{
    register int64_t r;
    register uint64_t p, dropped_bits;

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
