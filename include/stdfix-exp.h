/*
 * Copyright (c) 2014-2019 The University of Manchester
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

#endif /*__STDFIX_EXP_H__*/
