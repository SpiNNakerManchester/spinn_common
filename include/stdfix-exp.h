/*
 * Copyright (c) 2014 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//! \file
//! \brief Exponential functions for fixed point
//! \author Dave Lester (david.r.lester@manchester.ac.uk)
//! \copyright
//!    &copy; Dave Lester and The University of Manchester, 2013.
//!    All rights reserved. <br>
//!    SpiNNaker Project <br>
//!    Advanced Processor Technologies Group <br>
//!    School of Computer Science <br>
//!    The University of Manchester <br>
//!    Manchester M13 9PL, UK
//! \date 20 February, 2014

/*
 * stdfix-exp.h
 *
 *
 *  SUMMARY
 *    The API file for a non-garbage-collected memory manager.
 *
 *  AUTHOR
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  COPYRIGHT
 *    Copyright (c) Dave Lester and The University of Manchester, 2013.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  DESCRIPTION
 *
 *  CREATION DATE
 *    20 February, 2014
 *
 *  HISTORY
 * *  DETAILS
 *    Created on       : 20 February 2014
 *    Version          : $Revision$
 *    Last modified on : $Date$
 *    Last modified by : $Author$
 *    $Id$
 *
 *    $Log$
 */

#ifndef __STDFIX_EXP_H__
#define __STDFIX_EXP_H__

#include <stdint.h>
#include <stdfix.h>
#include <stdbool.h>

#include <stdfix-full-iso.h>

// stdfix-exp API

//! \brief Fixed point exponential
//! \param[in] x: Value to compute the exponential of.
//! \return The best approximation to e<sup>\p x</sup> possible in an ::s1615
s1615 expk(s1615 x);

//! \brief Fixed point exponential
//! \param[in] x: Value to compute the exponential of.
//! \return The best approximation to e<sup>\p x</sup> possible in a ::u1616
u1616 expuk(s1615 x);

//! \brief Fixed point exponential
//! \param[in] x: Value to compute the exponential of.
//! \return The best approximation to e<sup>\p x</sup> possible in a ::u032
u032 expulr(s1615 x);

#endif /*__STDFIX_EXP_H__*/
