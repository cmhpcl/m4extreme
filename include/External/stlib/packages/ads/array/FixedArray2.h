// -*- C++ -*-

/*!
  \file FixedArray2.h
  \brief 2-D specialization for FixedArray.
*/

#if !defined(__FixedArray2_h__)
#error This file is an implementation detail of the class FixedArray.
#endif

namespace ads {

//! Partial template specialization for N = 2.
template<typename T>
class FixedArray<2, T> {
   //
   // Private types.
   //

private:

   typedef ArrayTypes<T> Types;

   //
   // Public types.
   //

public:

   //--------------------------------------------------------------------------
   //! \name STL container requirements.
   // @{

   //! The element type of the array.
   typedef typename Types::value_type value_type;
   //! The parameter type for the value type.
   typedef typename Types::parameter_type parameter_type;
   //! A pointer to an array element.
   typedef typename Types::pointer pointer;
   //! A const pointer to an array element.
   typedef typename Types::const_pointer const_pointer;
   //! An iterator in the array.
   typedef typename Types::iterator iterator;
   //! A const iterator in the array.
   typedef typename Types::const_iterator const_iterator;
   //! A reference to an array element.
   typedef typename Types::reference reference;
   //! A const reference to an array element.
   typedef typename Types::const_reference const_reference;
   //! The size type is a signed integer.
   /*!
     Having \c std::size_t (which is an unsigned integer) as the size type
     causes minor problems.  Consult "Large Scale C++ Software Design" by
     John Lakos for a discussion of using unsigned integers in a class
     interface.
   */
   typedef typename Types::size_type size_type;
   //! Pointer difference type.
   typedef typename Types::difference_type difference_type;

   //@}

   //
   // Data
   //

private:

   value_type _data[2];

public:

   //--------------------------------------------------------------------------
   //! \name Constructors etc.
   // @{

   //! Default constructor.  Leave the data uninitialized.
   FixedArray() {}

   //! Trivial destructor.
   ~FixedArray() {}

   //! Copy constructor.
   FixedArray(const FixedArray& x) {
      _data[0] = x._data[0];
      _data[1] = x._data[1];
   }

   //! Copy constructor for a FixedArray of different type.
   template<typename T2>
   FixedArray(const FixedArray<2, T2>& x) {
      _data[0] = static_cast<value_type>(x[0]);
      _data[1] = static_cast<value_type>(x[1]);
   }

   //! Constructor.  Specify the component.
   explicit
   FixedArray(parameter_type x) {
      _data[0] = x;
      _data[1] = x;
   }

   //! Constructor.  Initialize from a C array.
   FixedArray(const void* vp) {
      const_pointer p = static_cast<const_pointer>(vp);
      _data[0] = *p++;
      _data[1] = *p;
   }

