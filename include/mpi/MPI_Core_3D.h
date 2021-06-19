//
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_MPI_CORE_3D_
#define _M4EXTREME_MPI_CORE_3D_

#include "MPI_Core.h"

namespace m4extreme {

  class MPI_Core_3D : public MPI_Core {

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
      double _x, _y, _z;
      double _vx, _vy, _vz;
      double _ax, _ay, _az;
      double _flag;
    } _NODE_DATA_TYPE;

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
    typedef struct {
      int _root;
      double _x, _y, _z;
      double _energy;
      double _weight;
      double _flag;
    } _MPT_DATA_TYPE;
#endif

  public:

    MPI_Core_3D();

    ~MPI_Core_3D();
    MPI_Core_3D(int rank, int size, MEMPModelBuilder * pModel, 
		std::map<dof_type*, int> * idmap,
		std::map<int, dof_type*> * dofmap);

    virtual void writeRangebox(const char * filename) const;

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
    virtual void async_free_energy(int len, const double * epsilon); 
    virtual void async_free_energy(int index, double epsilon); 
#endif

    void dump(std::ostream & os);
    void dump(ostream & os, const _NODE_DATA_TYPE & data);

    //
    // Get the missing information of the nodes in localIDs
    // Insert the nodes into the model on the current partition
    //
    template<typename InputIterator>
      void insertNodes(InputIterator first, InputIterator last) {
      // allgather the missing information from all the partitions
      std::set<int> totalIDs(first, last);
      m4extreme::mpi_synchronize(_numofCores, totalIDs);

      // collect the information missed on other partitions locally
      vector<dof_type*> out_dofs;
      vector<int> out_ids;
      vector<double> out_mass;
      std::map<dof_type*, double> & mass = _pModel->_m;
      std::map<int, dof_type*>::iterator pdof;
      std::map<dof_type*, double>::iterator pM;
      for ( set<int>::iterator pID = totalIDs.begin(); pID != totalIDs.end(); pID++ ) {
	if ( (pdof=_dofmap->find(*pID)) != _dofmap->end() ) {
	  if ( (pM=mass.find(pdof->second)) != mass.end() ) {
	    out_ids.push_back(*pID);
	    out_dofs.push_back(pdof->second);
	    out_mass.push_back(pM->second);
	  }
	}
      }

      // only find the coordinates of nodes locally
      pointset_type yemb;
      dynamic_cast<Model::Static::LocalState*> (_pModel->_pLS)->Embed(out_dofs.begin(), out_dofs.end(), yemb);

      // out_vals: 18 elements per node
      // mass, x, y, z, A00, A01, A02, A10, A11, A12, ax, ay, az, vx, vy, vz, dimension, carrier_id
      //
      int data_dim = 18;
      vector<double> out_vals;
      for ( int i = 0; i < out_dofs.size(); ++i ) {
	dof_type * xloc = out_dofs[i];

	// mass
	out_vals.push_back(out_mass[i]);
    
	// coordinates
	const point_type & ploc = yemb.find(xloc)->second;
	out_vals.push_back(ploc[0]);
	out_vals.push_back(ploc[1]);
	out_vals.push_back(ploc[2]);

	// boundary conditions, velocity and accelerations
	const vector_type & aloc = _pModel->_a->find(xloc)->second;
	const vector_type & vloc = _pModel->_v.find(xloc)->second;

#if defined(_M4EXTREME_BOUNDARY_CONDITIONS_)
	Set::Manifold::Map * floc = _pModel->_Emb.find(xloc)->second;
	int dimloc = 3;
    
	if ( floc != 0 ) { // nodes with constraints
	  Set::Euclidean::Cartesian::Embedding<0> * pEmb = 
	    dynamic_cast<Set::Euclidean::Cartesian::Embedding<0> *>(floc);
      
	  if ( pEmb != NULL ) {
	    dimloc = pEmb->size1();
	    const Set::VectorSpace::Hom & A = pEmb->LinearMapping();
	
	    switch (dimloc) {
	    case 0:
	      // A00, A01, A02, A10, A11, A12
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	  
	      // ax, ay, az
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);

	      // vx, vy, vz
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      break;
	    case 1:
	      // A00, A01, A02, A10, A11, A12
	      out_vals.push_back(A[0][0]);
	      out_vals.push_back(A[0][1]);
	      out_vals.push_back(A[0][2]);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);
	  
	      // ax, ay, az
	      out_vals.push_back(aloc[0]);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);

	      // vx, vy, vz
	      out_vals.push_back(vloc[0]);
	      out_vals.push_back(0.0);
	      out_vals.push_back(0.0);

	      break;
	    case 2:
	      // A00, A01, A02, A10, A11, A12
	      out_vals.push_back(A[0][0]);
	      out_vals.push_back(A[0][1]);
	      out_vals.push_back(A[0][2]);
	      out_vals.push_back(A[1][0]);
	      out_vals.push_back(A[1][1]);
	      out_vals.push_back(A[1][2]);
	  
	      // ax, ay, az
	      out_vals.push_back(aloc[0]);
	      out_vals.push_back(aloc[1]);
	      out_vals.push_back(0.0);
	  
	      // vx, vy, vz
	      out_vals.push_back(vloc[0]);
	      out_vals.push_back(vloc[1]);
	      out_vals.push_back(0.0);

	      break;
	    default:
	      cerr << "\nboundary conditions are not supported"
		"@MPI_Core_3D::insertNodes::dimloc! Abort." << endl;
	      MPI_Abort(MPI_COMM_WORLD, 0);
	    }
	
	  }
	  else {
	    cerr << "\nboundary conditions are not supported"
	      "@MPI_Core_3D::insertNodes::emb! Abort." << endl;
	    MPI_Abort(MPI_COMM_WORLD, 0);
	  }
	} else { // free nodes
	  // A00, A01, A02, A10, A11, A12
	  out_vals.push_back(0.0);
	  out_vals.push_back(0.0);
	  out_vals.push_back(0.0);
	  out_vals.push_back(0.0);
	  out_vals.push_back(0.0);
	  out_vals.push_back(0.0);

	  // ax, ay, az
	  out_vals.push_back(aloc[0]);
	  out_vals.push_back(aloc[1]);
	  out_vals.push_back(aloc[2]);

	  // vx, vy, vz
	  out_vals.push_back(vloc[0]);
	  out_vals.push_back(vloc[1]);
	  out_vals.push_back(vloc[2]);
	}
    
	out_vals.push_back(dimloc);
	out_vals.push_back(_pModel->_node_carrier_id.find(xloc)->second);
