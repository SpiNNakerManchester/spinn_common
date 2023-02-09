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
