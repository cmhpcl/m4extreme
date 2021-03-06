// -*- C++ -*-

/*!
  \file numerical/specialFunctions/ErrorFunction.h
  \brief The error functions erf() and erfc().
*/

#if !defined(__numerical_ErrorFunction_h__)
#define __numerical_ErrorFunction_h__

#include <cassert>
#include <cmath>
#include <cstddef>

namespace numerical {

//-----------------------------------------------------------------------------
//! \defgroup numerical_specialFunctions_ErrorFunction Error Functions
//@{

//! Return the error function.
double
erf(double x);

//! Return the complementary error function.
double
erfc(double x);

} // namespace numerical

//@}

#define __numerical_specialFunctions_ErrorFunction_ipp__
#include "ErrorFunction.ipp"
#undef __numerical_specialFunctions_ErrorFunction_ipp__

#endif
