/* Runge-Kutta-4-5 (with Cash-Karp Butcher Table)
 *
 * Author: David Lester
 * Date 25/1/2018
 *
 * This is a modified version of Runge-Kutta 4-5 with Cash-Karp Butcher Table from Numerical Recipes.
 *
 * The key differences are: I have tried to make it independent of the type of reals used.
 * And I have taken out the heap allocation and deallocation so that it all takes place on the stack instead.
 *
 * Reference: http://www.aip.de/groups/soe/local/numres/bookcpdf/c16-2.pdf
 *
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING (ISBN 0-521-43108-5)
 * Copyright (C) 1986-1992 by Cambridge University Press.
 *
 * The general form of a fifth-order Runge-Kutta formula is (Eqn 16.2.4):
 *
 *       k1 = h f(x_n,y_n)
 *       k2 = h f(x_n + a_2h, y_n + b_21k_1)
 *              ···
 *       k6 = h f(x_n + a_6h, y_n + b_61k_1 + · · · + b_65k_5)
 *
 *    y_n+1 = y_n +c_1k_1 +c_2k_2 +c_3k_3 +c_4k_4 +c_5k_5 +c_6k_6 +O(h^6)
 *
 * The embedded fourth-order formula is (Eqn 16.2.5):
 *
 *    y^∗_n+1 = y_n +c^∗_1k_1 +c^∗_2k_2 +c^∗_3k_3 +c^∗_4k_4 +c^∗_5k_5 +c^∗_6k_6 +O(h^5)
 *
 * The error estimate is then (Eqn 16.2.6):
 *
 *    \Delta = y_n+1 - y^*_n+1
 *
 * There are three major routines in this file:
 *
 *      runge_kutta_ks (combined with the two extra functions below, constitutes rkck in the reference):
 *                      Calculates the K-vectors as shown above.
 *                      The calculations 16.2.5,6 have been split from the function rkck in the reference, since
 *                      we use the error calculation more often to correctly size h before calling the step function
 *                      for 16.2.5.
 *
 *        - runge_kutta_error: Calculation of Eqn 16.2.6
 *        - runge_kutta_step:  Calculation of Eqn 16.2.5
 *
 *      select_step_size_and_step (was rkqs in reference): Selects a step-size, and then makes a step.
 *                                Uses two heuristic routines to grow and shrink the step size:
 *
 *        - grow_step_size
 *        - shrink_step_size
 *
 *                                As heuristics these functions can be changed to suit the task in hand.
 *
 *      odeint                    This is the public API for the Runge-Kutta ODE solver. Note it is
 *                                different to that provided in the reference because I've moved the reporting
 *                                over to the user.
 */

#include "generic-runge-kutta-impl.h"

/* Butcher Table for Cash-Karp is then:
 *
 * +---+------+----------------------------------------------------------+----------+-------------+
 * | i | a_i  |                          b_ij                            | c_i      | c_i^*       |
 * +---+------+----------------------------------------------------------+----------+-------------+
 * | 1 |      |                                                          |  37/378  |  2825/27648 |
 * | 2 | 1/5  |     1/5                                                  |    0     |      0      |
 * | 3 | 3/10 |     3/40       9/40                                      | 250/621  | 18575/48384 |
 * | 4 | 3/5  |     3/10      -9/10     6/5                              | 125/594  | 13525/55296 |
 * | 5 | 1    |   -11/54       5/2    -70/27        35/27                |    0     |   277/14336 |
 * | 6 | 7/8  |  1631/55296  175/512  575/13824  44275/110592  253/4096  | 512/1771 |     1/4     |
 * +---+------+----------------------------------------------------------+----------+-------------+
 *
 * To compress this table, and to ensure it's not re-initialised everytime the
 * function is called, we use macros to index into the table. The use of
 * static ensures we've informed the compiler that these tables remain constant.
 *
 * Note the missing elements in the c and c^* vectors. We have renamed c^* as d.
 *
 * We are using the macro q2r(n,d) to accurately represent the real number (n/d)
 * (of whatever type that actually is).
 */

