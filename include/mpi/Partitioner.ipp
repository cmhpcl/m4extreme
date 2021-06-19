//
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "Partitioner.h"

#define _M4EXTREME_MAX_BUF_SIZE_ 255

namespace m4extreme {

  template <int dim>
    void Partitioner<dim>::push_back(const string & filename) {

        std::ifstream ifs(filename.c_str());

        if (ifs.fail()) {
            cerr << "Cannot open mesh file " << filename.c_str()
                    << ". Quitting" << endl;
            exit(1);
        }

        string str;
        getline(ifs, str);

        /**
         * read the header of the eureka lexacy data input file
         */
        int dimension,  elmType, numofNodes, numofElms;
        if (sscanf(str.c_str(), "%d %d %d %d", &dimension, &elmType, &numofNodes, &numofElms) != 4) {
            cerr << filename << "is not a eureka mesh input file. Abort" << endl;
            ifs.close();
            assert(false);
        }
	
	assert( dimension == dim && elmType == dim+1);
        //cout << "insert " << numofNodes << " nodes and " << numofElms << " elements into partitioner" << endl;

        /**
         * read the nodal coordinates,
         */
        getline(ifs, str);

	int idloc;
	map<int, int> node_idmap, elm_idmap;
        int count = 0;
        int node_inc = _nodes.size();
        _NODE<dim> Nloc;
        while (!ifs.eof() && count < numofNodes) {
	  if ( _hasGroups ) {
	    if ( dim == 2 ) {
	      if (sscanf(str.c_str(), "%d %lg %lg", &idloc, Nloc._x, Nloc._x+1) != 3) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else if ( dim == 3 ) {
	      if (sscanf(str.c_str(), "%d %lg %lg %lg", &idloc, Nloc._x, Nloc._x+1, Nloc._x+2) != 4) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else {
	      assert(false);
	    }
	    
	    node_idmap.insert( make_pair(idloc, count+node_inc) );
	  }
	  else {
	    if ( dim == 2 ) {
	      if (sscanf(str.c_str(), "%lg %lg", Nloc._x, Nloc._x+1) != 2) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else if ( dim == 3 ) {
	      if (sscanf(str.c_str(), "%lg %lg %lg", Nloc._x, Nloc._x+1, Nloc._x+2) != 3) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else {
	      assert(false);
	    }    
	  }

	  _nodes.push_back(Nloc);

	  getline(ifs, str);
	  ++count;
        }

        /**
         * read connectivity table
         */
        count = 0;
	int elm_inc = _elements.size();
        _ELEMENT<dim> Eloc;
        Eloc._root = _numofBodies;
        while (!ifs.eof() && count < numofElms) {
	  if ( _hasGroups ) {
	    if ( dim == 2 ) {
	      if (sscanf(str.c_str(), "%d %d %d %d", &idloc, Eloc._ids, Eloc._ids + 1, Eloc._ids + 2) != 4) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }  
	    }
	    else if ( dim == 3 ) {
	      if (sscanf(str.c_str(), "%d %d %d %d %d", &idloc, Eloc._ids, Eloc._ids + 1, Eloc._ids + 2, Eloc._ids + 3) != 5) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else {
	      assert(false);
	    }

	    for ( int k=0; k<dim+1; ++k ) {
	      Eloc._ids[k] = node_idmap.find(Eloc._ids[k])->second;
	    }

	    Eloc._index = elm_inc + count;

	    elm_idmap.insert( make_pair(idloc, elm_inc + count) );
	  }
	  else {
	    if ( dim == 2 ) {
	      if (sscanf(str.c_str(), "%d %d %d", Eloc._ids, Eloc._ids + 1, Eloc._ids + 2) != 3) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }  
	    }
	    else if ( dim == 3 ) {
	      if (sscanf(str.c_str(), "%d %d %d %d", Eloc._ids, Eloc._ids + 1, Eloc._ids + 2, Eloc._ids + 3) != 4) {
                cerr << "bad format. Abort" << endl;
                ifs.close();
                assert(false);
	      }
	    }
	    else {
	      assert(false);
	    }

	    for ( int k=0; k<dim+1; ++k ) {
	      Eloc._ids[k] += node_inc;
	    }
	  }

	  _elements.push_back(Eloc);

	  getline(ifs, str);
	  ++count;
        }

	if ( _hasGroups ) {
	  int grouptype;
	  char substr[_M4EXTREME_MAX_BUF_SIZE_];
	  int numofEntity;
	  vector<int> entityloc;
	  map<int, int>::iterator pC;
	  map<string, vector<int> >::iterator pG;

	  while ( !ifs.eof() ) {
	    entityloc.clear();
	    while (std::sscanf(str.c_str(), "%d %[^\n]", &grouptype, substr) == 2) {

	      string groupname(substr);
  	      std::size_t found = groupname.find('\r');
	      if ( found != std::string::npos ) {
	      	 groupname.erase(found, 1);
	      }

	      found = groupname.rfind(' ');
	      std::size_t last  = groupname.length() - 1;
	      if ( found != std::string::npos ) {
		numofEntity = atoi(groupname.substr(found+1, last).c_str());
	      }
	      else {
		break;
	      }
	      
	      if ( numofEntity < 1 ) {
	        std::getline(ifs, str);
	        continue;
	      }

	      groupname.erase(found, last);
	      
	      if ( grouptype == 7 ) {		
		for ( int i = 0; i < numofEntity; ++i ) {
		  std::getline(ifs, str);
		  idloc = atoi(str.c_str());
		  
		  if ( (pC = node_idmap.find(idloc)) != node_idmap.end() ) {
		    entityloc.push_back(pC->second);
		  }
// 		  else {
// 		    assert(false);
// 		  }
		}
		
		if ( !entityloc.empty() ) {
		  if ( (pG = _node_groups.find(groupname)) != _node_groups.end() ) {
		    for ( int i = 0; i < entityloc.size(); ++i ) {
		    	pG->second.push_back(entityloc[i]);
		    }
		  }
		  else {
		    _node_groups.insert( make_pair(groupname, entityloc) );
		  }
		}
	      }
	      else if ( grouptype == 8 ) {
		for ( int i = 0; i < numofEntity; ++i ) {
		  std::getline(ifs, str);
		  idloc = atoi(str.c_str());
		  
		  if ( (pC = elm_idmap.find(idloc)) != elm_idmap.end() ) {
		    entityloc.push_back(pC->second);
		  }
		}
		
		if ( !entityloc.empty() ) {
		  if ( (pG = _solid_groups.find(groupname)) != _solid_groups.end() ) {
		    for ( int i = 0; i < entityloc.size(); ++i ) {
		    	pG->second.push_back(entityloc[i]);
		    }
		  }
		  else {
		    _solid_groups.insert( make_pair(groupname, entityloc) );
		  }
		}
	      }
	      else {
		break;
	      }
	    }
	    
	    std::getline(ifs, str);
	  }	    
	}

        ++_numofBodies;
        ifs.close();

        return;

    }

  template <int dim>
    void Partitioner<dim>::dump(int np) {
        
        assert ( _elementsets != NULL && _nodesets != NULL );
        char buf[_M4EXTREME_MAX_BUF_SIZE_];
        ofstream ofs[_numofBodies];

        for (int i = 0; i < np; ++i) {

            for (int j = 0; j < _numofBodies; ++j) {
                const set<int> & nodesloc = _nodesets[i][j];
                const vector<const _ELEMENT<dim>*> & elmsloc = _elementsets[i][j];

                sprintf(buf, "init_%d_%d.dat", j, i);
		string partfile = _destdir + "/" + buf;
                ofs[j].open(partfile.c_str());

                if (!ofs[j].is_open()) {
                    std::cerr << "couldnot open output file " << buf << std::endl;
                    assert(false);
                } else {
		    ofs[j] << dim << " " << dim+1 << " " << nodesloc.size() << " " << elmsloc.size() << std::endl;

                    std::set<int>::const_iterator pn;
                    for (pn = nodesloc.begin(); pn != nodesloc.end(); pn++) {
                        const _NODE<dim> & nloc = _nodes[*pn];
			  ofs[j] << *pn << " ";
			for ( int k=0; k<dim; ++k ) {
			  ofs[j] << nloc._x[k] << " "; 
			}
			ofs[j] << std::endl;
                    }

		    if ( _hasGroups ) {
		      for (int k = 0; k < elmsloc.size(); k++) {
			  ofs[j] << elmsloc[k]->_index << " ";
                        const int * ids = elmsloc[k]->_ids;
			for ( int l=0; l<dim+1; ++l ) {
			    ofs[j] << ids[l] << " ";
			  }
			ofs[j] << std::endl;
		      }
		    }
		    else {
		      for (int k = 0; k < elmsloc.size(); k++) {
                        const int * ids = elmsloc[k]->_ids;
			for ( int l=0; l<dim+1; ++l ) {
			  ofs[j] << ids[l] << " "; 
			}
			ofs[j] << std::endl;
		      }
		    }

                    ofs[j].close();
                }
            }

	    delete [] _nodesets[i];
	    delete [] _elementsets[i];
        }

	delete [] _nodesets;
	delete [] _elementsets;

	_elementsets = NULL;
	_nodesets = NULL;

	if ( _hasGroups ) {
	   ofstream gfs; 

	   sprintf(buf, "groups.dat");
	   string groupfile = _destdir + "/" + buf;
           gfs.open(groupfile.c_str());

	   if (!gfs.is_open()) {
               std::cerr << "couldnot open output group file " << buf << std::endl;
               assert(false);
	   }

	   map<string, vector<int> >::iterator pG;
	   for ( pG = _node_groups.begin(); pG != _node_groups.end(); pG++ ) {
	       const vector<int> & entities = pG->second;
	       gfs << "7 " << pG->first << " " << entities.size() << std::endl;
	       for ( int i = 0; i < entities.size(); ++i ) {
	       	   gfs << entities[i] << std::endl;
	       }
	   }

	   for ( pG = _solid_groups.begin(); pG != _solid_groups.end(); pG++ ) {
	       const vector<int> & entities = pG->second;
	       gfs << "8 " << pG->first << " " << entities.size() << std::endl;
	       for ( int i = 0; i < entities.size(); ++i ) {
	       	   gfs << entities[i] << std::endl;
	       }
	   }
	}

        return;
    }

  template <int dim>
  void Partitioner<dim>::insert(int part, const _ELEMENT<dim> * pEloc) {
    assert(_nodesets[part] != NULL && _elementsets[part] != NULL);
    
    int root = pEloc->_root;
    const int * ids = pEloc->_ids;
    
    for ( int k=0; k<dim+1; ++k ) {
      _nodesets[part][root].insert(ids[k]);
    }
	  
    _elementsets[part][root].push_back(pEloc);
  }


#if defined(_M4EXTREME_METIS_)

  template <int dim>
    int MetisPartitioner<dim>::operator()(int np) {

    assert(Partitioner<dim>::_numofBodies > 0);

        /**
         * construct metis input
         */
        int elmType = dim+1;
        int numofElms = Partitioner<dim>::_elements.size();
        int numofNodes = Partitioner<dim>::_nodes.size();

        // cout << "partitioning the entire mesh with " << numofNodes << " nodes and "
	//      << numofElms << " elements into " << np << " partitions by metis library ..." << endl;

	idx_t * eptr = new idx_t[numofElms + 1];
 	idx_t * elmnts = new idx_t[numofElms * elmType];
        for (int i = 0; i < numofElms; ++i) {
	    eptr[i] = i * elmType;
            idx_t * pe = elmnts + i * elmType;
	    for ( int k=0; k<dim+1; ++k ) {
	      *(pe+k) = Partitioner<dim>::_elements[i]._ids[k];
	    }
        }

	eptr[numofElms] = numofElms * elmType; 

        /**
         * call metis partitioning algorithm
         */
        idx_t ncommon = dim;
	idx_t flag = 0;
	idx_t ** dxadj   = new idx_t*;
	idx_t ** dadjncy = new idx_t*;
	METIS_MeshToDual(&numofElms, &numofNodes, eptr, elmnts, &ncommon, &flag, dxadj, dadjncy);

	idx_t ncon = 1;
	idx_t edgecut = 0;
        idx_t * epart = new idx_t[numofElms];
	METIS_PartGraphKway(&numofElms, &ncon, *dxadj, *dadjncy, NULL, NULL, NULL, 
			     &np, NULL, NULL, NULL, &edgecut, epart);
	    
	if ( *dxadj != NULL ) METIS_Free(*dxadj);
	if ( *dadjncy != NULL ) METIS_Free(*dadjncy);
	delete dxadj;
	delete dadjncy;
        delete [] eptr;
	delete [] elmnts;

        /**
         * output partitions to lexacy files
         */
        Partitioner<dim>::_nodesets = new set<int>*[np];
        Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[np];

        for ( int i = 0; i < np; ++i ) {
            Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
            Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];
        }

        for (int i = 0; i < numofElms; i++) {
            int partnumber = epart[i];
            const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
            int root = Eloc._root;
            const int * ids = Eloc._ids;
	    for ( int k=0; k<dim+1; ++k ) {
	      Partitioner<dim>::_nodesets[partnumber][root].insert(ids[k]);
	    }

            Partitioner<dim>::_elementsets[partnumber][root].push_back(&Eloc);
        }

        delete [] epart;

        Partitioner<dim>::dump(np);

        return 0;
    }


  template <int dim>
    int RecursiveMetisPartitioner<dim>::operator()(int np) {

        assert(Partitioner<dim>::_numofBodies > 0);

	/**
	 * number of partitions = 2^n
	 */
	
	int LOOP = log(np) / log(2);
	int numofPartitions = pow(2, LOOP);
	
	assert( numofPartitions >= 2 );

        int elmType = dim+1;
        int numofElms = Partitioner<dim>::_elements.size();
        int numofNodes = Partitioner<dim>::_nodes.size();

        cout << "partitioning the entire mesh with " << numofNodes << " nodes and "
	     << numofElms << " elements into " << numofPartitions << " partitions by metis library recursively ..." << endl;
	
	int count = 0;
	int bisection = 2;

	vector< vector< const _ELEMENT<dim>* > > elmsets;
	vector< const _ELEMENT<dim>* > P[2];

	if ( _type == 1) { // bisect the domain at y=_center_y
	  double Na = 1.0 / (double) elmType;
	  for ( int i = 0; i < numofElms; ++i ) {
	    const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
	    const int * ids = Eloc._ids;
	    double yp = 0.0;
	    for ( int k = 0; k < elmType; ++k ) {
	      yp += Na * Partitioner<dim>::_nodes[ids[k]]._x[1]; 
	    }
	    
	    if ( yp < _center_y ) {
	      P[0].push_back(&Eloc);
	    }
	    else {
	      P[1].push_back(&Eloc);
	    }
	  }

	  elmsets.push_back(P[0]);
	  elmsets.push_back(P[1]);
	  P[0].clear(); P[1].clear();

	  ++count;
	}
	else if ( _type == 2 ) { // divide the domain by surface x=_center_x and y = _center_y
	  assert( numofPartitions >= 4 );

	  double Na = 1.0 / (double) elmType;

	  for ( int i = 0; i < numofElms; ++i ) {
	    const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
	    const int * ids = Eloc._ids;
	    double yp = 0.0;
	    for ( int k = 0; k < elmType; ++k ) {
	      yp += Na * Partitioner<dim>::_nodes[ids[k]]._x[1]; 
	    }
	    
	    if ( yp < _center_y ) {
	      P[0].push_back(&Eloc);
	    }
	    else {
	      P[1].push_back(&Eloc);
	    }
	  }
	  
	  for ( int i = 0; i < 2; ++i ) {
	    vector< const _ELEMENT<dim>* > Ploc[2];

	    for ( int j = 0; j < P[i].size(); ++j ) {
	      const _ELEMENT<dim> * Eloc = P[i][j];
	      const int * ids = Eloc->_ids;
	      double xp = 0.0;
	      for ( int k = 0; k < elmType; ++k ) {
		xp += Na * Partitioner<dim>::_nodes[ids[k]]._x[0]; 
	      }
	    
	      if ( xp < _center_x ) {
		Ploc[0].push_back(Eloc);
	      }
	      else {
		Ploc[1].push_back(Eloc);
	      }
	    }

	    elmsets.push_back(Ploc[0]);
	    elmsets.push_back(Ploc[1]);	   
	    P[i].clear();
	  }

	  count += 2;
	}
	else {
	  for ( int i = 0; i < numofElms; ++i ) {
	    P[0].push_back(&Partitioner<dim>::_elements[i]);
	  }
	  elmsets.push_back(P[0]);	 
	  P[0].clear(); P[1].clear();
	}

	while ( count < LOOP ) {

	  vector< vector< const _ELEMENT<dim>* > > elmsets_next;

	  for ( int i = 0; i < elmsets.size(); ++i ) {
	    const vector< const _ELEMENT<dim>* > & elmsloc = elmsets[i];
	    
	    /**
	     * construct metis input
	     */
	    int ne = elmsloc.size();
	    idx_t * melms = new idx_t[ne * elmType];
	    idx_t * eptr  = new idx_t[ne + 1];

	    for (int j = 0; j < ne; ++j) {
	      eptr[j] = j * elmType;
	      idx_t * pe = melms + j * elmType;
	      for ( int k = 0; k < elmType; ++k ) {
		*(pe+k) = elmsloc[j]->_ids[k];
	      }
	    }
	    eptr[ne] = ne * elmType;

	    /**
	     * call metis partitioning algorithm
	     */
	    idx_t ncommon = dim;
	    idx_t flag = 0; 
	    idx_t ** dxadj = new idx_t*();
	    idx_t ** dadjncy = new idx_t*();
	    METIS_MeshToDual(&ne, &numofNodes, eptr, melms, &ncommon, &flag, dxadj, dadjncy);

	    idx_t ncon = 1;
	    idx_t edgecut = 0;
	    idx_t * epart = new idx_t[ne];
	    /*METIS_PartGraphRecursive(&ne, &ncon, *dxadj, *dadjncy, NULL, NULL, &wflag, &flag, 
				     &bisection, options, &edgecut, epart);*/

	    if ( dxadj != NULL ) free(dxadj);
	    if ( dadjncy != NULL ) free(dadjncy);
            delete [] eptr;
	    delete [] melms;

	    for ( int j = 0; j < ne; ++j ) {
	      P[epart[j]].push_back(elmsloc[j]);
	    }

	    delete [] epart;

	    elmsets_next.push_back(P[0]);
	    elmsets_next.push_back(P[1]);
	    P[0].clear(); P[1].clear();
	  }

	  elmsets.clear();
	  elmsets = elmsets_next;

	  ++count;
	}

        /**
         * output partitions to lexacy files
         */
        Partitioner<dim>::_nodesets = new set<int>*[numofPartitions];
        Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[numofPartitions];

        for ( int i = 0; i < numofPartitions; ++i ) {
            Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
            Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];
        }

	for ( int i = 0; i < numofPartitions; ++i ) {
	  
	  const vector< const _ELEMENT<dim>* > & elmsloc = elmsets[i];
	  
	  for ( int j = 0; j < elmsloc.size(); ++j ) {
	    int root = elmsloc[j]->_root;
	    const int * ids = elmsloc[j]->_ids;
	    
	    for ( int k = 0; k < elmType; ++k ) {
	      Partitioner<dim>::_nodesets[i][root].insert(ids[k]);
	    }

	    Partitioner<dim>::_elementsets[i][root].push_back(elmsloc[j]);            
	  }

	}

        Partitioner<dim>::dump(np);

        return 0;
    }

#endif

  template <int dim>
    int RandomPartitioner<dim>::operator ()(int np) {

        assert(Partitioner<dim>::_numofBodies > 0);

        srand(time(NULL));
        return 0;
    }

    /**
     * suppose all the elements have been renumbered in a z-ascend order
     */
  template <int dim>
    int ZPartitioner<dim>::operator ()(int np) {

        assert(Partitioner<dim>::_numofBodies > 0);

        int numofElms = Partitioner<dim>::_elements.size();
        int elepc = numofElms / np + 0.5;

        /**
         * output partitions to lexacy files
         */
        Partitioner<dim>::_nodesets = new set<int>*[np];
        Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[np];

        int count = 0;
        
        for ( int i = 0; i < np; ++i ) {
            Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
            Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];

            count = 0;
            while ( count < elepc && i * elepc + count < numofElms) {
                const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i * elepc + count];
                int root = Eloc._root;
                const int * ids = Eloc._ids;
		for ( int k=0; k<dim+1; ++k ) {
		  Partitioner<dim>::_nodesets[i][root].insert(ids[k]);
		}

                Partitioner<dim>::_elementsets[i][root].push_back(&Eloc);
                ++count;
            }
        }

