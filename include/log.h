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
//! \brief Logarithm functions for fixed point

#ifndef __LOG_H__
#define __LOG_H__

#include "stdfix-full-iso.h"
#include "arm_acle.h"

//! \brief Logarithm: `log(x)` for the `accum` type (s16.15).
//! \param[in] x: Value to compute the logarithm of as an `accum`
//! \return The logarithm of \p x in `accum` format
s1615 logk(s1615 x);

#endif /*__LOG_H__*/
