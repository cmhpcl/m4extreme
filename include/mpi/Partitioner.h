//
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_MESH_PARTITIONER_
#define _M4EXTREME_MESH_PARTITIONER_

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <stdlib.h>

#if defined(_M4EXTREME_METIS_)
extern "C" {
#include "metis.h"
}
#endif

using namespace std;

namespace m4extreme {

  template<int dim> class _NODE {
  public:
    double _x[dim];
  };

  template<int dim> class _ELEMENT {
  public:
    int _index;
    int _root;
    int _ids[dim+1];
  };

  template<int dim> class Partitioner {
  public:

  Partitioner(const char* outdir=NULL)
    :_hasGroups(false), _numofBodies(0), _elementsets(NULL), _nodesets(NULL) {
      if ( outdir == NULL ) {
	_destdir = "./data";
      }
      else {
	_destdir = outdir;
      }
    }

  Partitioner(bool hasGroups, const char* outdir=NULL)
    :_hasGroups(hasGroups), _numofBodies(0), _elementsets(NULL), _nodesets(NULL) {
      if ( outdir == NULL ) {
	_destdir = "./data";
      }
      else {
	_destdir = outdir;
      }
    }

    virtual ~Partitioner() {
	if (_nodesets != NULL) {
	  for (int i = 0; i < _numofBodies; ++i) {
	    if (_nodesets[i] != NULL) {
	      delete [] _nodesets[i];
	    }
	  }

	  delete [] _nodesets;
	}

	if (_elementsets != NULL) {
	  for (int i = 0; i < _numofBodies; ++i) {
	    if (_elementsets[i] != NULL) {
	      delete [] _elementsets[i];
	    }
	  }

	  delete [] _elementsets;
	}
      }

      virtual int operator()(int np) = 0;
      virtual void push_back(const string & filename);

      vector< _NODE<dim> > & getNodes() { return _nodes; }
      vector< _ELEMENT<dim> > & getElements() { return _elements; }

      const int getNumofBodies() const { return _numofBodies; }
      const map<string, vector<int> > & getNodeGroups() const { return _node_groups; }
      const map<string, vector<int> > & getElementGroups() const { return _solid_groups; }

      const vector<int> & getNodeGroup(const string & name) const {
	map<string, vector<int> >::const_iterator pG;
	if ( (pG=_node_groups.find(name)) != _node_groups.end() ) {
	  return pG->second;
	}

	cerr << "failed to find nodal group " << name.c_str() << endl;
	assert(false);
      }

      const vector<int> & getElementGroup(const string & name) const {
	map<string, vector<int> >::const_iterator pG;
	if ( (pG=_solid_groups.find(name)) != _solid_groups.end() ) {
	  return pG->second;
	}

	cerr << "failed to find element group " << name.c_str() << endl;
	assert(false);
      }
    
  protected:
      void dump(int);
      void insert(int, const _ELEMENT<dim> *);
        
  protected:
      string _destdir;
      bool _hasGroups;
      int _numofBodies;
      vector< _NODE<dim> > _nodes;      
      vector< _ELEMENT<dim> > _elements;
      map<int, int> _node_inverse_idmap, _elm_inverse_idmap;
      map<string, vector<int> > _node_groups, _solid_groups;
      set<int> ** _nodesets;
      vector<const _ELEMENT<dim>* > ** _elementsets;
  };

  template<int dim> class 
    RandomPartitioner : public Partitioner<dim> {
  public:
    RandomPartitioner(bool hasGroups):Partitioner<dim>(hasGroups) {}
    int operator()(int np);
  };

  template<int dim> class  
    ZPartitioner : public Partitioner<dim> {
  public:
    ZPartitioner(bool hasGroups):Partitioner<dim>(hasGroups){}
    int operator()(int np);
  };

  template<int dim> class 
    AdaptivePartitioner : public Partitioner<dim> {
  public:
    AdaptivePartitioner() : _perc(0.5) {}
    AdaptivePartitioner(double perc) : _perc(perc) {}
      AdaptivePartitioner(bool hasGroups, double perc):_perc(perc),Partitioner<dim>(hasGroups) {}
    int operator()(int np);

  private:
    double _perc;
  };

  template<int dim> class
    QuantitivePartitioner : public Partitioner<dim> {
  public:
    QuantitivePartitioner() : _quantity(0){}
    QuantitivePartitioner(int q) : _quantity(q) {}
      QuantitivePartitioner(bool hasGroups, int q) : _quantity(q), Partitioner<dim>(hasGroups) {}
    int operator()(int np);

  private:    
    int _quantity;
  };

  template<int dim> class 
    GeometricPartitioner : public Partitioner<dim> {
  public:
    GeometricPartitioner() : _cx(0.0), _cy(0.0), _r2(0.0), _z(0.0) {}
    GeometricPartitioner(double cx, double cy, double r, double z) 
      : _z(z), _r2(r*r), _cy(cy), _cx(cx) {}
    int operator()(int np);

  private:
    double _cx, _cy, _r2, _z;
  };

#if defined(_M4EXTREME_METIS_)
  template<int dim> class 
    MetisPartitioner : public Partitioner<dim> {
  public:
    MetisPartitioner(bool hasGroups, const char* outdir=NULL):Partitioner<dim>(hasGroups, outdir){}
    int operator()(int np);
  };

  template<int dim> class 
    RecursiveMetisPartitioner : public Partitioner<dim> {
  public:
    RecursiveMetisPartitioner(double x, double y):
    _center_x(x), _center_y(y), _type(0), Partitioner<dim>(false){}
    RecursiveMetisPartitioner(double x, double y, int type):
    _center_x(x), _center_y(y), _type(type), Partitioner<dim>(false){}
    RecursiveMetisPartitioner(bool hasGroups, double x, double y, int type):
    _center_x(x), _center_y(y), _type(type), Partitioner<dim>(hasGroups){}   
    int operator()(int np);

  private:
    int _type;
    double _center_x, _center_y;
  };
#endif
  
  template<int dim> int partitioning(const std::vector<std::string> & filenames, 
				     int np, int flag, double perc, int quantity, bool hasGroups,
				     const char* destdir="./data") ;
}

#include "Partitioner.ipp"

#endif //end of _M4EXTREME_MESH_PARTITIONER_
