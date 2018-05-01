/*! \file generic-runge-kutta.h
 * \author David R Lester
 * \date 12/8/2017
 * &copy 2017 The University of Manchester and David R Lester.
 *
 * This short file is the header file for a generic fourth-order Runge-Kutta algorithm
 * to solve initial value problems (IVP) for systems of first-order ODEs.
 * For details (\see https://en.wikipedia.org/wiki/Runge-Kutta_methods).
 */

#ifndef __GENERIC_RUNGE_KUTTA4
#define __GENERIC_RUNGE_KUTTA4

#include <stdint.h>

//! The algorithm will work with any suitable type for the real numbers
//! Just compile with the compile flag -DREAL_TYPE=double, for example.
//! Note it does not yet work with accum or int types.

#ifndef REAL_TYPE
#define REAL_TYPE float
#endif

#ifndef __REAL_T_DEFINED
#define __REAL_T_DEFINED
typedef REAL_TYPE real_t;
#endif /*__REAL_T_DEFINED*/


typedef uint32_t dim_t;
typedef real_t*  state_t; //! \brief The type of the state vector for ODE system

//! \brief Converts an integer number to a real (whatever that real type might be)
//!        Note: Defined as a macro to permit use in static const declarations.
//!
//! \param[in] n Integer

#define n2r(n) ((real_t)(n))

//! \brief Converts a rational number to a real (whatever that real type might be)
//!        Note: Defined as a macro to permit use in static const declarations.
//!
//! \param[in] n Numerator of the rational number
//! \param[in] d Denominator of the rational number

#define q2r(n,d) (n2r((n))/n2r((d)))

//! \typepdef fun_t
//! \brief This is the type of a C function pointer used by the generic
//! fourth-order Runga-Kutta algorithm.
//!
//! The function pointed to has an output argument which is a vector of
//! changes, and three input arguments: time, a state vector of variables of
//! the ODE system, and n, the dimension of the state vector.

typedef void (*fun_t)(state_t, real_t, state_t);

unsigned int* odeint_statistics (void);
void odeint (real_t y[], dim_t n, real_t x1, real_t x2, real_t eps, real_t h1, real_t hmin, fun_t derivs);


#endif /*__GENERIC_RUNGE_KUTTA4*/
