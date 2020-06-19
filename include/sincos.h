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
//! \brief Trigonometric functions for fixed point

#ifndef __SINCOS_H__
#define __SINCOS_H__

#include <stdfix-full-iso.h>

//! \brief This function calculates the sin function for the ::s1615 type.
//! \param[in] x: is positive value represented as an ::s1615.
//! \return A value representing sin(x) in ::s1615 format.
s1615    sink(s1615 x);
//! \brief This function calculates the cos function for the ::s1615 type.
//! \param[in] x: is positive value represented as an ::s1615.
//! \return A value representing cos(x) in ::s1615 format.
s1615    cosk(s1615 x);

#endif /*__SINCOS_H__*/
