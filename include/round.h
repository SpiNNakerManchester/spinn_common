#include <stdfix-full-iso.h>
#include <random.h>

#define MULT_ROUND_NEAREST(x, y) (kbits(__stdfix_smul_k_round_nearest(bitsk(x), bitsk(y))))
#define MULT_ROUND_STOCHASTIC(x, y) (kbits(__stdfix_smul_k_round_stochastic(bitsk(x), bitsk(y))))

static inline int32_t __stdfix_smul_k_round_stochastic(
    int32_t x,
    int32_t y)
{
    if (x == INT32_MIN && y == INT32_MIN) { // special case for -1.0*-1.0
        return INT32_MAX;
    }

    int64_t result = __I64(x) * __I64(y);

    uint32_t p = mars_kiss32() & 0x7FFF;
    uint32_t dropped_bits = result & 0x7FFF;

    if (p <= dropped_bits)
        return __stdfix_sat_k((__I64(x) * __I64(y)) >> 15) + 0x1;
    else return __stdfix_sat_k((__I64(x) * __I64(y)) >> 15);
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
