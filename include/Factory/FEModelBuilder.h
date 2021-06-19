// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_FEMODELBUILDER_H__INCLUDED_)
#define M4EXTREME_FEMODELBUILDER_H__INCLUDED_

#include "ModelBuilder.h"

namespace m4extreme {
    
    ////////////////////////////////////////////////////////////////////////////
    //  Finite Element Model Builder class interfaces
    ////////////////////////////////////////////////////////////////////////////

    class FEModelBuilder : public ModelBuilder {
    public:

        FEModelBuilder(Clock * Chronos, unsigned int dim) : ModelBuilder(Chronos, dim) {
        }

        ~FEModelBuilder() {
        }

        virtual void CreateModel();

    protected:

        virtual void _initializeMaterial() {
        }
        virtual void _initializeSingleElement(Geometry::Cell *,
                std::map<Geometry::Cell*, point_type> &,
                ElementBuilder *,
                const double * = NULL);
    };    
}

#endif //M4EXTREME_FEMODELBUILDER_H__INCLUDED_
