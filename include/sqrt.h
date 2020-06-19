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

/*! \file
 *
 *  \brief Implementation of square root for the ::s1615 type, returning a
 *  ::s1615 result.
 *
 *  \details The details of the algorithm are from
 *     <em>"Elementary Functions: Algorithms and Implementation",</em> 2nd edn,
 *     Jean-Michel Muller, Birkhauser, 2006.
 *
 *  \author
 *    Dave Lester (david.r.lester@manchester.ac.uk)
 *
 *  \copyright
 *    &copy; Dave Lester, Jamie Knight and The University of Manchester, 2014.
 *    All rights reserved.
 *    SpiNNaker Project
 *    Advanced Processor Technologies Group
 *    School of Computer Science
 *    The University of Manchester
 *    Manchester M13 9PL, UK
 *
 *  \date 23 October, 2014
 *
 */

#ifndef __SQRT_H__
#define __SQRT_H__

#include <stdfix-full-iso.h>

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED extern __attribute__((deprecated("Not implemented")))
#endif

//! \brief Square root.
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
extern s1615                sqrtk(s1615 x);

/*
 * All these functions have no implementation in this library. They will
 * generate a compile-time warning if used, and a link-time error.
 */

//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED s07           sqrthr(s07 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED s015          sqrtr(s015 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED s031          sqrtlr(s031 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED s87           sqrthk(s87 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED s3231         sqrtlk(s3231 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u08           sqrtuhr(u08 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u016          sqrtur(u016 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u032          sqrtulr(u032 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u88           sqrtuhk(u88 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u1616         sqrtuk(u1616 x);
//! \brief Square root.
//! \warning Unimplemented
//! \param[in] x: Non-negative value to get the square root of
//! \return the square root of \p x
UNIMPLEMENTED u3232         sqrtulk(u3232 x);

//! \brief Square root.
//! \param[in] f: Non-negative value to get the square root of
//! \return the square root of \p f
//! \todo Test that this actually works!
#define sqrtfx(f) \
    _Generic((f), \
	s07:   sqrthr(f), \
	s015:  sqrtr(f), \
	s031:  sqrtlr(f), \
	s87:   sqrthk(f), \
	s1615: sqrtk(f), \
	s3231: sqrtlk(f), \
	u08:   sqrtuhr(f), \
	u016:  sqrtur(f), \
	u032:  sqrtulr(f), \
	u88:   sqrtuhk(f), \
	u1616: sqrtuk(f), \
	u3232: sqrtulk(f), \
	default: __builtin_trap() \
    )

#endif /*__SQRT_H__*/