        if ( np * elepc < numofElms ) {
            count = 0;
            while ( np * elepc + count < numofElms ) {
                const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[np * elepc + count];
                int root = Eloc._root;
                const int * ids = Eloc._ids;
		for ( int k=0; k<dim+1; ++k ) {
		  Partitioner<dim>::_nodesets[np-1][root].insert(ids[k]);
		}

                Partitioner<dim>::_elementsets[np-1][root].push_back(&Eloc);

                ++count;
            }
        }

        Partitioner<dim>::dump(np);

        return 0;
    }


    /**
     * suppose all the elements have been renumbered in a z-ascend order
     */
  template <int dim>
    int AdaptivePartitioner<dim>::operator ()(int np) {

        assert(Partitioner<dim>::_numofBodies > 0);

        int numofElms = Partitioner<dim>::_elements.size();
        int elepc = _perc * numofElms / (np - 1) + 0.5;

        /**
         * output partitions to lexacy files
         */
        Partitioner<dim>::_nodesets = new set<int>*[np];
        Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[np];

        for ( int i = 0; i < np - 1; ++i ) {
            Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
            Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];

            for ( int j = 0; j < elepc; ++j ) {
                const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i * elepc + j];
                int root = Eloc._root;
                const int * ids = Eloc._ids;
		for ( int k=0; k<dim+1; ++k ) {
		  Partitioner<dim>::_nodesets[i][root].insert(ids[k]);
		}
                
                Partitioner<dim>::_elementsets[i][root].push_back(&Eloc);
	    }
	}
	
	int numofBigChunk = (np - 1) * elepc - 1;
	Partitioner<dim>::_nodesets[np-1] = new set<int>[Partitioner<dim>::_numofBodies];
	Partitioner<dim>::_elementsets[np-1] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];

	for ( int i = numofElms - 1; i > numofBigChunk; --i ) {
	  const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
	  int root = Eloc._root;
	  const int * ids = Eloc._ids;
	  for ( int k=0; k<dim+1; ++k ) {
	    Partitioner<dim>::_nodesets[np-1][root].insert(ids[k]);
	  }
	  
	  Partitioner<dim>::_elementsets[np-1][root].push_back(&Eloc);
	}
        
        Partitioner<dim>::dump(np);

        return 0;
    }

    /**
     * suppose all the elements have been renumbered in a z-ascend order
     */
  template <int dim>
    int QuantitivePartitioner<dim>::operator ()(int np) {

        assert(Partitioner<dim>::_numofBodies > 0);

        int numofElms = Partitioner<dim>::_elements.size();       
	assert( _quantity * np <= numofElms );

        /**
         * output partitions to lexacy files
         */
        Partitioner<dim>::_nodesets = new set<int>*[np];
        Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[np];

        for ( int i = 0; i < np - 1; ++i ) {
            Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
            Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];

            for ( int j = 0; j < _quantity; ++j ) {
                const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i * _quantity + j];
                int root = Eloc._root;
                const int * ids = Eloc._ids;
		for ( int k=0; k<dim+1; ++k ) {
		  Partitioner<dim>::_nodesets[i][root].insert(ids[k]);
		}
                
                Partitioner<dim>::_elementsets[i][root].push_back(&Eloc);
	    }
	}
	
	int numofBigChunk = (np - 1) * _quantity - 1;
	Partitioner<dim>::_nodesets[np-1] = new set<int>[Partitioner<dim>::_numofBodies];
	Partitioner<dim>::_elementsets[np-1] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];

	for ( int i = numofElms - 1; i > numofBigChunk; --i ) {
	  const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
	  int root = Eloc._root;
	  const int * ids = Eloc._ids;
	  for ( int k=0; k<dim+1; ++k ) {
	    Partitioner<dim>::_nodesets[np-1][root].insert(ids[k]);
	  }
	  
	  Partitioner<dim>::_elementsets[np-1][root].push_back(&Eloc);
	}
        
        Partitioner<dim>::dump(np);

        return 0;
    }

  template <int dim>
  int GeometricPartitioner<dim>::operator()(int np) {
    
    int elmType = dim+1;
    int numofElms = Partitioner<dim>::_elements.size();
    int numofNodes = Partitioner<dim>::_nodes.size();

    assert(Partitioner<dim>::_numofBodies > 0 && np >= 10 && numofElms > np);

    cout << "partitioning the entire mesh with " << numofNodes << " nodes and "
	 << numofElms << " elements into " << np << " partitions by geometric partitioner ..." << endl;

    Partitioner<dim>::_nodesets = new set<int>*[np];
    Partitioner<dim>::_elementsets = new vector<const _ELEMENT<dim>*>*[np];
    
    for ( int i = 0; i < np; ++i ) {
      Partitioner<dim>::_nodesets[i] = new set<int>[Partitioner<dim>::_numofBodies];
      Partitioner<dim>::_elementsets[i] = new vector<const _ELEMENT<dim>*>[Partitioner<dim>::_numofBodies];
    }

    vector<const _ELEMENT<dim> * > specialset;
    double Na = 1.0 / (double) elmType;

    if ( dim == 2 ) {
      return 1;
    }
    else if ( dim == 3 ) {
	  
      for ( int i = 0; i < numofElms; ++i ) {
	const _ELEMENT<dim> & Eloc = Partitioner<dim>::_elements[i];
	const int * ids = Eloc._ids;
	double xp = 0.0;
	double yp = 0.0;
	double zp = 0.0;
	for ( int k = 0; k < elmType; ++k ) {
	  xp += Na * Partitioner<dim>::_nodes[ids[k]]._x[0]; 
	  yp += Na * Partitioner<dim>::_nodes[ids[k]]._x[1];
	  zp += Na * Partitioner<dim>::_nodes[ids[k]]._x[2]; 
	}
	    
	if ( (xp - _cx) * (xp - _cx) + (yp - _cy) * (yp - _cy) <= _r2 &&
	     zp <= _z ) {
	  specialset.push_back(&Eloc);
	}
	else if ( zp > _z ){
	  Partitioner<dim>::insert(np-1, &Eloc);
	}
	else {
	  Partitioner<dim>::insert(np-2, &Eloc);
	}
      }

    }
    else {
      assert(false);
    }
	
    int ne = specialset.size();
    assert( ne > np - 2 );

    int elepc = ne / (np - 2) + 0.5;    
    int part = 0;
    while ( part < np - 3 ) {
      int count = 0;
      while ( count < elepc ) {
	Partitioner<dim>::insert(part, specialset[part * elepc + count]);
	++count;
      }

      ++part;
    }
    
    int bigChunk = (np - 3) * elepc;  
    while ( bigChunk < ne ) {
      Partitioner<dim>::insert(np-3, specialset[bigChunk]);
      ++bigChunk;
    }

    Partitioner<dim>::dump(np);

    return 0;
  }

  template<int dim> int partitioning(const std::vector<std::string> & filenames, 
				     int np, int flag, double perc, int quantity, bool hasGroups,
				     const char* destdir="./data") {
    
    m4extreme::Partitioner<dim> * partitioner = NULL;
    
    switch (flag) {
    case 0:
      partitioner = new m4extreme::RandomPartitioner<dim>(hasGroups);
      break;     
    case 1:
#if defined(_M4EXTREME_METIS_)
      partitioner = new m4extreme::MetisPartitioner<dim>(hasGroups, destdir);
#endif
      break;
    case 2:
      partitioner = new m4extreme::ZPartitioner<dim>(hasGroups);
      break;
    case 3:
      partitioner = new m4extreme::AdaptivePartitioner<dim>(hasGroups, perc);
      break;
    case 4:
      partitioner = new m4extreme::QuantitivePartitioner<dim>(hasGroups, quantity);
      break;
    default:
      cerr << "no partitioning algorithm has been selected. Abort." << endl;
      assert(false);
    }
    
    if (partitioner != NULL) {    
      for ( int i = 0; i < filenames.size(); ++i ) {
	partitioner->push_back(filenames[i]);
      }
      
      (*partitioner)(np);   
      
      delete partitioner;  
    } 
    else {
      cerr << "no partitioner is available! Abort." << endl;
      return 1;
    }
    
    return 0;
  }

} //end of namespace m4extreme
