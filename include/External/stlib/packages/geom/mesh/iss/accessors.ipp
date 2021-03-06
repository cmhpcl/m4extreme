// -*- C++ -*-

#if !defined(__geom_mesh_iss_accessors_ipp__)
#error This file is an implementation detail.
#endif

namespace geom {


// Get the previous vertex index.
template<std::size_t N, typename T>
inline
int
getPreviousVertexIndex(const IndSimpSetIncAdj<N, 1, T>& mesh,
                       const std::size_t n) {
   typedef IndSimpSetIncAdj<N, 1, T> ISS;
   typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

   const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
   std::size_t simplexIndex;
   // For each incident simplex.
   for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
         iter != iterEnd; ++iter) {
      simplexIndex = *iter;
      if (mesh.getIndexedSimplex(simplexIndex)[1] == n) {
         return mesh.getIndexedSimplex(simplexIndex)[0];
      }
   }
   assert(false);
   return -1;
}


// Get the next vertex index.
template<std::size_t N, typename T>
inline
int
getNextVertexIndex(const IndSimpSetIncAdj<N, 1, T>& mesh, const std::size_t n) {
   typedef IndSimpSetIncAdj<N, 1, T> ISS;
   typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

   const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
   std::size_t simplexIndex;
   // For each incident simplex.
   for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
         iter != iterEnd; ++iter) {
      simplexIndex = *iter;
      if (mesh.getIndexedSimplex(simplexIndex)[0] == n) {
         return mesh.getIndexedSimplex(simplexIndex)[1];
      }
   }
   assert(false);
   return -1;
}


template<std::size_t N, std::size_t M, typename T>
inline
void
getFace(const IndSimpSet<N, M, T>& mesh,
        const std::size_t simplexIndex, const std::size_t vertexIndex,
        typename IndSimpSet<N, M, T>::SimplexFace* face) {
   typedef typename IndSimpSet<N, M, T>::IndexedSimplexFace
   IndexedSimplexFace;

   IndexedSimplexFace indexedFace;
   // Get the vertex indices of the face.
   getIndexedFace(mesh, simplexIndex, vertexIndex, &indexedFace);
   // Assign the vertices of the face.
   for (std::size_t m = 0; m != M; ++m) {
      (*face)[m] = mesh.getVertices()[indexedFace[m]];
   }
}


template<std::size_t N, std::size_t M, typename T>
inline
void
getIndexedFace(const IndSimpSet<N, M, T>& mesh,
               const std::size_t simplexIndex, const std::size_t vertexIndex,
               typename IndSimpSet<N, M, T>::IndexedSimplexFace* face) {
   // The number of the vertex in the simplex.
   const std::size_t n = index(mesh.getIndexedSimplices()[simplexIndex],
                               vertexIndex);
   // Get the vertex indices of the face.
   getFace(mesh.getIndexedSimplices()[simplexIndex], n, face);
}


template<std::size_t N, std::size_t M, typename T>
inline
void
getCentroid(const IndSimpSet<N, M, T>& mesh, const std::size_t n,
            typename IndSimpSet<N, M, T>::Vertex* x) {
   // CONTINUE: This is the arithmetic mean.  Look up the formula for
   // the centroid.

   typedef typename IndSimpSet<N, M, T>::IndexedSimplex IndexedSimplex;

   const IndexedSimplex& s = mesh.getIndexedSimplices()[n];
   *x = mesh.getVertices()[s[0]];
   for (std::size_t m = 1; m != M + 1; ++m) {
      *x += mesh.getVertices()[s[m]];
   }
   *x /= T(M + 1);
}


template<std::size_t N, std::size_t M, typename T>
inline
bool
isOriented(const IndSimpSetIncAdj<N, M, T>& mesh) {
   typedef IndSimpSetIncAdj<N, M, T> ISS;
   typedef typename ISS::IndexedSimplexFace IndexedSimplexFace;

   std::size_t m, nu, mu;
   IndexedSimplexFace f, g;

   // For each simplex.
   const std::size_t simplicesSize = mesh.getSimplicesSize();
   for (std::size_t n = 0; n != simplicesSize; ++n) {
      // For each adjacent simplex.
      for (m = 0; m != M + 1; ++m) {
         // The m_th adjacent simplex.
         nu = mesh.getAdjacent(n, m);
         // If this is not a boundary face.
         if (nu != std::size_t(-1)) {
            mu = mesh.getMirrorIndex(n, m);
            getFace(mesh.getIndexedSimplex(n), m, &f);
            getFace(mesh.getIndexedSimplex(nu), mu, &g);
            reverseOrientation(&g);
            if (! haveSameOrientation(f, g)) {
               return false;
            }
         }
      }
   }

   return true;
}

} // namespace geom
