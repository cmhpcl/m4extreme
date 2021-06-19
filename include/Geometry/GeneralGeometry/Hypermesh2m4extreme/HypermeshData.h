// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(_M4EXTREME_HYPERMESHDATA_H__INCLUDED_)
#define _M4EXTREME_HYPERMESHDATA_H__INCLUDED_

#pragma once

#include <vector>
using namespace std;

namespace m4extreme {
  struct hypermeshNode
  {
    int id;
    double coordinate[3];
  };

  struct hypermeshTria3Element
  {
    int id;
    int node[3];
  };

  struct hypermeshTetra4Element
  {
    int id;
    int node[4];
  };

  struct hypermeshQuad4Element
  {
    int id;
    int node[4];
  };

  struct hypermeshHexa8Element
  {
    int id;
    int node[8];
  };

  struct hypermeshNodeSets
  {
    char name[255];
    vector<int> node;
  };

  struct hypermeshElementSets
  {
    char name[255];
    vector<int> element;
  };

  struct constraint
  {
    static const int hypermeshConstraintDofMax = 6;
    int id;
    int dof[hypermeshConstraintDofMax];
    int ex_geom;
  };

  struct hypermeshConstraintSet
  {
    int id;
    char title[255];
    vector<constraint> nodalConstraint;
  };

  struct load
  {
    static const int hypermeshLoadDofMax = 3;
    static const int hypermeshLoadValueMax = 3;
    static const int typeNodalForce = 1;
    static const int typeNodalVelocity = 5;
    static const int typeNodalDisplacement = 3;

    int id;
    int type;
    int dof_face[hypermeshLoadDofMax];
    double value[hypermeshLoadValueMax];
    bool is_expanded;
  };

  struct hypermeshLoadSet
  {
    static const int hypermeshLoadSetGravMax = 6;
    static const int hypermeshLoadSetOriginMax = 3;
    static const int hypermeshLoadSetOmegaMax = 3;

    int id;
    char title[255];
    double defTemp;
    bool tempOn;
    bool gravOn;
    bool omegaOn;
    double grav[hypermeshLoadSetGravMax];
    double origin[hypermeshLoadSetOriginMax];
    double omega[hypermeshLoadSetOmegaMax];
    vector<load> loads;
  };

}

#endif
