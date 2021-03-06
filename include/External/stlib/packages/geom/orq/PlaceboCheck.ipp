// -*- C++ -*-

#if !defined(__geom_PlaceboCheck_ipp__)
#error This file is an implementation detail of the class PlaceboCheck.
#endif

namespace geom {


//
// Mathematical member functions
//


template < std::size_t N, typename _Record, typename _MultiKey, typename _Key,
         typename _MultiKeyAccessor >
template<class OutputIterator>
inline
std::size_t
PlaceboCheck<N, _Record, _MultiKey, _Key, _MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
   std::size_t count = 0;
   typename std::vector<Record>::const_iterator recordIterator
   = _records.begin() + getStartingPoint();
   const typename std::vector<Record>::const_iterator recordEnd
   = recordIterator + getQuerySize();
   for (; recordIterator != recordEnd; ++recordIterator) {
      if (window.isIn(_multiKeyAccessor(*recordIterator))) {
         *(iter++) = *recordIterator;
         ++count;
      }
   }
   return count;
}

} // namespace geom
