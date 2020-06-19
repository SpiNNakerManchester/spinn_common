/*
 * Copyright (c) 2013-2019 The University of Manchester
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//! \file
//! \brief Support code for random.h

#include <stdint.h>
#include <stdbool.h>
#include <stdfix.h>
#include "stdfix-full-iso.h"
#include "polynomial.h"

#ifndef NO_INLINE
#define NO_INLINE	__attribute__((noinline))
#endif

//! Precalculated extreme values of the normal distribution
static int32_t low_values[] = {
      229376,  // 7.0
      136628,  // 4.1695693238150016769941586303372689400327
      131359,  // 4.0087725930708411881818210320355889243017
      128187,  // 3.9119529919160001670722249101622708468998
      125892,  // 3.8419306829712283133151316098040051639911
      124086,  // 3.7868194449983402763131211397648554569241
      122593,  // 3.7412524605583848106538418311541645451693
      121318,  // 3.7023364718901400753335901075251073966233
      120204,  // 3.6683292815484261480128929871057879082975
      119213,  // 3.6380986692639864274074160136736633407040
      118321,  // 3.6108664483212513474073228506484208624655
      117508,  // 3.5860742890225079617751212517656408955687
      116762,  // 3.5633077356190277342193712560442849122243
      116072,  // 3.5422505056364735446460423573544199799560
      115430,  // 3.5226556381533075502778727820276468013065
      114830,  // 3.5043265382664131964066103970044783050030
      114265,  // 3.4871041001974256712642520295201949257208
      113733,  // 3.4708577075316797916101920842325453909628
      113229,  // 3.4554787872817792620362878463145179720772
      112751,  // 3.4408760935809452410288929357867439285194
      112295,  // 3.4269721916155209090919539778465177340721
      111860,  // 3.4137007924643318270572205067903129890920
      111444,  // 3.4010047027689763364020768462526011491044
      111045,  // 3.3888342262761816502950161391910712653167
      110662,  // 3.3771459026058869717750272167491982817867
      110294,  // 3.3659015011939826219631961885050176670591
      109939,  // 3.3550672107677343607425691753990264468280
      109596,  // 3.3446129803835481959929309704285727577399
      109265,  // 3.3345119791886319455039632784164305963852
      108945,  // 3.3247401500889482748951156834115456732761
      108635,  // 3.3152758383613645595159532094185121136837
      108334,  // 3.3060994805746625271460790219577049724580
      108042   // 3.2971933424171863645116941846338065473089
};

//! Offset terms for central_approx()
static int offset[6] = {
    7200, 6272, 4608, 2048, 0, 0
};
//! Multiplication factors for central_approx()
static int multiplier[6] = {
    32, 16, 8, 6, 6, 0
};

//! Leading terms of the distribution for lo_approx()
static int leading_terms[11] = {
    98304, 98304, 65536, 65536, 65536, 16, 14, 13, 11, 10, 0
};

/*static int new_poly [5]
  = {*/

//! Patchwise approximation polynomial terms
static int polynomials[11][5] = {
      {   489693536,  //  0.22803132236003875732421875
         -966086639,  // -0.4498691479675471782684326171875
         1048652819,  //  0.4883170216344296932220458984375
        -1202555181,  // -0.5599833936430513858795166015625
          638185482}, //  0.297178273089230060577392578125   +3.0
      {   510971994,  //  0.237939876504242420196533203125
        -1008958387,  // -0.4698328613303601741790771484375
         1097593202,  //  0.511106663383543491363525390625
        -1269255408,  // -0.591043107211589813232421875
          208850025}, //  0.0972533714957535266876220703125  +3.0
      {   535062308,  //  0.24915780313313007354736328125
        -1057614622,  // -0.492490186356008052825927734375
         1153468280,  //  0.5371255241334438323974609375
        -1347876992,  // -0.627654135227203369140625
         1901851228}, //  0.88561849109828472137451171875    +2.0
      {   562587223,  //  0.2619750904850661754608154296875
        -1113360216,  // -0.5184487514197826385498046875
         1217886707,  //  0.5671226917766034603118896484375
        -1442344496,  // -0.671643994748592376708984375
         1417447081}, //  0.6600502324290573596954345703125  +2.0
      {   594357355,  //  0.2767692110501229763031005859375
        -1177895230,  // -0.548500209115445613861083984375
         1292890733,  //  0.6020491630770266056060791015625
        -1558625637,  // -0.7257916205562651157379150390625
          896662126}, //  0.417540839873254299163818359375   +2.0
      {  1364524482,  //
            4157352,  //
          893590920,  //
          482307216,  //                                      +0.5
          -56131990}, //                                      +4.0
      {  1594011413,  //
           34197350,  //
          980366094,  //
          540913971,  //                                      +0.5
           14157698}, //                                      +3.5
      {  2059946197,  //
          130876674,  //
         1126841309,  //
          628910107,  //                                      +0.5
         -391651081}, //                                      +3.25
      {   669410721,  //
          151375990,  //
          657881394,  //
          208981697,  //                                      +0.5
         -111730420}, //                                      +2.75
      {   135763632,  //
          134215538,  //
          347443147,  //
         -134393924,  //                                      +0.5
           14235314}, //                                      +2.5
      {           0,  //
                  0,  //
                  0,  //
                  0,  //
                  0}  //
};

