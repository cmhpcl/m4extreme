// -*- C++ -*-

/*!
  \file SequentialScan.h
  \brief The sequential scan algorithm.
*/

#if !defined(__geom_SequentialScan_h__)
#define __geom_SequentialScan_h__

#include "ORQ.h"

#include "../../ads/functor/Dereference.h"

#include <vector>
#include <algorithm>

namespace geom {

//! The sequential scan algorithm for ORQ's in N-D.
/*!
  Stores a vector records.
*/
template < std::size_t N,
         typename _Record,
         typename _MultiKey =
         typename std::iterator_traits<_Record>::value_type,
         typename _Key = typename _MultiKey::value_type,
         typename _MultiKeyAccessor = ads::Dereference<_Record> >
class SequentialScan :
   public ORQ<N, _Record, _MultiKey, _Key, _MultiKeyAccessor> {
private:

   //
   // Private types.
   //

   typedef ORQ<N, _Record, _MultiKey, _Key, _MultiKeyAccessor> Base;
   //! The container of record iterators.
   typedef std::vector<_Record> Container;
   //! A const iterator.
   typedef typename Container::const_iterator ConstIterator;

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

private:

   //
   // Member data.
   //

   //! The vector of record iterators.
   std::vector<Record> _recordIterators;

protected:

   using Base::_multiKeyAccessor;

private:

   //
   // Not implemented
   //

   //! Copy constructor not implemented
   SequentialScan(const SequentialScan&);

   //! Assignment operator not implemented
   SequentialScan&
   operator=(const SequentialScan&);

public:

   //-------------------------------------------------------------------------
   //! \name Constructors.
   // @{

   //! Default constructor.
   SequentialScan() :
      Base(),
      _recordIterators() {}

   //! Reserve storage for \c size records.
   explicit
   SequentialScan(const std::size_t size) :
      Base(),
      _recordIterators() {
      _recordIterators.reserve(size);
   }

   //! Construct from a range of records.
   /*!
     \param first the beginning of a range of records.
     \param last the end of a range of records.
   */
   template<typename InputIterator>
   SequentialScan(InputIterator first, InputIterator last) :
      Base(),
      _recordIterators() {
      insert(first, last);
   }

   // @}
   //--------------------------------------------------------------------------
   /*! \name Accesors.
     Functionality inherited from ORQ.
   */
   // @{

   //! Return the number of records.
   using Base::getSize;

   //! Return true if the grid is empty.
   using Base::isEmpty;

   // @}
   //-------------------------------------------------------------------------
   //! \name Insert records.
   // @{

   //! Add a single record.
   void
   insert(const Record recordIterator) {
      _recordIterators.push_back(recordIterator);
      incrementSize();
   }

   //! Add a range of records.
   template<class InputIterator>
   void
   insert(InputIterator first, InputIterator last) {
      while (first != last) {
         insert(first);
         ++first;
      }
   }

   // @}
   //-------------------------------------------------------------------------
   //! \name Memory usage.
   // @{

   //! Return the total memory usage.
   std::size_t
   getMemoryUsage() const {
      return (sizeof(SequentialScan) +
              _recordIterators.size() * sizeof(Record));
   }

   // @}
   //-------------------------------------------------------------------------
   //! \name Window Queries.
   // @{

   //! Get the records in the window.  Return the # of records inside.
   template<typename OutputIterator>
   std::size_t
   computeWindowQuery(OutputIterator iter, const BBox& window) const;

   // @}
   //-------------------------------------------------------------------------
   //! \name File I/O.
   // @{

   //! Print the records.
   void
   put(std::ostream& out) const;

   // @}

protected:

   //! Increment the number of records.
   using Base::incrementSize;
};


//! Write to a file stream.
/*! \relates SequentialScan */
template < std::size_t N,
         typename _Record,
         typename _MultiKey,
         typename _Key,
         typename _MultiKeyAccessor >
inline
std::ostream&
operator<<(std::ostream& out,
           const SequentialScan<N, _Record, _MultiKey, _Key, _MultiKeyAccessor>&
           x) {
   x.put(out);
   return out;
}


} // namespace geom

#define __geom_SequentialScan_ipp__
#include "SequentialScan.ipp"
#undef __geom_SequentialScan_ipp__

#endif
