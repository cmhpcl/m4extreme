//
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_MPI_CORE_TM_2D_
#define _M4EXTREME_MPI_CORE_TM_2D_

#include "MPI_Core_2D.h"

namespace m4extreme {

  class MPI_Core_TM_2D : public MPI_Core_2D {

    typedef struct {
      int _id;
      int _carrier_id;

#if defined(_M4EXTREME_BOUNDARY_CONDITIONS_) 
      // only works for linear mappings
      // dim is the dimension of embeded space
      int _dim;
      double  _A00, _A01, _A02, _A10, _A11, _A12;
#endif

      double _mass;
      double _x, _y, _T;
      double _vx, _vy;
      double _ax, _ay;
      double _flag;
    } _NODE_DATA_TYPE;

  public:

    MPI_Core_TM_2D();
    ~MPI_Core_TM_2D();
    MPI_Core_TM_2D(int rank, int size, MEMPModelBuilder * pModel, 
		   std::map<dof_type*, int> * idmap,
		   std::map<int, dof_type*> * dofmap);

  protected:
    virtual void _synchronizeNodes(); 

  }; // end_of_MPI_Core_TM_2D
  
} // end_of_m4extreme

#endif
