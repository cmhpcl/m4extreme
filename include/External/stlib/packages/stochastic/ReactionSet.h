// -*- C++ -*-

/*!
  \file stochastic/ReactionSet.h
  \brief The state of the stochastic simulation.
*/

#if !defined(__stochastic_ReactionSet_h__)
#define __stochastic_ReactionSet_h__

#include "Reaction.h"

#include "../ext/vector.h"

namespace stochastic {

//! The state of the stochastic simulation.
template<bool _IsDiscrete>
class ReactionSet {
   //
   // Public types.
   //
public:

   //! The reaction type.
   typedef Reaction<_IsDiscrete> ReactionType;

   //
   // Private types.
   //
private:

   typedef std::vector<ReactionType> ReactionContainer;

   //
   // More public types.
   //
public:

   //! A const iterator on reactions.
   typedef typename ReactionContainer::const_iterator ReactionConstIterator;

   //
   // Member data.
   //
private:

   //! The reactions.
   ReactionContainer _reactions;

   //
   // Not implemented.
   //
private:

   //! Assignment operator not implemented.
   ReactionSet&
   operator=(const ReactionSet&);


   //--------------------------------------------------------------------------
   //! \name Constructors etc.
   //@{
public:

   //! Default constructor.
   ReactionSet() :
      _reactions() {}

   //! Construct from a range of reactions.
   template<typename _InputIterator>
   ReactionSet(_InputIterator reactionsBeginning, _InputIterator reactionsEnd) :
      _reactions(reactionsBeginning, reactionsEnd) {
   }

   // Use the default copy constructor and destructor.

   //@}
   //--------------------------------------------------------------------------
   //! \name Accessors.
   //@{
public:

   //! Get the number of reactions.
   std::size_t
   getSize() const {
      return _reactions.size();
   }

   //! Get the specified reaction.
   const ReactionType&
   getReaction(const std::size_t n) const {
      return _reactions[n];
   }

   //! Get the beginning of the range of reactions.
   ReactionConstIterator
   getBeginning() const {
      return _reactions.begin();
   }

   //! Get the end of the range of reactions.
   ReactionConstIterator
   getEnd() const {
      return _reactions.end();
   }

   //! Return the specified propensity function.
   template<typename Container>
   double
   computePropensity(const std::size_t n, const Container& populations) const {
      return _reactions[n].computePropensityFunction(populations);
   }

   // CONTINUE
#if 0
   //! Compute the propensity functions.
   template<typename _Container, typename _RandomAccessIterator>
   void
   computePropensities(const _Container& populations,
                       _RandomAccessIterator propensities) const {
      for (std::size_t i = 0; i != _reactions.size(); ++i) {
         propensities[i] = _reactions[i].computePropensityFunction(populations);
      }
   }
#endif

   // CONTINUE: REMOVE
#if 0
   //! Compute the number of species from the reactions.
   std::size_t
   computeNumberOfSpecies() const {
      int maxIndex = -1;
      for (std::size_t i = 0; i != _reactions.size(); ++i) {
         // computeMaximum checks for empty arrays.
         maxIndex = std::max(maxIndex, ads::computeMaximum
                             (_reactions[i].getReactants().getIndices()));
         maxIndex = std::max(maxIndex, ads::computeMaximum
                             (_reactions[i].getProducts().getIndices()));
      }
      return maxIndex + 1;
   }
#endif

   //@}
   //--------------------------------------------------------------------------
   //! \name Manipulators.
   //@{
public:

   //! Set the rate constants.
   template<typename _ForwardIterator>
   void
   setRateConstants(_ForwardIterator beginning, _ForwardIterator end) {
#ifdef DEBUG_stlib
      assert(std::distance(beginning, end) == getSize());
#endif
      for (std::size_t i = 0; i != getSize(); ++i, ++beginning) {
         _reactions[i].setRateConstant(*beginning);
      }
   }

   //! Set all of the rate constants to a value.
   void
   setRateConstants(const double value) {
      for (std::size_t i = 0; i != getSize(); ++i) {
         _reactions[i].setRateConstant(value);
      }
   }

   //! Set the specified rate constant.
   void
   setRateConstant(const std::size_t n, const double value) {
      _reactions[n].setRateConstant(value);
   }

   //! Build from the reactions.
   template<typename _InputIterator>
   void
   rebuild(_InputIterator reactionsBeginning, _InputIterator reactionsEnd);

   //@}
};


//! Return true if the states are equal.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
bool
operator==(const ReactionSet<_IsDiscrete>& x, const ReactionSet<_IsDiscrete>& y);

//! Return true if the states are not equal.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
inline
bool
operator!=(const ReactionSet<_IsDiscrete>& x, const ReactionSet<_IsDiscrete>& y) {
   return !(x == y);
}

//! Write the reactions in ascii format.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
void
writeAscii(std::ostream& out, const ReactionSet<_IsDiscrete>& x);

//! Read the reactions in ascii format.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
void
readAscii(std::istream& in, ReactionSet<_IsDiscrete>* x);

//! Read the reactants and products in ascii format.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
void
readReactantsAndProductsAscii(std::istream& in, ReactionSet<_IsDiscrete>* x);

//! Read the reactants and products in ascii format.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
void
readReactantsAndProductsAscii(std::istream& in,
                              const std::size_t numberOfReactions,
                              ReactionSet<_IsDiscrete>* x);

//! Read the rate constants in ascii format.
/*! \relates ReactionSet */
template<bool _IsDiscrete>
void
readRateConstantsAscii(std::istream& in, ReactionSet<_IsDiscrete>* x);

} // namespace stochastic

#define __stochastic_ReactionSet_ipp__
#include "ReactionSet.ipp"
#undef __stochastic_ReactionSet_ipp__

#endif