static const real_t a[]  = {
	q2r(1, 5), q2r(3, 10), q2r(3, 5), q2r(1, 1), q2r(7, 8)};
static const real_t b[]  = {
	q2r(1, 5),
	q2r(3, 40),       q2r(9, 40),
	q2r(3, 10),       q2r(-9, 10),   q2r(6, 5),
	q2r(-11, 54),     q2r(5, 2),     q2r(-70, 27),    q2r(35, 27),
	q2r(1631, 55296), q2r(175, 512), q2r(575, 13824), q2r(44275, 110592), q2r(253, 4096)};

static const real_t c[] = {
	q2r(37, 378), q2r(250, 621), q2r(125, 594), q2r(0, 1), q2r(512, 1771)};
static const real_t d[] = {
	q2r(37, 378)  - q2r(2825, 27648),
	q2r(250, 621) - q2r(18575, 48384),
	q2r(125, 594) - q2r(13525, 55296),
	q2r(-277, 14336),
	q2r(512, 1771) - q2r(1, 4)};

//! The following variables are used to gather statistics about the quality of the solver
//! and its execution. All are initialised to zero. Probably need a reset function.
//! External interface to these values to be determined by Donal/Oliver/Andrew(s).

#define RKCK_UNDERFLOW          0
#define RKCK_SIZE_GUESSED_RIGHT 1
#define RKCK_SIZE_GUESSED_WRONG 2
#define RKCK_STEPS_EXCEEDED     3
#define RKCK_STEP_TOO_SMALL     4

static unsigned int rkck_statistics[] = {0, 0, 0, 0, 0};

unsigned int* odeint_statistics(void)
{
    return rkck_statistics;
}

#define RKN 6                        /* Dimension of the RK-CK system              */
#define index_b(i, j) (((i)==2)? 0: (((i)-3)*((i)-3)+1 + ((j)-1)))
#define A(i)          (a[i-2])
#define B(i, j)       (b[index_b((i), (j))])
#define K(i)          (ks+(n*(i-1))) /* Get the start address of the i-th K-vector */

//! \brief Tests for vector equality
//!
//! \param[in] x A vector of reals.
//! \param[in] y A vector of reals.
//! \param[in] n Dimension of the vectors.
//! \return      Returns true if the two vectors have pairwise equal elements

static inline bool vector_eq(
	real_t* x, real_t* y, unsigned int n)
{
    unsigned int i;
    bool r = true;

    for (i = 0; i < n; i++) {
        r &= (x[i] == y[i]);
    }

    return r;
}

//! \brief Tests that the k1 vector is equal to the instantaneous derivatives at y,x.
//!        Note: used only in the assertion in runge_kutta_ks.
//!
//! \param[in] k1     A vector of elements
//! \param[in] y      The current ODE state vector
//! \param[in] x      The current independent variable (time)
//! \param[in] n      The dimension of the vectors.
//! \param[in] derivs The function describing the ODE system.

static inline bool K1_is_derivs(
	real_t* k1, real_t* y, real_t x, unsigned int n, fun_t derivs)
{
    real_t dydx[n];

    derivs(dydx, x, y);

    return vector_eq(k1, dydx, n);
}

//! \brief Construct the K-vectors for a given starting position (y,x), and time-step h.
//!
//! \param     ks     The constructed K-vectors (excluding the first one which has been pre-computed).
//! \param[in] y      The ODE state vector
//! \param[in] x      The current time
//! \param[in] n      The dimension of the ODE system.
//! \param[in] h      The time step
//! \param[in] derivs The functional description of the ODE system.

