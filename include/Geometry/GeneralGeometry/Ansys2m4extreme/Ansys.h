// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(_M4EXTREME_ANSYS_H__INCLUDED_)
#define _M4EXTREME_ANSYS_H__INCLUDED_

#pragma once
#include <fstream>
#include <vector>
#include <string>

using namespace std;

namespace m4extreme {
  struct ansysNode
  {
    unsigned int id;
    double coordinate[3];
  };

  struct ansysElement
  {
    unsigned int id;
    unsigned int node[8];
  };

  struct ansysNodeSets
  {
    char name[255];
    vector<unsigned int> node;
  };

  struct ansysElementSets
  {
    char name[255];
    vector<unsigned int> element;
  };

  class Ansys
  {
  private:
    ifstream _inputFile;
    vector<ansysNode> _nodes;
    vector<ansysElement> _elements;
    vector<ansysNodeSets> _nodeSets;
    vector<ansysElementSets> _elementSets;

    int _nodeNum;
    int _elemNum;

  public:
    // the construction function
    Ansys(const char* inputFileName);

    // the destruction function
    ~Ansys();

    // Nodes functions
    const vector<ansysNode>& AskNodeData(vector<ansysNode>& nodes);
    const int& ReturnNodeNum(int& nodeNum, string lineStr);

    // Elements functions
    const vector<ansysElement>& AskElementData(vector<ansysElement>& elements);
    const int& ReturnElemNum(int& elemNum, string lineStr);

    // Sets functions
    const vector<ansysNodeSets>& AskNodeSetData(vector<ansysNodeSets>& nodeSets);
    const vector<ansysElementSets>& AskElementSetData(vector<ansysElementSets>& elementSets);

    // Other functions
    const int& ReturnDataType(int& dataType, string lineStr);
    const int& WhetherIgnoreThisLine(int& dataFlag, string lineStr);
  };

}

#endif
