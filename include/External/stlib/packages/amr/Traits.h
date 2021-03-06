// -*- C++ -*-

/*!
  \file amr/Traits.h
  \brief The traits for an orthtree.
*/

#if !defined(__amr_Traits_h__)
#define __amr_Traits_h__

#include "SpatialIndexMorton.h"

#include "../array/MultiIndexTypes.h"

namespace amr {

//! The traits for an orthtree.
/*!
  \param _Dimension The dimension of the space.
  \param _MaximumLevel The maximum level in the tree.
  \param _SpatialIndex The spatial index data structure.  This determines
  the level and position of the node.  It also holds a key for storing the
  node in the \c std::map data structure.
  \param _Number The real number type.
*/
template<std::size_t _Dimension, std::size_t _MaximumLevel,
         template<std::size_t, std::size_t> class _SpatialIndex =
         SpatialIndexMorton,
         typename _Number = double>
struct Traits {
   //! The space dimension.
   static const std::size_t Dimension = _Dimension;
   //! The maximum level.
   static const std::size_t MaximumLevel = _MaximumLevel;
   //! A single index.
   typedef typename array::MultiIndexTypes<Dimension>::Index Index;
   //! A multi-index in a multi-array.
   typedef typename array::MultiIndexTypes<Dimension>::IndexList IndexList;
   //! A list if sizes.
   typedef typename array::MultiIndexTypes<Dimension>::SizeList SizeList;
   //! The spatial index.
   typedef _SpatialIndex<Dimension, MaximumLevel> SpatialIndex;
   //! The number type.
   typedef _Number Number;
   //! A Cartesian point.
   typedef std::tr1::array<Number, Dimension> Point;
   //! The number of orthants = 2^Dimension.
   static const std::size_t NumberOfOrthants = SpatialIndex::NumberOfOrthants;
};

} // namespace amr

#endif
