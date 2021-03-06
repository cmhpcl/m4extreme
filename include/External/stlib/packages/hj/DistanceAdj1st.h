// -*- C++ -*-

/*!
  \file DistanceAdj1st.h
  \brief Distance equation.  First-order, adjacent scheme.
*/

#if !defined(__hj_DistanceAdj1st_h__)
#define __hj_DistanceAdj1st_h__

#ifdef DEBUG_stlib
// Include the debugging code.
#include "debug.h"
#endif

#include "Distance.h"
#include "DistanceScheme.h"

namespace hj {

//! Distance equation.  Adjacent difference scheme.  1st order.
/*!
  \param N is the space dimension.
  \param T is the number type.
*/
template<std::size_t N, typename T>
class DistanceAdj1st;

} // namespace hj

#define __hj_DistanceAdj1st2_h__
#include "DistanceAdj1st2.h"
#undef __hj_DistanceAdj1st2_h__

#define __hj_DistanceAdj1st3_h__
#include "DistanceAdj1st3.h"
#undef __hj_DistanceAdj1st3_h__

#endif
