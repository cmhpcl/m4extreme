// -*- C++ -*-

/*!
  \file surfaceArea/Reaction.h
  \brief A reaction.
*/

#if !defined(__surfaceArea_Reaction_h__)
#define __surfaceArea_Reaction_h__

#if _MSC_VER >= 1600
#include <array>
#else
#include <tr1/array>
#endif

namespace surfaceArea {

//! The type of atom and the polar/non-polar index.
struct LogicalAtom {
   //! The atom type index.
   std::size_t atomIndex;
   //! 0/1 indicates polar/non-polar.
   /*! This may then be used to access the array of polar/non-polar areas. */
   std::size_t polarIndex;
};

//! Add a location to a LogicalAtom.
template<typename _Number>
class PhysicalAtom :
   public LogicalAtom {
public:
   //! The Cartesian point that marks the center.
   std::tr1::array<_Number, 3> center;

   //! Default constructor. Default initialized data.
   PhysicalAtom() :
      LogicalAtom(),
      center() {
   }

   //! Construct from a LogicalAtom.
   PhysicalAtom(const LogicalAtom& logicalAtom) :
      LogicalAtom(logicalAtom),
      center() {
   }

   //! Assignment operator for a LogicalAtom.
   PhysicalAtom&
   operator=(const LogicalAtom& logicalAtom) {
      atomIndex = logicalAtom.atomIndex;
      polarIndex = logicalAtom.polarIndex;
      return *this;
   }
};

} // namespace surfaceArea

#endif
