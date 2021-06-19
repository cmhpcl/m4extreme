// LinLib.h: interface for the Element library.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(SOLVER_LINEAR_LINLIB_H__INCLUDED_)
#define SOLVER_LINEAR_LINLIB_H__INCLUDED_

#pragma once

#include "./Linear.h"
#include "./Cholesky/Cholesky.h"

#if defined(_M4EXTREME_MPI_)
#include "./SuperLU/SuperLUMPI.h"
#else
#include "./SuperLU/SuperLU.h"
#endif

#endif // !defined(SOLVER_LINEAR_LINLIB_H__INCLUDED_)
