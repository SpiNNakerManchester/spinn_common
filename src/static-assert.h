/*! \file
 *
 *  \brief This file provides a static assertion facility.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk),
 *    Jamie Knight (knightj@cs.man.ac.uk)
 *
 *  \copyright
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 12 December, 2013
 *
 *  \details
 *    Created on       : 12 December 2013
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 *
 */

#ifndef __STATIC_ASSERT_H__
#define __STATIC_ASSERT_H__


#ifdef __GNUC__

//! \brief This macro gives access to GCC's built-in static assertion.

#define static_assert(predicate, msg) _Static_assert (predicate, "msg")
#else

#define COMPILE_TIME_ASSERT(predicate, file)                            \
    _impl_CASSERT_LINE(predicate,__LINE__,file)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file)                       \
  typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#define static_assert(predicate, msg) COMPILE_TIME_ASSERT (predicate, msg)
#endif

#endif  /*__STATIC_ASSERT_H__*/