//! \brief Access to the tail approximation polynomial table evaluation
//! \param[in] approx: The number of the approximation
//!     (this selects the polynomial patch).
//! \param[in] x: The point at which the function is approximated.
//! \return The corrected value of the polynomial at \p x
static int lo_approx(
	unsigned int approx,
	int x)
{
    int r;

    assert(approx <= 4);
    assert(0 <= x);

    r = __horner_int_b(polynomials[approx], x, 4);
    r = __stdfix_round_s32(r, 15);
    r = leading_terms[approx] + (r >> 16);
    return r;
}

//! \brief Access to the central approximation polynomial table evaluation
//! \param[in] approx: The number of the approximation
//!     (this selects the polynomial patch).
//! \param[in] x: The point at which the function is approximated.
//! \return The corrected value of the polynomial at \p x
static int mid_approx(
	unsigned int approx,
	int x)
{
    int r;

    assert(approx <= 5);
    assert(0 <= x);

    r = __horner_int_b(polynomials[approx + 5], x, 4);
    r += x << 14; // DRL Mod for overflow prevention
    return r;
}

#ifndef abs
//! Absolute value
#define abs(x)		(((x) < 0) ? -(x) : (x))
#endif

#ifndef negative
//! Is-negative test
#define negative(x)	((x) < 0)
#endif

//! \brief Access to the tail approximation functions
//! \param[in] approx: The number of the approximation
//! \param[in] p: The point at which the function is approximated.
//! \return Normal value (using algorithm relevant for tail regions)
static int tail_approx(
	unsigned int approx,
	int p)
{
    int r;

    assert(32 <= p && p < 2048);

    p = p - (1 << (14 - approx));
    p = p << (approx + 1);
    r = lo_approx(9 - approx, p);

    return r;
}

//! \brief Access to the central approximation functions
//! \param[in] approx: The number of the approximation.
//! \param[in] p: The point at which the function is approximated.
//! \return Normal value (using algorithm relevant for central region)
static int central_approx(
	unsigned int approx,
	int p)
{
    int r, z;

    assert(0 <= p && p <= INT16_MAX);

    z = p;
    z = __smulbb(z, z) >> 17;		// DRL HACK rounding???

    z -= offset[5 - approx];
    z *= multiplier[5 - approx];
    z <<= 1;				//DRL HACK!!

    assert(0 <= z);

    r = mid_approx(5 - approx, z);
    r = r >> 16;

    int tmp = leading_terms[10 - approx];

    r += tmp << 13;
    r = (int) __smulwb(r, p);		// DRL HACK rounding???
    return r;
}

int NO_INLINE __norminv_rbits(
	int x)
{
    int neg = negative(x);
    int r, shift;

    assert(INT16_MIN <= x && x <= INT16_MAX);

    if (x - INT16_MIN <= 31) {
        r = low_values[x - INT16_MIN];
    } else if (INT16_MAX - x + 1 <= 31) {
        r = low_values[INT16_MAX - x + 1];
    } else {
        if (x < 0) {
            x = -x;
        }

        assert(0 <= x && x <= INT16_MAX - 31);

        shift = __builtin_clz((INT16_MAX + 1) - x) - 17;

        if (shift > 4) {
            r = tail_approx(shift, (INT16_MAX + 1) - x);
        } else {
            r = central_approx(shift + 1, x);
        }
    }

    if (neg) {
        r = -r;
    }
    return r;
}
