// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
///////////////////////////////////////////////////////////////////////////////

#if !defined(Femap_h_)
#define Femap_h_

#include <fstream>
#include <vector>
#include <map> 
#include "FemapData.h"  

namespace femap {

  class Femap {
  public:
    int getNumberOfMaterials(void) const    { return _materials.size(); };
    int getNumberOfProperties(void) const   { return _properties.size(); };
    int getNumberOfNodes(void) const        { return _nodes.size(); };
    int getNumberOfElements(void) const     { return _elements.size(); };
    int getNumberOfElements(int t); // no. of elements with topology indicator t
    int getNumberOfConstraintSets(void) const { return _constraintSets.size(); };
    int getNumberOfLoadSets(void) const     { return _loadSets.size(); };
    int getNumberOfGroups(void) const       { return _groups.size(); };

    std::vector<femapMaterial>      getMaterials(void) const  { return _materials; };
    std::vector<femapProperty>      getProperties(void) const { return _properties; };
    std::vector<femapNode>          getNodes(void) const      { return _nodes; };
    std::vector<femapElement>       getElements(void) const   { return _elements; };
    std::vector<femapElement>       getElements(int t); // gets elements with topology t
    std::vector<femapConstraintSet> getConstraintSets(void) const { return _constraintSets; };
    std::vector<femapLoadSet>       getLoadSets(void) const   { return _loadSets; };
    std::vector<femapGroup>         getGroups(void) const     { return _groups; };

    const femapMaterial&      getMaterial(int i) const { return _materials[i]; };
    const femapProperty&      getProperty(int i) const { return _properties[i]; };
    const femapNode&          getNode(int i) const     { return _nodes[i]; };
    const femapElement&       getElement(int i) const  { return _elements[i]; };
    const femapConstraintSet& getConstraintSet(int i) const { return _constraintSets[i]; };
    const femapLoadSet&       getLoadSet(int i) const  { return _loadSets[i]; };
    const femapGroup&         getGroup(int i) const    { return _groups[i]; };

    int getMaterialId(int i)       { return _materialIdMap[i]; };
    int getPropertyId(int i)       { return _propertyIdMap[i]; };
    int getNodeId(int i)           { return _nodeIdMap[i]; };
    int getElementId(int i)        { return _elementIdMap[i]; };
    int getConstraintSetId(int i)  { return _constraintSetIdMap[i]; };
    int getLoadSetId(int i)        { return _loadSetIdMap[i]; };
    int getGroupId(int i)          { return _groupIdMap[i]; };

  protected:
    std::vector<femapMaterial>      _materials;
    std::vector<femapProperty>      _properties;
    std::vector<femapNode>          _nodes;
    std::vector<femapElement>       _elements;
    std::vector<femapGroup>         _groups;
    std::vector<femapConstraintSet> _constraintSets;
    std::vector<femapLoadSet>       _loadSets;

    std::map<int,int>               _materialIdMap;
    std::map<int,int>               _propertyIdMap;
    std::map<int,int>               _nodeIdMap;
    std::map<int,int>               _elementIdMap;
    std::map<int,int>               _groupIdMap;
    std::map<int,int>               _constraintSetIdMap;
    std::map<int,int>               _loadSetIdMap;

  };

  class FemapInput : public Femap {
  public:
    FemapInput(const char* fileName);

  private:
    std::ifstream _ifs;

    void _readHeader(void);
    void _readProperty(void);
    void _readNodes(void);
    void _readElements(void);
    void _readGroups(void);
    void _readConstraints(void);
    void _readLoads(void);
    void _readMaterial(void);

    // record processing
    void   skipRecord(int);
    double readRecord();
    //
    void nextLine(int);
    void nextLine(void);

    // AP
    void skipBlock(void);

  };

  class FemapOutput : public Femap {
  public:
    FemapOutput(const char* fileName) : _ofs(fileName) {return;}

  private:
    std::ofstream _ofs;

  };

}

#endif //Femap_h_