   //! Constructor.  Specify the two components.
   explicit
   FixedArray(parameter_type x0, parameter_type x1) {
      _data[0] = x0;
      _data[1] = x1;
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Assignment operators.
   // @{

   //! Assignment operator.
   FixedArray&
   operator=(const FixedArray& x) {
      if (&x != this) {
         _data[0] = x._data[0];
         _data[1] = x._data[1];
      }
      return *this;
   }

   //! Assignment operator.  Assign from a value
   FixedArray&
   operator=(parameter_type x) {
      _data[0] = x;
      _data[1] = x;
      return *this;
   }

   //! Assignment operator for a FixedArray of different type.
   template<typename T2>
   FixedArray&
   operator=(const FixedArray<2, T2>& x) {
      _data[0] = x[0];
      _data[1] = x[1];
      return *this;
   }

   //! Assignment operator for an Array.
   template<typename T2, bool A>
   FixedArray&
   operator=(const Array<1, T2, A>& x) {
#ifdef DEBUG_stlib
      assert(size() == x.size());
#endif
      _data[0] = x[0];
      _data[1] = x[1];
      return *this;
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Static members.
   // @{

   //! Return the size of the array.
   static
   size_type
   size() {
      return 2;
   }

   //! Return true if the array has zero size.
   static
   bool
   empty() {
      return false;
   }

   //! Return the size of the largest possible FixedArray.
   static
   size_type
   max_size() {
      return std::numeric_limits<int>::max() / sizeof(value_type);
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Accessors.
   // @{

   //! Return a const_iterator to the beginning of the data.
   const_iterator
   begin() const {
      return _data;
   }

   //! Return a const_iterator to the end of the data.
   const_iterator
   end() const {
      return _data + 2;
   }

   //! Return a const_pointer to the data.
   const_pointer
   data() const {
      return _data;
   }

   //! Subscripting.  Return the i_th component.
   parameter_type
   operator()(const int i) const {
#ifdef DEBUG_stlib
      assert(i == 0 || i == 1);
#endif
      return _data[i];
   }

   //! Subscripting.  Return the i_th component.
   parameter_type
   operator[](const int i) const {
#ifdef DEBUG_stlib
      assert(i == 0 || i == 1);
#endif
      return _data[i];
   }

   //! Linear search for \c x.
   template<typename EqualityComparable>
   const_iterator
   find(const EqualityComparable& x) const {
      if (_data[0] == x) {
         return begin();
      }
      if (_data[1] == x) {
         return begin() + 1;
      }
      return end();
   }

   //! Linear search for \c x.  Return the index of the matching element.
   template<typename EqualityComparable>
   int
   find_index(const EqualityComparable& x) const {
      if (_data[0] == x) {
         return 0;
      }
      if (_data[1] == x) {
         return 1;
      }
      return 2;
   }

   //! Return true if this array has the element \c x.
   template<typename EqualityComparable>
   bool
   has(const EqualityComparable& x) const {
      return (_data[0] == x || _data[1] == x);
   }

   //! Return true if the array is in sorted order.
   bool
   is_sorted() const {
      if (_data[1] < _data[0]) {
         return false;
      }
      return true;
   }

   //! Return true if the array is in sorted order.
   template<class StrictWeakOrdering>
   bool
   is_sorted(StrictWeakOrdering comp) const {
      if (comp(_data[1], _data[0])) {
         return false;
      }
      return true;
   }

   //! Return the index of the minimum element.
   int
   min_index() const {
      if (_data[0] < _data[1]) {
         return 0;
      }
      return 1;
   }

   //! Return the index of the minimum element using the comparison functor.
   template<class StrictWeakOrdering>
   int
   min_index(StrictWeakOrdering comp) const {
      if (comp(_data[0], _data[1])) {
         return 0;
      }
      return 1;
   }

   //! Return the index of the maximum element.
   int
   max_index() const {
      if (_data[0] > _data[1]) {
         return 0;
      }
      return 1;
   }

   //! Return the index of the maximum element using the comparison functor.
   template<class StrictWeakOrdering>
   int
   max_index(StrictWeakOrdering comp) const {
      if (comp(_data[0], _data[1])) {
         return 0;
      }
      return 1;
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Manipulators.
   // @{

   //! Return an iterator to the beginning of the data.
   iterator
   begin() {
      return _data;
   }

   //! Return an iterator to the end of the data.
   iterator
   end() {
      return _data + 2;
   }

   //! Return a pointer to the data.
   pointer
   data() {
      return _data;
   }

   //! Subscripting.  Return a reference to the i_th component.
   reference
   operator()(const int i) {
#ifdef DEBUG_stlib
      assert(i == 0 || i == 1);
#endif
      return _data[i];
   }

   //! Subscripting.  Return a reference to the i_th component.
   reference
   operator[](const int i) {
#ifdef DEBUG_stlib
      assert(i == 0 || i == 1);
#endif
      return _data[i];
   }

   //! Swaps data with another FixedArray of the same size and type.
   void
   swap(FixedArray& x) {
      std::swap(_data[0], x._data[0]);
      std::swap(_data[1], x._data[1]);
   }

   //! Linear search for \c x.
   template<typename EqualityComparable>
   iterator
   find(const EqualityComparable& x) {
      if (_data[0] == x) {
         return begin();
      }
      if (_data[1] == x) {
         return begin() + 1;
      }
      return end();
   }

   //! Negate each component.
   void
   negate() {
      _data[0] = - _data[0];
      _data[1] = - _data[1];
   }

   //! Fill the array with the given \c value.
   void
   fill(parameter_type value) {
      _data[0] = value;
      _data[1] = value;
   }

   //! Copy the specified range into the array.
   template<typename InputIterator>
   void
   copy(InputIterator start, InputIterator finish) {
      _data[0] = *start;
      ++start;
      _data[1] = *start;
#ifdef DEBUG_stlib
      assert(++start == finish);
#endif
   }

   //! Sort the elements of the array.
   void
   sort() {
      if (_data[1] < _data[0]) {
         std::swap(_data[0], _data[1]);
      }
   }

   //! Sort the elements of the array.
   template<class StrictWeakOrdering>
   void
   sort(StrictWeakOrdering comp) {
      if (comp(_data[1], _data[0])) {
         std::swap(_data[0], _data[1]);
      }
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Assignment operators with scalar operand.
   // @{

   //! Add x to each component.
   FixedArray&
   operator+=(parameter_type x) {
      _data[0] += x;
      _data[1] += x;
      return *this;
   }

   //! Subtract x from each component.
   FixedArray&
   operator-=(parameter_type x) {
      _data[0] -= x;
      _data[1] -= x;
      return *this;
   }

   //! Multiply each component by \c x.
   FixedArray&
   operator*=(parameter_type x) {
      _data[0] *= x;
      _data[1] *= x;
      return *this;
   }

   //! Divide each component by \c x.
   FixedArray&
   operator/=(parameter_type x) {
#ifdef DEBUG_stlib
      assert(x != 0);
#endif
      _data[0] /= x;
      _data[1] /= x;
      return *this;
   }

   //! Mod each component by \c x.
   FixedArray&
   operator%=(parameter_type x) {
#ifdef DEBUG_stlib
      assert(x != 0);
#endif
      _data[0] %= x;
      _data[1] %= x;
      return *this;
   }

   //! Left-shift each component by the \c offset.
   FixedArray&
   operator<<=(const int offset) {
      _data[0] <<= offset;
      _data[1] <<= offset;
      return *this;
   }

   //! Right-shift each component by the \c offset.
   FixedArray&
   operator>>=(const int offset) {
      _data[0] >>= offset;
      _data[1] >>= offset;
      return *this;
   }

   // @}
   //--------------------------------------------------------------------------
   //! \name Assignment operators with FixedArray operand.
   // @{

   //! Add x to this.
   template<typename T2>
   FixedArray&
   operator+=(const FixedArray<2, T2>& x) {
      _data[0] += x[0];
      _data[1] += x[1];
      return *this;
   }

   //! Subtract x from this.
   template<typename T2>
   FixedArray&
   operator-=(const FixedArray<2, T2>& x) {
      _data[0] -= x[0];
      _data[1] -= x[1];
      return *this;
   }

   //! Multiply this by x.
   template<typename T2>
   FixedArray&
   operator*=(const FixedArray<2, T2>& x) {
      _data[0] *= x[0];
      _data[1] *= x[1];
      return *this;
   }

   //! Divide this by x.
   template<typename T2>
   FixedArray&
   operator/=(const FixedArray<2, T2>& x) {
#ifdef DEBUG_stlib
      assert(x[0] != 0 && x[1] != 0);
#endif
      _data[0] /= x[0];
      _data[1] /= x[1];
      return *this;
   }

   //! Mod this by x.
   template<typename T2>
   FixedArray&
   operator%=(const FixedArray<2, T2>& x) {
#ifdef DEBUG_stlib
      assert(x[0] != 0 && x[1] != 0);
#endif
      _data[0] %= x[0];
      _data[1] %= x[1];
      return *this;
   }

   // @}
};


} // namespace ads

#define __FixedArray2_ipp__
#include "FixedArray2.ipp"
#undef __FixedArray2_ipp__
