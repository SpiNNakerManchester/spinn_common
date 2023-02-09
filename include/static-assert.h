/*
 * Copyright (c) 2013-2023 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#define static_assert(predicate, msg) \
    _Static_assert(predicate, "msg")

#else
#define COMPILE_TIME_ASSERT(predicate, file) \
    _impl_CASSERT_LINE(predicate, __LINE__, file)
#define _impl_PASTE(a, b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
  typedef char _impl_PASTE(assertion_failed_##file##_, line)[2*!!(predicate)-1];
#define static_assert(predicate, msg) \
	COMPILE_TIME_ASSERT(predicate, msg)
#endif

#endif  /*__STATIC_ASSERT_H__*/