static void runge_kutta_ks(
	real_t* ks, real_t* y, real_t x, unsigned int n, real_t h, fun_t derivs)
{
    unsigned int i, j, k;
    real_t ytemp[n];

    assert(n > 0);                                 // Assert that the vectors are non-empty
    assert(K1_is_derivs(K(1), y, x, n, derivs));   // Assert that on entry to the routine, K(1) is already
                                                   // initialised with the instantaneous derivatives at y,x.

    for (i = 2; i <= RKN; i++) {                   // Calculate the K(i) vector
	for (k = 0; k < n; k++) {                  // Do component-wise operations
	    ytemp[k] = B(i, 1) * K(1)[k];          // Initialise the accumulation
	    for (j = 2; j < i; j++) {              // Iterate over the vector being constructed
	        ytemp[k] += B(i, j) * K(j)[k];     // Addition of linear combinations of previous K(i)'s
	    }
	    ytemp[k] = y[k] + h*ytemp[k];          // We're calculating modified states for use
	}                                          // in the derivs calculation ...
	derivs(K(i), x + h*A(i), ytemp);           //  ... which is done here. This assigns a result to K(i).
    }
}

//! \brief Perform the matrix-vector multiply: b = h (ks a).
//!
//! \param[in]  a  A vector of dimension RKN-1, of coefficients
//! \param[out] b  The result vector of dimension n
//! \param[in]  ks An RKN x n matrix of reals
//! \param[in]  h  A scaling factor, which is in fact the integration step-size.
//! \param[in]  n  Dimension of ODE system.

static void matrix_vector(
	const real_t* a, real_t* b, real_t* ks, real_t h, unsigned int n)
{
    unsigned int i, j;

    assert(n > 0);

    for (j = 0; j < n; j++) {           // Iterate component-wise over the output vector
        b[j] = K(1)[j] * a[0];          // Initialise output vector, then skip an element ..
	for (i = 3; i <= RKN; i++) {    //  .. Iterating over the remaining terms of the RK system
	    b[j] +=  K(i)[j] * a[i-2];  // Accumulate component result.
	}
	b[j] *= h;                      // Scale result
    }
}

//! \brief A wrapper function to matrix_vector, to calculate the component-wise
//!        error of the RK-CK algorithm.
//!
//! \param[out] yerr  A vector of dimension n, of estimated errors
//! \param[in]  ks    An RKN x n matrix of reals
//! \param[in]  h     A scaling factor, which is in fact the integration step-size.
//! \param[in]  n     Dimension of ODE system.

static inline void runge_kutta_error(
	real_t* yerr, real_t* ks, real_t h, unsigned int n)
{
    matrix_vector(d, yerr, ks, h, n);
}


//! \brief A wrapper function to matrix_vector, to calculate the component-wise
//!        evolution of the RK-CK algorithm.
//!
//! \param[in]  y     A vector of dimension n, containing the current state variables.
//! \param[out] yout  A vector of dimension n, of modified state variables
//! \param[in]  ks    An RKN x n matrix of reals
//! \param[in]  h     A scaling factor, which is in fact the integration step-size.
//! \param[in]  n     Dimension of ODE system.

static inline void runge_kutta_step(
	real_t* yout, real_t* ks, real_t h, unsigned int n)
{
    matrix_vector(c, yout, ks, h, n);
}

//! \brief Calculates the maximum estimated error for each component of the ODE state vector.
//!
//! \param[in] yerr  A vector of dimension n, of the estimated errors for each component.
//! \param[in] yscal A vector of dimension n, of scaling factors for each component.
//! \param[in] n     Dimension of ODE system.
//! \return          The value of the component with the largest absolute value.

static inline real_t maximum_error(
	real_t* yerr, real_t* yscal, unsigned int n)
{
    real_t errmax = fabs(yerr[0] * yscal[0]);
    unsigned int i;

    assert(n > 0);

    for (i = 1; i < n; i++) {
        errmax = fmax(errmax, fabs(yerr[i] * yscal[i]));   // Scale relative to required tolerance.
    }

    return errmax;
}

//! \brief Copies the vector a to the vector b.
//!
//! \param[out] b Target vector
//! \param[in]  a Source vector
//! \param[in]  n Dimension of ODE system.

static inline void copy_vector(
	real_t* b, real_t* a, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++) {
        *b++ = *a++;
    }
}

#define SAFETY  q2r(9, 10)
#define PGROW   q2r(-2, 10)
#define PSHRNK  q2r(-25, 100)

//! \brief Given an error make a suggested scale-factor for the next step size
//!        to try. The maximum growth is a factor of 5.