#else
	cerr << "boundary conditions are not pre-defined"
	  "@MPI_Core_3D::insertNodes! Abort." << endl;
	MPI_Abort(MPI_COMM_WORLD, 0);
#endif
      }

      // synchronize the missing information to all partitions
      int total_counter = 0;
      int *recv_ids = NULL;
      double *recv_data = NULL;

      m4extreme::mpi_allGatherData(_numofCores, 1, data_dim, out_ids, out_vals, total_counter, recv_ids, recv_data);
      
      // find the requested information
      // note that each partition only sends the local mass of the node, must assemble the mass
      // to get the total mass of the node
      set<int> request_ids(first, last);
      map<int, double>::iterator pGD;
      map<int, double> globalMass;
      for ( int i = 0; i < total_counter; ++i ) {
	int idloc = recv_ids[i];
	double * phead = recv_data+i*data_dim;
    
	// assemble the nodal mass
	if ( (pGD=globalMass.find(idloc)) == globalMass.end() ) {
	  globalMass.insert( make_pair(idloc, *phead) );
	}
	else {
	  pGD->second += *phead;
	}

	// for a new node from other partitions
	if ( request_ids.find(idloc) != request_ids.end() &&
	     _dofmap->find(idloc) == _dofmap->end() ) {
	  point_type * pnewloc = new point_type(3);
      
	  (*pnewloc)[0] = *(phead+1);
	  (*pnewloc)[1] = *(phead+2);
	  (*pnewloc)[2] = *(phead+3);
      
	  dof_type * xnewloc = pnewloc;
	  Set::Manifold::Map  * emb  = 0;
	  Set::Manifold::TMap * demb = 0;
      
#if defined(_M4EXTREME_BOUNDARY_CONDITIONS_)
	  const int & dimloc = (int)*(phead+16);
      
	  if ( dimloc < 3 ) {
	    xnewloc = new Set::Euclidean::Cartesian::Point(dimloc);
	
	    Set::VectorSpace::Hom A(3, dimloc);
	    if ( dimloc == 1 ) {
	      A[0][0] = *(phead+4);
	      A[0][1] = *(phead+5);
	      A[0][2] = *(phead+6);
	    }
	    else if ( dimloc == 2 ) {
	      A[0][0] = *(phead+4);
	      A[0][1] = *(phead+5);
	      A[0][2] = *(phead+6);
	      A[1][0] = *(phead+7);
	      A[1][1] = *(phead+8);
	      A[1][2] = *(phead+9);
	    }
	
	    Set::Euclidean::Cartesian::Point Origin(dimloc);	  
	    emb  = new Set::Euclidean::Cartesian::Embedding<0>(Origin, *pnewloc, A);
	    demb = new Set::Euclidean::Cartesian::Embedding<1>(*dynamic_cast<Set::Euclidean::Cartesian::Embedding<0>*>(emb));
	    delete pnewloc;
	  }
      
	  vector_type vloc(dimloc), aloc(dimloc);
      
	  if ( dimloc > 0 ) {
	    aloc[0] = *(phead+10);
	    vloc[0] = *(phead+13);
	  }
      
	  if ( dimloc > 1 ) {
	    aloc[1] = *(phead+11);
	    vloc[1] = *(phead+14);
	  }
      
	  if ( dimloc > 2 ) {
	    aloc[2] = *(phead+12);
	    vloc[2] = *(phead+15);
	  }
      
#else
	  assert(false);
#endif
      
	  _idmap->insert(make_pair(xnewloc, idloc));
	  _dofmap->insert(make_pair(idloc, xnewloc));
      
	  _pModel->_x.insert(xnewloc);
	  _pModel->_v.insert(make_pair(xnewloc, vloc));
	  _pModel->_a->insert(make_pair(xnewloc, aloc));	
	  _pModel->_Emb.insert (make_pair(xnewloc, emb));   
	  _pModel->_DEmb.insert(make_pair(xnewloc, demb));
      
	  const int & cid = (int)*(phead+17);
	  _pModel->_carriers[cid]->insert(xnewloc);
	  _pModel->_node_carrier_id.insert( make_pair(xnewloc, cid) );
    
	  request_ids.erase(idloc);
	} // end of new nodes    
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }
  
      if ( recv_data != NULL ) {
	free(recv_data);
      }

      // insert the global mass
      while ( first != last ) {
	dof_type *xloc = _dofmap->find(*first)->second;
	mass.insert( make_pair(xloc, globalMass.find(*first)->second) );
	first++;
      }
  
      return;
    }

  private:
    void _constructCommunicationMap();
    void _synchronizeNodes(); 
    void _constructMigrationPath_I(double *, vector<COST_TYPE> &);   
    void _constructMigrationPath_II(double *, vector<COST_TYPE> &);   
    void _migrateNodes();
    void _migrateMpts(int);

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
    void clean_buffered_mpts();
    void clean_buffered_mpts(int);
#endif

  private:
      double *_xmin, *_ymin, *_zmin;
      double *_xmax, *_ymax, *_zmax;

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
      double *_xpmin, *_ypmin, *_zpmin;
      double *_xpmax, *_ypmax, *_zpmax;
      std::vector<int> _overlaps_p;
      std::map<int, int> _cm_recv_p;
      std::map<int, std::vector<_MPT_DATA_TYPE> > _shadowMpts;
      MPI_Datatype _energy_type;
      std::vector<int> _xpnew;
#endif
  }; // end_of_MPI_Core_3D

} // end_of_m4extreme

#endif
