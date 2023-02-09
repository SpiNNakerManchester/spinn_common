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