static inline real_t grow_step_size(
	real_t err)
{
    return fmin(SAFETY * pow(err, PGROW), n2r(5));
}

//! \brief Given an error make a suggested scale-factor for the next step size
//!        to try. The minimum growth is a factor of 1/10.

static inline real_t shrink_step_size(
	real_t err)
{
    return fmax(SAFETY * pow(err, PSHRNK), q2r(1, 10));
}

//! \brief Checks that the step size does not underflow when added to the
//!        current start point of integration. Has the side-effect of
//!        incrementing the variable rkck_underflow, if an underflow is
//!        detected.
//!
//! \param[in]  x Current start point of integration
//! \param[in]  h step size
//! \return     Returns true if there has been an underflow; otherwise false.

static inline bool step_size_underflow(
	real_t x, real_t h)
{
    real_t xnew = x+h;
    bool   r    = (xnew == x);  // Check that the new step size is not so
                                // small as to be a rounding error.

    if (r) {
        rkck_statistics[RKCK_UNDERFLOW]++;
    }

    return r;
}

//! \brief Determines if the step size is now small enough to be acceptable; has the side-effect of
//!        reducing the size of the step, if it is currently too large.
//!
//! \param     hp  The address of the current step size; modified if unacceptable.
//! \param[in] err The current scaled error.
//! \param[in] x   Current start point of integration
//! \return        Returns true if the current step size is unacceptable; false otherwise.

static inline bool unacceptable_step_size(
	real_t* hp, real_t err, real_t x)
{
    real_t h_new = *hp;
    bool   r     = (err > n2r(1));

    if (r) {
        h_new = (*hp) * shrink_step_size(err);// Re-size the next step size to be smaller ..
        if (step_size_underflow(x, h_new)) {  //  ... and check for underflow.
	    return false;                     // There's no point going smaller as the step size is already underflowing
        }
	(*hp) = h_new;
    }

    return r;
}

//! \brief Fifth-order Runge-Kutta step with monitoring of local truncation
//!        error to ensure accuracy and adjust stepsize.
//!
//! \param[out] r      (Pointer to) Result structure
//! \param      y      Vector state
//! \param      x      Independent variable vector (time)
//! \param[in]  dydx   Vector of derivatives
//! \param[in]  n      Dimension of state vector
//! \param[in]  htry   Step size to be attempted.
//! \param[in]  eps    User-supplied required error
//! \param[in]  yscal  Vector against which to scale error
//! \param[in]  derivs The user-supplied computation of the righthand side derivatives

static inline void select_step_size_and_step(
	rkqs_t *r, real_t y[], real_t *xp, real_t ks[], dim_t n,
	real_t htry, real_t eps, real_t yscal[], fun_t derivs)
{
    real_t ytemp[n];
    real_t errmax = 0, h;
    unsigned int i;
    
    h = htry;                                 // Set stepsize to the initial trial value.

    do {
        runge_kutta_ks(ks, y, *xp, n, h, derivs); // Set up the K vectors.
	runge_kutta_error(ytemp, ks, h, n);   // Calculate the error of this attempt.
	errmax = maximum_error(ytemp, yscal, n) / eps; // Evaluate error, scale and convert error to a ratio
    } while (unacceptable_step_size(&h, errmax, *xp)); // Continue to reduce step size while the error is unacceptable.

    runge_kutta_step(ytemp, ks, h, n);        // Take a step
    
    r->hnext = h * grow_step_size(errmax);    // Set the next step size to be (potentially) larger
    r->hdid  = h;                             // Set the actual step size taken.
    *xp += h;                                 // Increment the starting point of the integration.

    for (i = 0; i < n; i++) {
        y[i] += ytemp[i];                     // Add the evolution values to the state variables.
    }
}


#define MAXSTP 100000
#define TINY \
    (q2r(1, 10000000000) * q2r(1, 10000000000) * q2r(1, 10000000000)) /* = 1.0e-30 */

