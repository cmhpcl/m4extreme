//
// Bo Li
// Caltech, Pasadena, CA, 91125
//

#if !defined(_M4EXTREME_FEMAP2M4EXTREME_H_)
#define _M4EXTREME_FEMAP2M4EXTREME_H_

#include "Femap.h"
#include "Set/Manifold/Euclidean/Orthonormal/Orthonormal.h"

typedef Set::Euclidean::Orthonormal::Point point_type;

#if defined(__cplusplus)

namespace m4extreme {

    extern "C" {
#endif /* __cplusplus */

        bool
        Femap2m4extreme(unsigned int DIM,
		     const char* fileName,
		     vector<point_type> & v,
		     vector< vector<unsigned int> > & conn);

        bool
        Femap2m4extreme_full(unsigned int DIM,
			  const char* fileName,
			  map<unsigned int, point_type> & v,
			  map<unsigned int, vector<unsigned int> > & conn,
			  vector<femapGroup> & groups);

        void
        MeshReader(const char * inputFile,
                std::vector<point_type> &,
                std::vector< std::vector<unsigned int > > &);

#if defined(__cplusplus)
    }
}
#endif /* __cplusplus */

#endif
