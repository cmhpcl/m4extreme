// -*- C++ -*-

/*!
  \file cuda/BBox.h
  \brief Implements a class for an axis-oriented bounding box in N dimensions.
*/

#if !defined(__cuda_BBox_h__)
#define __cuda_BBox_h__

#include "../ext/array.h"

#include "bound.h"

#ifndef __CUDA_ARCH__
#include <iostream>
#endif

namespace geom {


//! An axis-oriented bounding box in the specified dimension.
/*!
  \param _T is the number type.
  \param _N is the dimension.
  
  This class is an aggregate type. Thus it has no user-defined constructors.
*/
template<typename _T, std::size_t _N>
struct BBox {
   //
   // Types.
   //

   //! The number type.
   typedef _T Number;
   //! The point type.
   typedef std::tr1::array<_T, _N> Point;

   //
   // Constants.
   //

   //! The space dimension.
   static const std::size_t Dimension = _N;

   //
   // Member data
   //

   //! The lower corner.
   Point lower;
   //! The upper corner.
   Point upper;

   //
   // Member functions.
   //

   //! Return true if the BBox is empty.
   /*! An invalid bounding box has NaN for its coordinates. */
   __device__
   __host__
   bool
   isValid() const {
      return lower[0] == lower[0];
   }

   //! Invalidate the box by setting the coordinates to NaN.
   __device__
   __host__
   void
   invalidate() {
      for (std::size_t i = 0; i != Dimension; ++i) {
         lower[i] = nan(0);
         upper[i] = nan(0);
      }
   }

   // CONTINUE: See if I can remove this and use isValid instead.
   //! Return true if the BBox is empty.
   __device__
   __host__
   bool
   isEmpty() const {
      for (std::size_t i = 0; i != Dimension; ++i) {
         if (lower[i] > upper[i]) {
            return true;
         }
      }
      return false;
   }

   //! Offset (expand or contract) by the specified amount.
   __device__
   __host__
   void
   offset(const Number value) {
      lower -= value;
      upper += value;
   }

   //! Return true if the object is contained in this bounding box.
   template<typename _Object>
   __device__
   __host__
   bool
   isIn(const _Object& x) const {
      // Check if the bounding box around the object is contained in this 
      // bounding box.
      return isIn(geom::bound(x));
   }

   //! Return true if the point is in this bounding box.
   __device__
   __host__
   bool
   isIn(const Point& p) const {
      for (std::size_t i = 0; i != Dimension; ++i) {
         if (p[i] < lower[i] || upper[i] < p[i]) {
            return false;
         }
      }
      return true;
   }

   //! Return true if the bounding box \c x is in this bounding box.
   __device__
   __host__
   bool
   isIn(const BBox& x) const {
      return isIn(x.lower) && isIn(x.upper);
   }

   //! Make the bounding box expand to contain the object.
   template<typename _Object>
   __device__
   __host__
   void
   add(const _Object& x) {
      add(geom::bound(x));
   }

   //! Make the bounding box expand to contain the point.
   __device__
   __host__
   void
   add(const Point& p) {
      for (std::size_t i = 0; i != Dimension; ++i) {
         if (p[i] < lower[i]) {
            lower[i] = p[i];
         }
         if (p[i] > upper[i]) {
            upper[i] = p[i];
         }
      }
   }

   //! Make the bounding box expand to contain the box.
   __device__
   __host__
   void
   add(const BBox& x) {
      add(x.lower);
      add(x.upper);
   }

