// -*- C++ -*-

#if !defined(__levelSet_positiveDistance_ipp__)
#error This file is an implementation detail of positiveDistance.
#endif

namespace levelSet {


// Construct a level set for a union of balls.
template<typename _T, std::size_t _D>
inline
void
positiveDistance(array::SimpleMultiArrayRef<_T, _D>* grid,
                 const geom::BBox<_T, _D>& domain,
                 const std::vector<geom::Ball<_T, _D> >& balls,
                 const _T offset, const _T maxDistance) {
   typedef array::SimpleMultiArrayRef<_T, _D> SimpleMultiArray;
   typedef typename SimpleMultiArray::IndexList IndexList;
   typedef typename SimpleMultiArray::Range Range;
   typedef array::SimpleMultiIndexRangeIterator<_D> Iterator;

   geom::SimpleRegularGrid<_T, _D> regularGrid(grid->extents(), domain);
   geom::BBox<_T, _D> window;
   Range range;
   std::tr1::array<_T, _D> p;
   _T d;
   geom::Ball<_T, _D> offsetBall;

   // For each ball.
   for (std::size_t i = 0; i != balls.size(); ++i) {
      // Build a bounding box around the ball.
      offsetBall.center = balls[i].center;
      offsetBall.radius = balls[i].radius + offset;
      window = bound(offsetBall);
      window.offset(maxDistance);
      // Compute the index range for the bounding box.
      range = regularGrid.computeRange(window);
      // For each index in the range.
      const Iterator end = Iterator::end(range);
      for (Iterator index = Iterator::begin(range); index != end; ++index) {
         // The grid value.
         _T& g = (*grid)(*index);
         // Convert the index to a Cartesian location.
         p = regularGrid.indexToLocation(*index);
         // Compute the signed distance to the surface of the ball.
         d = euclideanDistance(p, offsetBall.center) - offsetBall.radius;
         // If the distance is unknown or if the computed distance is less
         // than the current value, store the distance.
         if (g != g || d < g) {
            g = d;
         }
      }
   }
}


// Calculate the positive distance for the refined patches.
template<typename _T, std::size_t _D, std::size_t _N>
inline
void
positiveDistanceOnRefined
(Grid<_T, _D, _N>* grid,
 const std::vector<geom::Ball<_T, _D> >& balls,
 const array::StaticArrayOfArrays<std::size_t>& dependencies,
 const _T offset, const _T maxDistance) {
   typedef array::SimpleMultiArrayRef<_T, _D> MultiArrayRef;
   typedef typename MultiArrayRef::IndexList IndexList;
   typedef typename MultiArrayRef::Range Range;
   typedef array::SimpleMultiIndexRangeIterator<_D> Iterator;

   // Use a multi-array to wrap the patches.
   const IndexList patchExtents = (*grid)[0].extents();
   MultiArrayRef patch(0, patchExtents);
   std::vector<geom::Ball<_T, _D> > influencingBalls;
   // Loop over the patches.
   const Iterator end = Iterator::end(grid->extents());
   for (Iterator i = Iterator::begin(grid->extents()); i != end; ++i) {
      const std::size_t index = grid->arrayIndex(*i);
      if (! (*grid)[index].isRefined()) {
         continue;
      }
      // Build the parameters.
      patch.rebuild((*grid)[index].data(), patchExtents);
      influencingBalls.clear();
      for (std::size_t n = 0; n != dependencies.size(index); ++n) {
         influencingBalls.push_back(balls[dependencies(index, n)]);
      }
      // Compute the distance.
      positiveDistance(&patch, grid->getVertexPatchDomain(*i),
                       influencingBalls, offset, maxDistance);
   }
}


template<typename _T, std::size_t _D, std::size_t _N>
inline
void
positiveDistance(Grid<_T, _D, _N>* grid,
                 const std::vector<geom::Ball<_T, _D> >& balls,
                 const _T offset, const _T maxDistance) {
   // Dispense with the trivial case.
   if (grid->empty()) {
      return;
   }

   // Determine the patch/ball dependencies.
   array::StaticArrayOfArrays<std::size_t> dependencies;
   {
      // Offset the balls' radii to include the volume of calculated distance.
      std::vector<geom::Ball<_T, _D> > offsetBalls(balls);
      for (std::size_t i = 0; i != offsetBalls.size(); ++i) {
         offsetBalls[i].radius += offset + maxDistance;
      }
      // Calculate the dependencies.
      patchDependencies(*grid, offsetBalls.begin(), offsetBalls.end(),
                        &dependencies);
   }

   // Refine the appropriate patches and set the rest to have an unknown
   // distance.
   grid->refine(dependencies);

   // Calculate the positive distance on the refined patches.
   positiveDistanceOnRefined(grid, balls, dependencies, offset, maxDistance);
}


template<typename _T, std::size_t _D, std::size_t _N>
inline
void
findPatchesWithNegativeDistance
(const Grid<_T, _D, _N>& grid,
 const std::vector<geom::Ball<_T, _D> >& balls,
 const array::StaticArrayOfArrays<std::size_t>& dependencies,
 std::vector<typename Grid<_T, _D, _N>::IndexList>* patchIndices) {
   typedef array::SimpleMultiIndexRangeIterator<_D> Iterator;

   const _T HalfDiagonal = 0.5 * grid.spacing * (_N - 1) * std::sqrt(_D);
   const std::tr1::array<_T, _D> CenterOffset =
      ext::filled_array<std::tr1::array<_T, _D> >(0.5 * grid.spacing *
                                                  (_N - 1));

   assert(grid.size() == dependencies.getNumberOfArrays());
   assert(patchIndices->empty());

   std::tr1::array<_T, _D> center;
   // Loop over the patches. Track both the patch multi-index, p, and the
   // container index, i.
   Iterator p = Iterator::begin(grid.extents());
   for (std::size_t i = 0; i != dependencies.getNumberOfArrays(); ++i, ++p) {
      center = grid.getPatchLowerCorner(*p);
      center += CenterOffset;
      // For each of the relevant balls.
      for (std::size_t j = 0; j != dependencies.size(i); ++j) {
         const geom::Ball<_T, _D>& b = balls[dependencies(i, j)];
         // The patch is contained within the ball if
         // distance(center, b.center) + HalfDiagonal < b.radius
         // We use the squared distance in the conditional.
         if (HalfDiagonal < b.radius &&
             squaredDistance(center, b.center) <
             (b.radius - HalfDiagonal) * (b.radius - HalfDiagonal)) {
            patchIndices->push_back(*p);
            break;
         }
      }
   }
}


template<typename _T, std::size_t _D, std::size_t _N>
inline
void
positiveDistanceOutside(Grid<_T, _D, _N>* grid,
                        const std::vector<geom::Ball<_T, _D> >& balls,
                        const _T offset, const _T maxDistance) {
   typedef typename Grid<_T, _D, _N>::IndexList IndexList;

   // Dispense with the trivial case.
   if (grid->empty()) {
      return;
   }

   // Determine the patch/ball dependencies.
   array::StaticArrayOfArrays<std::size_t> interiorDependencies;
   // Offset the balls' radii to include the volume of calculated distance.
   std::vector<geom::Ball<_T, _D> > offsetBalls(balls);
   for (std::size_t i = 0; i != offsetBalls.size(); ++i) {
      offsetBalls[i].radius += offset + maxDistance;
   }
   // Calculate the dependencies.
   patchDependencies(*grid, offsetBalls.begin(), offsetBalls.end(),
                     &interiorDependencies);

   // Determine the patches that have all negative distances.
   std::vector<IndexList> negativePatches;
   findPatchesWithNegativeDistance(*grid, offsetBalls, interiorDependencies,
                                   &negativePatches);

   // Calculate the sizes for the positive distance dependencies.
   // Start with the sizes for the interior dependencies.
   std::vector<std::size_t> sizes(grid->size());
   for (std::size_t i = 0; i != sizes.size(); ++i) {
      sizes[i] = interiorDependencies.size(i);
   }
   // Clear the negative patches.
   for (std::size_t i = 0; i != negativePatches.size(); ++i) {
      sizes[grid->arrayIndex(negativePatches[i])] = 0;
   }

   // Copy the relevant positive patch dependencies from the interior
   // patch dependencies.
   array::StaticArrayOfArrays<std::size_t> positiveDependencies;
   positiveDependencies.rebuild(sizes.begin(), sizes.end());
   for (std::size_t i = 0; i != positiveDependencies.getNumberOfArrays(); ++i) {
      for (std::size_t j = 0; j != positiveDependencies.size(i); ++j) {
         positiveDependencies(i, j) = interiorDependencies(i, j);
      }
   }

   // Refine the appropriate patches and set the rest to have an unknown
   // distance.
   grid->refine(positiveDependencies);

   // Set the fill value for patches with all negative distances.
   for (std::size_t i = 0; i != negativePatches.size(); ++i) {
      (*grid)(negativePatches[i]).fillValue =
         - std::numeric_limits<_T>::infinity();
   }

   // Calculate the positive distance on the refined patches.
   positiveDistanceOnRefined(grid, balls, positiveDependencies, offset, 
                             maxDistance);
}


} // namespace levelSet
