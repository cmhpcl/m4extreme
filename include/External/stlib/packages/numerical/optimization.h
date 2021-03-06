// -*- C++ -*-

/*!
  \file optimization.h
  \brief Includes the optimization classes.
*/

/*!
  \page optimization Optimization

  1-D optimization:
  - numerical::Bracket
  - numerical::Brent
  - numerical::DBrent

  Multidimensional optimization for functors with an argument type of
  \c std::vector<double>:
  - numerical::CoordinateDescentHookeJeeves
  - numerical::ConjugateGradient
  - numerical::QuasiNewtonLBFGS

  There is also a collection of
  \ref optimizationStaticDimension "optimization algorithms"
  for functions on spaces whose dimension is known at compile-time.

  Use the optimization package by including the file numerical/optimization.h.
*/

#if !defined(__numerical_optimization_h__)
#define __numerical_optimization_h__

#include "optimization/staticDimension.h"
#include "optimization/ConjugateGradient.h"
#include "optimization/CoordinateDescentHookeJeeves.h"
#include "optimization/QuasiNewtonLBFGS.h"

#endif
