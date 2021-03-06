// -*- C++ -*-

/*!
  \file amr/PatchAdjacent.h
  \brief A patch that stores links to its adjacent neighbors in the orthree.
*/

#if !defined(__amr_PatchAdjacent_h__)
#define __amr_PatchAdjacent_h__

#include "Patch.h"
#include "Orthtree.h"

namespace amr {

//! A patch that stores links to its adjacent neighbors in the orthree.
/*!
  This class stores an iterator for each signed direction. If the neighbor
  is at the same or a lower level, the iterator points to that neighbor.
  If the neighbors are at higher levels, the iterator points to the lower
  corner of the block of neighbors.
*/
template<class _PatchData, class _Traits>
class PatchAdjacent : public Patch<_PatchData, _Traits> {
   //
   // Private types.
   //
private:

   typedef Patch<_PatchData, _Traits> Base;

   //
   // Public types.
   //
public:

   //! The patch data.
   typedef _PatchData PatchData;
   //! A single index.
   typedef typename _Traits::Index Index;
   //! A multiindex.
   typedef typename _Traits::IndexList IndexList;
   //! A list of sizes.
   typedef typename Base::SizeList SizeList;
   //! A spatial index.
   typedef typename _Traits::SpatialIndex SpatialIndex;
   //! The orthtree data structure.
   typedef Orthtree<PatchAdjacent, _Traits> OrthtreeType;
   //! An iterator in the orthtree data structure.
   typedef typename OrthtreeType::iterator iterator;

   //
   // Member data.
   //
private:

   std::tr1::array<iterator, 2 * _Traits::Dimension> _adjacent;

   //
   // Not implemented.
   //
private:

   // Default constructor not implemented.
   PatchAdjacent();

   //--------------------------------------------------------------------------
   //! \name Constructors etc.
   //@{
public:

   //! Allocate memory for the patch data. Set the adjacent link to a null value.
   PatchAdjacent(const SpatialIndex& spatialIndex, const SizeList& extents,
                 const iterator end) :
      Base(spatialIndex, extents),
      _adjacent() {
      std::fill(_adjacent.begin(), _adjacent.end(), end);
   }

   //! Allocate memory for the patch data. The adjacent links are copied.
   /*! Use the example patch to determine any necessary parameters. */
   PatchAdjacent(const SpatialIndex& spatialIndex, const PatchAdjacent& patch) :
      Base(spatialIndex, patch),
      _adjacent(patch._adjacent) {
   }

   //@}
   //--------------------------------------------------------------------------
   //! \name Accessors.
   //@{
public:

   using Base::getPatchData;

   //! Return an adjacent node in the specified direction.
   iterator
   getAdjacent(const int direction) const {
      return _adjacent[direction];
   }

   using Base::getMessageStreamSize;

   //@}
   //--------------------------------------------------------------------------
   //! \name Equality.
   //@{
public:

   //! Return true if the data structures are equal.
   bool
   operator==(const PatchAdjacent& other) const {
      return _adjacent == other._adjacent && Base::operator==(other);
   }

   //@}
   //--------------------------------------------------------------------------
   //! \name Manipulators.
   //@{
public:

   //! Return the adjacent node in the specified direction.
   void
   setAdjacent(const int direction, const iterator neighbor) {
      _adjacent[direction] = neighbor;
   }

   //@}
   //--------------------------------------------------------------------------
   //! \name Message stream I/O.
   //@{
public:

   using Base::write;
   using Base::read;

   //@}
};

} // namespace amr

#define __amr_PatchAdjacent_ipp__
#include "PatchAdjacent.ipp"
#undef __amr_PatchAdjacent_ipp__

#endif
