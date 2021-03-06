// -*- C++ -*-

/*!
  \file array/EquilateralArray.h
  \brief A multi-array that has equal extents in each dimension.
*/

#if !defined(__array_EquilateralArray_h__)
#define __array_EquilateralArray_h__

#include "EquilateralArrayImp.h"

namespace array {

//! A multi-array that has equal extents in each dimension.
/*!
  \param _T The value type.
  \param _D The dimension
  \param _N The extent in each dimension.
*/
template<typename _T, std::size_t _D, std::size_t _N>
class EquilateralArray :
      public EquilateralArrayImp<_T, _D, _N, std::tr1::array<_T, numerical::Exponentiation < std::size_t, _N, _D >::Result> > {
   //
   // Types.
   //
private:

   //! The base class.
   typedef EquilateralArrayImp<_T, _D, _N, std::tr1::array<_T, numerical::Exponentiation < std::size_t, _N, _D >::Result> > Base;

   //
   // Use from the base class.
   //
public:

   using Base::begin;
   using Base::end;
   using Base::size;

   //--------------------------------------------------------------------------
   /*! \name Constructors etc.
     We use the default copy constructor, assignment operator, and
     destructor. */
   // @{
public:

   //! Default constructor.
   EquilateralArray() :
      Base() {
   }

   //! Construct from a \c std::tr1::array of possibly different value type.
   template<typename _T2>
   EquilateralArray(const std::tr1::array<_T2, Base::Size>& data) :
      Base() {
      std::copy(data.begin(), data.end(), begin());
   }

   //! Construct from a range of data.
   template<typename _InputIterator>
   EquilateralArray(_InputIterator start, _InputIterator finish) :
      Base() {
      // Assume that the range has the correct number of elements.
      std::copy(start, finish, begin());
   }

   //! Construct from a pointer to data.
   template<typename _T2>
   EquilateralArray(const _T2* data) :
      Base() {
      std::copy(data, data + size(), begin());
   }

   //! Construct from a fill value.
   EquilateralArray(const typename Base::value_type& value) :
      Base() {
      std::fill(begin(), end(), value);
   }

   // @}
};


} // namespace array

#endif
