// -*- C++ -*-

/*!
  \file content.h
  \brief Define functions to compute content (length, area, volume, etc.).
*/
/*!
  \page content Content

  "Content" (also called hypervolume) is a dimension independent name for
  length, area, volume, etc.  We supply functions to compute the content
  (hypervolume) of simplices.  We group the functions according to
  \ref content_distance "distance",
  \ref content_area "area" and
  \ref content_volume "volume".

  If the dimension
  of the simplex is equal to the dimension of its vertices, then the
  content is signed.  If the dimension of the simplex is less than the
  dimension of the vertices, then the content is unsigned.  (The content
  is not defined when the dimension of the simplex is greater than the
  dimension of its vertices.)  Examples:
  - The area of a triangle in 2-D is signed.
    (Simplex dimension = vertex dimension = 2.)
  - The area of a triangle in 3-D is unsigned.
    (Simplex dimension = 2.  Vertex dimension = 3.)
  - A triangle is 1-D does not have a well-defined area.
    (Simplex dimension = 2.  Vertex dimension = 1.)
*/

#if !defined(__geom_content_h__)
#define __geom_content_h__

#include "Point.h"

#include "../../ads/tensor/SquareMatrix.h"

namespace geom {

//-----------------------------------------------------------------------------
/*! \defgroup content_distance Content: Distance */
// @{


//! Return the distance between the points \c x and \c y.
/*!
  In 1-D the distance is signed.  In N-D the distance is unsigned for
  N > 1.
 */
template<std::size_t N, typename T>
T
computeDistance(const std::tr1::array<T, N>& x, const std::tr1::array<T, N>& y);

//! Return the distance between the points \c x[0] and \c x[1].
template<std::size_t N, typename T>
T
computeDistance(const std::tr1::array<std::tr1::array<T, N>, 2>& x);



//! Return the distance between the points \c x and \c y.
/*!
  In 1-D the distance is signed.  In N-D the distance is unsigned for
  N > 1.
 */
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<T, N>& x, const std::tr1::array<T, N>& y);


//! Return the distance between the points \c x[0] and \c x[1].
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<std::tr1::array<T, N>, 2>& x);



//! Calculate the gradient (with respect to x) of the distance between the points \c x and \c y.
template<std::size_t N, typename T>
void
computeGradientOfDistance(const std::tr1::array<T, N>& x,
                          const std::tr1::array<T, N>& y,
                          std::tr1::array<T, N>* gradient);


//! Calculate the gradient (with respect to x[0]) of the distance between the points \c x[0] and \c x[1].
template<std::size_t N, typename T>
void
computeGradientOfDistance(const std::tr1::array<std::tr1::array<T, N>, 2>& x,
                          std::tr1::array<T, N>* gradient);



//! Calculate the gradient (with respect to x) of the distance between the points \c x and \c y.
template<std::size_t N, typename T>
void
computeGradientOfContent(const std::tr1::array<T, N>& x,
                         const std::tr1::array<T, N>& y,
                         std::tr1::array<T, N>* gradient);


//! Calculate the gradient (with respect to x[0]) of the distance between the points \c x[0] and \c x[1].
template<std::size_t N, typename T>
void
computeGradientOfContent(const std::tr1::array<std::tr1::array<T, N>, 2>& x,
                         std::tr1::array<T, N>* gradient);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup content_area Content: Area */
// @{


