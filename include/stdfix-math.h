/*! \file
 *
 *  \brief API for fix point transcendental functions
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

#ifndef __STDFIX_MATH_H__
#define __STDFIX_MATH_H__

//#include <stdint.h>
#include "stdfix-full-iso.h"

// constants
#define __PI_K  (kbits (102944))
#define __LN2_K (kbits ( 22713))
#define __E_K   (kbits ( 89073))

#define __EXP_UNIMPLEMENTED  (-1)
#define __LOG_UNIMPLEMENTED  (-2)
#define __SIN_UNIMPLEMENTED  (-3)
#define __COS_UNIMPLEMENTED  (-4)
#define __SQRT_UNIMPLEMENTED (-5)

void stdfix_error (int);

accum expk  (accum x);
accum logk  (accum x);
accum sink  (accum x);
accum cosk  (accum x);
accum sqrtk (accum x);

#define expfx(f)						      \
    ({__typeof__ (f) tmp = (f);                                       \
      if      (__builtin_types_compatible_p (__typeof__(f), accum))   \
          tmp = expk (f);                                             \
      else                                                            \
	  stdfix_error (__EXP_UNIMPLEMENTED);                         \
      tmp;							      \
    })

#define logfx(f)						      \
    ({__typeof__ (f) tmp = (f);                                       \
      if      (__builtin_types_compatible_p (__typeof__(f), accum))   \
          tmp = logk (f);                                             \
      else                                                            \
	  stdfix_error (__LOG_UNIMPLEMENTED);                         \
      tmp;							      \
    })

#define sinfx(f)						      \
    ({__typeof__ (f) tmp = (f);                                       \
      if      (__builtin_types_compatible_p (__typeof__(f), accum))   \
          tmp = sink (f);                                             \
      else                                                            \
	  stdfix_error (__SIN_UNIMPLEMENTED);                         \
      tmp;							      \
    })

#define cosfx(f)						      \
    ({__typeof__ (f) tmp = (f);                                       \
      if      (__builtin_types_compatible_p (__typeof__(f), accum))   \
          tmp = cosk (f);                                             \
      else                                                            \
	  stdfix_error (__COS_UNIMPLEMENTED);                         \
      tmp;							      \
    })

#define sqrtfx(f)						      \
    ({__typeof__ (f) tmp = (f);                                       \
      if      (__builtin_types_compatible_p (__typeof__(f), accum))   \
          tmp = sqrtk (f);                                            \
      else                                                            \
	  stdfix_error (__SQRT_UNIMPLEMENTED);                        \
      tmp;							      \
    })

/* Use the following when _all_ types have an implementation.

#define sqrtfx(f)						      \
    ({								      \
	__typeof__ (f) tmp = (f);				      \
	if      (__builtin_types_compatible_p (__typeof__(f), s07))   \
	    tmp = sqrthr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s015))  \
	    tmp = sqrtr (f, n);                                       \
	else if (__builtin_types_compatible_p (__typeof__(f), s031))  \
	    tmp = sqrtlr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s87))   \
	    tmp = sqrthk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), s1615)) \
	    tmp = sqrtk (f, n);                                       \
	else if (__builtin_types_compatible_p (__typeof__(f), s3231)) \
	    tmp = sqrtlk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u08))   \
	    tmp = sqrtuhr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u016))  \
	    tmp = sqrtur (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u032))  \
	    tmp = sqrtulr (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u88))   \
	    tmp = sqrtuhk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u1616)) \
	    tmp = sqrtuk (f, n);				      \
	else if (__builtin_types_compatible_p (__typeof__(f), u3232)) \
	    tmp = sqrtulk (f, n);				      \
	else							      \
	    abort ();						      \
	tmp;							      \
    })
*/

#endif /*__STDFIX_MATH_H__*/