   //! Bound the sequence of objects.
   template<typename _InputIterator>
   __device__
   __host__
   void
   bound(_InputIterator begin, _InputIterator end) {
      // If there are no objects, make the box invalid.
      if (begin == end) {
         invalidate();
         return;
      }
      // Bound the first object.
      *this = geom::bound(*begin++);
      // Add the rest of the objects.
      while (begin != end) {
         add(*begin++);
      }
   }
};


//
// Equality Operators.
//


//! Equality.
/*! \relates BBox */
template<typename _T, std::size_t _N>
inline
__device__
__host__
bool
operator==(const BBox<_T, _N>& a, const BBox<_T, _N>& b) {
   return (a.lower == b.lower && a.upper == b.upper);
}


//! Inequality.
/*! \relates BBox */
template<typename _T, std::size_t _N>
inline
__device__
__host__
bool
operator!=(const BBox<_T, _N>& a, const BBox<_T, _N>& b) {
   return !(a == b);
}


//
// File I/O.
//

#ifndef __CUDA_ARCH__

//! Read the bounding box.
/*! \relates BBox */
template<typename _T, std::size_t _N>
inline
__host__
std::istream&
operator>>(std::istream& in, BBox<_T, _N>& x) {
   return in >> x.lower >> x.upper;
}


//! Write the bounding box.
/*! \relates BBox */
template<typename _T, std::size_t _N>
inline
__host__
std::ostream&
operator<<(std::ostream& out, const BBox<_T, _N>& x) {
   return out << x.lower << ' ' << x.upper;
}


//! Print in a nice format.
/*! \relates BBox */
template<typename _T, std::size_t _N>
__host__
void
printFormatted(std::ostream& out, const BBox<_T, _N>& x) {
   for (std::size_t i = 0; i != _N; ++i) {
      if (i != 0) {
         out << " x ";
      }
      out << "[ " << x.lower[i] << " ... " << x.upper[i] << " ]";
   }
}

#endif

//
// Mathematical Functions.
//


//! Make a bounding box from the lower and upper corners.
template<typename _T, std::size_t _N>
inline
__device__
__host__
BBox<_T, _N>
makeBBox(const std::tr1::array<_T, _N>& lower,
         const std::tr1::array<_T, _N>& upper) {
   BBox<_T, _N> x = {lower, upper};
   return x;
}


//! Return the content (length, area, volume, etc.) of the box.
template<typename _T, std::size_t _N>
inline
__device__
__host__
_T
content(const BBox<_T, _N>& box) {
   _T x = 1;
   for (std::size_t n = 0; n != _N; ++n) {
      x *= box.upper[n] - box.lower[n];
   }
   return x;
}


//! Put a bounding box around a point.
template<typename _T, std::size_t _N>
inline
__device__
__host__
BBox<_T, _N>
bound(const std::tr1::array<_T, _N>& p) {
   BBox<_T, _N> box = {p, p};
   return box;
}


//! Put a bounding box around two points.
template<typename _T, std::size_t _N>
inline
__device__
__host__
BBox<_T, _N>
bound(const std::tr1::array<_T, _N>& p, const std::tr1::array<_T, _N>& q) {
   BBox<_T, _N> box;
   for (std::size_t i = 0; i != _N; ++i) {
      box.lower[i] = ::min(p[i], q[i]);
      box.upper[i] = ::max(p[i], q[i]);
   }
   return box;
}


//! Put a bounding box around three points.
template<typename _T, std::size_t _N>
inline
__device__
__host__
BBox<_T, _N>
bound(const std::tr1::array<_T, _N>& p, const std::tr1::array<_T, _N>& q,
      const std::tr1::array<_T, _N>& r) {
   BBox<_T, _N> box;
   for (std::size_t i = 0; i != _N; ++i) {
      box.lower[i] = ::min(p[i], ::min(q[i], r[i]));
      box.upper[i] = ::max(p[i], ::max(q[i], r[i]));
   }
   return box;
}


//! Put a bounding box around a box.
template<typename _T, std::size_t _N>
inline
__device__
__host__
const BBox<_T, _N>&
bound(const BBox<_T, _N>& x) {
   return x;
}

//! Return the squared distance between two 1-D intervals.
template<typename _T>
__device__
__host__
_T
squaredDistanceBetweenIntervals(const _T lower1, const _T upper1,
                                const _T lower2, const _T upper2) {
   // Consider the intervals [l1..u1] and [l2..u2].
   // l1 u1 l2 u2
   if (upper1 < lower2) {
      return (upper1 - lower2) *(upper1 - lower2);
   }
   // l2 u2 l1 u2
   if (upper2 < lower1) {
      return (upper2 - lower1) *(upper2 - lower1);
   }
   return 0;
}


//! Return the squared distance between two bounding boxes.
template<typename _T, std::size_t _N>
__device__
__host__
_T
squaredDistance(const BBox<_T, _N>& x, const BBox<_T, _N>& y) {
   _T d2 = 0;
   for (std::size_t i = 0; i != _N; ++i) {
      d2 += squaredDistanceBetweenIntervals(x.lower[i], x.upper[i],
                                            y.lower[i], y.upper[i]);
   }
   return d2;
}


//! Return true if the bounding boxes overlap.
/*! \relates BBox */
template<typename _T, std::size_t _N>
__device__
__host__
bool
doOverlap(const BBox<_T, _N>& a, const BBox<_T, _N>& b) {
   for (std::size_t i = 0; i != _N; ++i) {
      if (::max(a.lower[i], b.lower[i]) >
          ::min(a.upper[i], b.upper[i])) {
         return false;
      }
   }
   return true;
}


//! Return the intersection of the bounding boxes.
/*! \relates BBox */
template<typename _T, std::size_t _N>
__device__
__host__
BBox<_T, _N>
intersection(const BBox<_T, _N>& a, const BBox<_T, _N>& b) {
   BBox<_T, _N> box = {::max(a.lower, b.lower), ::min(a.upper, b.upper)};
   return box;
}

// The scan conversion function are not implemented for CUDA.

} // namespace geom

#endif
