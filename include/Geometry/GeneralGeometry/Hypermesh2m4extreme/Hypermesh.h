// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(_M4EXTREME_HYPERMESH_H__INCLUDED_)
#define _M4EXTREME_HYPERMESH_H__INCLUDED_

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "HypermeshData.h"

using namespace std;

namespace m4extreme {
  class Hypermesh
  {
  private:
    ifstream _inputFile;
    string _setPosSignal;
    vector<hypermeshNode> _nodes;
    vector<hypermeshTria3Element> _tria3_elements;
    vector<hypermeshTetra4Element> _tetra4_elements;
    vector<hypermeshQuad4Element> _quad4_elements;
    vector<hypermeshHexa8Element> _hexa8_elements;
    vector<hypermeshNodeSets> _nodeSets;
    vector<hypermeshElementSets> _elementSets;
    vector<hypermeshConstraintSet> _constraintSets;
    vector<hypermeshLoadSet> _loadSets;

  public:
    // the construction function
    Hypermesh(const char* inputFileName);

    // the destruction function
    ~Hypermesh();

    // Nodes functions
    void ReadNodes(string firstLineStr);
    const vector<hypermeshNode>& AskNodeData(vector<hypermeshNode>& nodes);

    // Elements functions
    void ReadElements(string firstLineStr, int elementType);
    const vector<hypermeshTria3Element>& AskElementData(vector<hypermeshTria3Element>& tria3);
    const vector<hypermeshTetra4Element>& AskElementData(vector<hypermeshTetra4Element>& tetra4);
    const vector<hypermeshQuad4Element>& AskElementData(vector<hypermeshQuad4Element>& quad4);
    const vector<hypermeshHexa8Element>& AskElementData(vector<hypermeshHexa8Element>& hexa8);

    // Sets functions
    void ReadNodeSets(string firstLineStr);
    void ReadElementSets(string firstLineStr);
    const vector<hypermeshNodeSets>& AskNodeSetData(vector<hypermeshNodeSets>& nodeSets);
    const vector<hypermeshElementSets>& AskElementSetData(vector<hypermeshElementSets>& elementSets);

    // Constraint functions
    void ReadConstraints(string firstLineStr);

    // Load functions
    void ReadLoads(string firstLineStr);

    // Other functions
    const int& ReturnDataType(int& dataType, string lineStr);
  };

}

#endif
