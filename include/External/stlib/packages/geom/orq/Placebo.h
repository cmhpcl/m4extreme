// -*- C++ -*-

/*!
  \file Placebo.h
  \brief A placebo class.
*/

#if !defined(__geom_Placebo_h__)
#define __geom_Placebo_h__

#include "ORQ.h"

#include "../../ads/functor/Dereference.h"
#include "../../numerical/random/uniform/DiscreteUniformGeneratorMt19937.h"

#include <vector>
#include <algorithm>

namespace geom {


//! A placebo for ORQ's in N-D
/*!
  Stores a vector of records.
*/
template < std::size_t N,
         typename _Record,
         typename _MultiKey =
         typename std::iterator_traits<_Record>::value_type,
         typename _Key = typename _MultiKey::value_type,
         typename _MultiKeyAccessor = ads::Dereference<_Record> >
class Placebo :
   public ORQ<N, _Record, _MultiKey, _Key, _MultiKeyAccessor> {
private:

   //
   // Private types.
   //

   typedef ORQ<N, _Record, _MultiKey, _Key, _MultiKeyAccessor> Base;

public:

   //
   // Public types.
   //

   //! A pointer to the record type.
   typedef _Record Record;
   //! The multi-key type.
   typedef _MultiKey MultiKey;
   //! The key type.
   typedef _Key Key;

   //! A Cartesian point.
   typedef typename Base::Point Point;
   //! Bounding box.
   typedef typename Base::BBox BBox;

protected:

   //
   // Member data.
   //

   //! The vector of pointers to records.
   std::vector<Record> _records;

   //! The number of records to return with each query.
   std::size_t _querySize;

   //! Random number generator.
   mutable numerical::DiscreteUniformGeneratorMt19937 _random;

protected:

   using Base::_multiKeyAccessor;

private:

   //
   // Not implemented
   //

   //! Copy constructor not implemented
   Placebo(const Placebo&);

   //! Assignment operator not implemented
   Placebo&
   operator=(const Placebo&);

public:

   //-------------------------------------------------------------------------
   //! \name Constructors.
   //@{

   //! Default constructor.
   Placebo() :
      Base(),
      _records(),
      _querySize(0) {}

   //! Reserve storage for \c size records.
   explicit
   Placebo(const std::size_t size) :
      Base(),
      _records(),
      _querySize(0) {
      _records.reserve(size);
   }

   //! Construct from a range of records.
   /*!
     \param first the beginning of a range of records.
     \param last the end of a range of records.
   */
   template<typename InputIterator>
   Placebo(InputIterator first, InputIterator last) :
      Base(),
      _records(),
      _querySize(0) {
      insert(first, last);
      shuffle();
   }

   //@}
   //-------------------------------------------------------------------------
   //! \name Accessors.
   //@{

   //! Return the number of records.
   using Base::getSize;

   //! Return true if the grid is empty.
   using Base::isEmpty;

   //! Return the number of records to return with each query.
   std::size_t
   getQuerySize() const {
      return _querySize;
   }

   //@}
   //-------------------------------------------------------------------------
   //! \name Manipulators.
   //@{

   //! Set the number of records to return with each query.
   void
   setQuerySize(const std::size_t size) {
      _querySize = size;
   }

   //! Shuffle the record pointers.
   void
   shuffle() {
      std::random_shuffle(_records.begin(), _records.end());
   }

   //@}
   //-------------------------------------------------------------------------
   //! \name Insert records.
   //@{

   //! Insert a single record.
   void
   insert(Record record) {
      _records.push_back(record);
      incrementSize();
   }

   //! Insert a range of records.
   template<class InputIterator>
   void
   insert(InputIterator first, InputIterator last) {
      while (first != last) {
         insert(first);
         ++first;
      }
      shuffle();
   }

   //@}
   //-------------------------------------------------------------------------
   //! \name Window queries.
   //@{

   //! Get the records in the window.  Return the # of records inside.
   template<typename OutputIterator>
   std::size_t
   computeWindowQuery(OutputIterator iter, const BBox& window) const;

   //@}
   //-------------------------------------------------------------------------
   //! \name Memory usage.
   //@{

   //! Return the total memory usage.
   std::size_t
   getMemoryUsage() const {
      return (sizeof(std::vector<Record>) + _records.size() * sizeof(Record));
   }

   //@}
   //-------------------------------------------------------------------------
   //! \name File I/O.
   //@{

   //! Print the records.
   void
   put(std::ostream& out) const;

   //@}

protected:

   //! Increment the number of records.
   using Base::incrementSize;

   //! Return a starting point for the window query.
   std::size_t
   getStartingPoint() const {
      return _random() % (_records.size() - _querySize);
   }
};


//! Write to a file stream.
/*! \relates Placebo */
template < std::size_t N,
         typename _Record,
         typename _MultiKey,
         typename _Key,
         typename _MultiKeyAccessor >
inline
std::ostream&
operator<<(std::ostream& out,
           const Placebo<N, _Record, _MultiKey, _Key, _MultiKeyAccessor>& x) {
   x.put(out);
   return out;
}


} // namespace geom

#define __geom_Placebo_ipp__
#include "Placebo.ipp"
#undef __geom_Placebo_ipp__

#endif
