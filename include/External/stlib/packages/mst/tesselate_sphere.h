// -*- C++ -*-

/*!
  \file tesselate_sphere.h
  \brief Function for tesselating a unit sphere.
*/

#if !defined(__tesselate_sphere_h__)
#define __tesselate_sphere_h__

#include "../geom/mesh/iss/build.h"
#include "../geom/mesh/iss/quality.h"
#include "../geom/mesh/iss/subdivide.h"

#include <list>

#include <cmath>
#include <cassert>

namespace mst {

//! Tesselate the unit sphere.
template<typename T>
void
tesselateUnitSphere(const T maxEdgeLength,
                    geom::IndSimpSetIncAdj<3, 2, T>* mesh);


//! Tesselate the unit sphere.
template<typename T>
void
tesselateUnitSphere(const int refinementLevel,
                    geom::IndSimpSetIncAdj<3, 2, T>* mesh);


//! Tesselate all the surfaces (not just visible) of the atoms.
template<typename T, typename PointInIter, typename NumberInIter >
void
tesselateAllSurfaces(PointInIter centersBegin, PointInIter centersEnd,
                     NumberInIter radiiBegin, NumberInIter radiiEnd,
                     const T maxEdgeLength,
                     geom::IndSimpSetIncAdj<3, 2, T>* mesh);

} // namespace mst

#define __tesselate_sphere_ipp__
#include "tesselate_sphere.ipp"
#undef __tesselate_sphere_ipp__

#endif