static inline void step_size_heuristic_statistics(
	real_t h_actual, real_t h_chosen)
{
    if (h_actual == h_chosen) {
	rkck_statistics[RKCK_SIZE_GUESSED_RIGHT]++;
    } else {
	rkck_statistics[RKCK_SIZE_GUESSED_WRONG]++;
    }
}

static inline void next_step_size_check(
	real_t h_next, real_t h_min)
{
    if (fabs(h_next) <= h_min) {
	rkck_statistics[RKCK_STEP_TOO_SMALL]++;
    }
}

static inline void max_steps_exceeded(
	unsigned int n)
{
    if (n > MAXSTP) {
	rkck_statistics[RKCK_STEPS_EXCEEDED]++;
    }
}

//! \brief Scaling used to monitor accuracy. This general-purpose choice can be modified
//!        if need be.
//!
//! \param[out] yscal The resultant scale vector
//! \param[in]  y     The current values of the ODE state vector
//! \param[in]  dydx  The current instantaneous derivative values for the state variables.
//! \param[in]  h     The current step size
//! \param[in]  n     The dimension of each vector

static inline void scale_vector(
	real_t yscal[], real_t y[], real_t dydx[], real_t h, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++) {
        yscal[i] = n2r(1) / (fabs(y[i]) + fabs(dydx[i] * h) + TINY);
    }
}

//! \brief Checks that the two real_valued arguments both have the same (non-zero) sign.
//!
//! \param[in] a First real variable
//! \param[in] b Second real variable
//! \return    Returns true if (a > 0 && b > 0) or (a < 0 && b < 0); false otherwise

static inline bool same_sign(
	real_t a, real_t b)
{
    return a*b > q2r(0, 1);
}

//! \brief Runge-Kutta driver with adaptive stepsize control.
//!
//! \param      y      Initial values of the vector y, also holds the final result on successful completion.
//! \param[in]  n      Dimension of the ODE system.
//! \param[in]  x1     Initial value of the independent variable x
//! \param[in]  x2     Final value of the independent variable x
//! \param[in]  eps    Global accuracy
//! \param[in]  h1     User-guessed initial step size
//! \param[in]  hmin   Minimum allowed step size
//! \param[in]  derivs The user-supplied routine for calculating the right-hand side derivatives

void odeint(
	real_t y[], dim_t n, real_t x1, real_t x2, real_t eps, real_t h1,
	real_t hmin, fun_t derivs)
{
    real_t ks[RKN*n];                         // The K-vector structure
    real_t x = x1, h = SIGN(h1, x2-x1);       // Assign h: the magnitude of h1, and the sign of x2-x1
    real_t yscal[n];                          // A scaling vector used to scale the results during error analysis
    rkqs_t rs;                                // Results structure for the rkqs routine.
    dim_t  nstp = 1;                          // The number of individual integration steps actually taken

    assert(hmin > n2r(0));                    // The minimum step size is positive. Note the actual h might not be.

    do {
        derivs(K(1), x, y);                   // Initialise K-vectors with the instantaneous derivatives
	scale_vector(yscal, y, K(1), h, n);   // Initialise the yscal-vector
	if (same_sign(x+h-x2, x+h-x1)) {
	    h = x2-x;                         // If stepsize overshoots: decrease it.
	}

	select_step_size_and_step(&rs, y, &x, ks, n, h, eps, yscal, derivs);
	                                      // Take a single step of a suitable size (size to be determined by rkqs)
	step_size_heuristic_statistics(rs.hdid, h);
	                                      // Update the statistics counters associated with the step size heuristic
	next_step_size_check(rs.hnext, hmin);
	h = rs.hnext;
	                                      // Take at most MAXSTP steps, and also stop when x isn't in [x1,x2)
    } while ((++nstp <= MAXSTP) && ((x-x2) * (x2-x1) < n2r(0)));

    max_steps_exceeded(nstp);                 // Update statistics on too many steps taken

    printf("Underflows %u, right/wrong guess %u/%u, too many steps: %u, steps too small %u\n",
	    rkck_statistics[0], rkck_statistics[1], rkck_statistics[2], rkck_statistics[3],
	    rkck_statistics[4]);
}