//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 2-D the area is signed.
*/
template<typename T>
T
computeArea(const std::tr1::array<T, 2>& a, const std::tr1::array<T, 2>& b,
            const std::tr1::array<T, 2>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  In 2-D the area is signed.
*/
template<typename T>
T
computeArea(const std::tr1::array<std::tr1::array<T, 2>, 3>& p);


//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 3-D the area is unsigned.
*/
template<typename T>
T
computeArea(const std::tr1::array<T, 3>& a, const std::tr1::array<T, 3>& b,
            const std::tr1::array<T, 3>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  In 3-D the area is unsigned.
*/
template<typename T>
T
computeArea(const std::tr1::array<std::tr1::array<T, 3>, 3>& p);



//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 2-D the area is signed.  In N-D the area is unsigned for
  N > 2.
 */
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<T, N>& a, const std::tr1::array<T, N>& b,
               const std::tr1::array<T, N>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<std::tr1::array<T, N>, 3>& p);



//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
template<typename T>
void
computeGradientOfArea(const std::tr1::array<T, 2>& a,
                      const std::tr1::array<T, 2>& b,
                      const std::tr1::array<T, 2>& c,
                      std::tr1::array<T, 2>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
template<typename T>
void
computeGradientOfArea(const std::tr1::array<T, 3>& a,
                      const std::tr1::array<T, 3>& b,
                      const std::tr1::array<T, 3>& c,
                      std::tr1::array<T, 3>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c p[0], \c p[1] and \c p[2].
template<std::size_t N, typename T>
void
computeGradientOfArea(const std::tr1::array<std::tr1::array<T, N>, 3>& p,
                      std::tr1::array<T, N>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
/*!
  This is simply a wrapper for computeGradientOfArea.
*/
template<std::size_t N, typename T>
inline
void
computeGradientOfContent(const std::tr1::array<T, N>& a,
                         const std::tr1::array<T, N>& b,
                         const std::tr1::array<T, N>& c,
                         std::tr1::array<T, N>* gradient) {
   computeGradientOfArea(a, b, c, gradient);
}


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  This is simply a wrapper for computeGradientOfArea.
*/
template<std::size_t N, typename T>
inline
void
computeGradientOfContent(const std::tr1::array<std::tr1::array<T, N>, 3>& p,
                         std::tr1::array<T, N>* gradient) {
   computeGradientOfArea(p, gradient);
}


// @}
//-----------------------------------------------------------------------------
/*! \defgroup content_volume Content: Volume */
// @{


//! Return the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.
 */
template<std::size_t N, typename T>
T
computeVolume(const std::tr1::array<T, N>& a, const std::tr1::array<T, N>& b,
              const std::tr1::array<T, N>& c, const std::tr1::array<T, N>& d);


//! Return the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
template<std::size_t N, typename T>
T
computeVolume(const std::tr1::array<std::tr1::array<T, N>, 4>& p);



//! Return the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.

  This is simply a wrapper for computeVolume().
*/
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<T, N>& a, const std::tr1::array<T, N>& b,
               const std::tr1::array<T, N>& c, const std::tr1::array<T, N>& d);


//! Return the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.

  This is simply a wrapper for computeVolume().
*/
template<std::size_t N, typename T>
T
computeContent(const std::tr1::array<std::tr1::array<T, N>, 4>& p);



//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
template<typename T>
void
computeGradientOfVolume(const std::tr1::array<T, 3>& a,
                        const std::tr1::array<T, 3>& b,
                        const std::tr1::array<T, 3>& c,
                        const std::tr1::array<T, 3>& d,
                        std::tr1::array<T, 3>* gradient);


//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
template<std::size_t N, typename T>
inline
void
computeGradientOfVolume(const std::tr1::array<std::tr1::array<T, N>, 4>& p,
                        std::tr1::array<T, N>* gradient) {
   computeGradientOfVolume(p[0], p[1], p[2], p[3], gradient);
}



//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  This is simply a wrapper for computeGradientOfVolume().
*/
template<std::size_t N, typename T>
inline
void
computeGradientOfContent(const std::tr1::array<T, N>& a,
                         const std::tr1::array<T, N>& b,
                         const std::tr1::array<T, N>& c,
                         const std::tr1::array<T, N>& d,
                         std::tr1::array<T, N>* gradient) {
   computeGradientOfVolume(a, b, c, d, gradient);
}


//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
/*!
  This is simply a wrapper for computeGradientOfVolume().
*/
template<std::size_t N, typename T>
inline
void
computeGradientOfContent(const std::tr1::array<std::tr1::array<T, N>, 4>& p,
                         std::tr1::array<T, N>* gradient) {
   computeGradientOfVolume(p, gradient);
}


// @}

} // namespace geom

#define __geom_content_ipp__
#include "content.ipp"
#undef __geom_content_ipp__

#endif
