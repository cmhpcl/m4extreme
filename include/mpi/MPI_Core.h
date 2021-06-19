//
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#ifndef _M4EXTREME_MPI_CORE_
#define _M4EXTREME_MPI_CORE_

#include <cassert>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <math.h>
#include <limits>
#include <numeric>

#include "mpi.h"

#include "Factory/Builder.h"

#define MAX_MPT_SIZE          4096
#define TAG_NODES_ASSEMBLE    123
#define TAG_NODES_UNION       234
#define TAG_MPTS_FAILURE      1234
#define TAG_MPTS_IMMIGRATION  2345

#define _M4EXTREME_BOUNDARY_CONDITIONS_

namespace m4extreme {

    inline
    bool pred(const std::pair<int, double>& lhs,
	      const std::pair<int, double>& rhs){
      return lhs.second < rhs.second;
    }

    inline
    int mpi_sum(int x, int numofTasks) {
      int *recv = (int*)malloc(numofTasks * sizeof(int));
      MPI_Allgather(&x, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);
      int sum = std::accumulate(recv, recv+numofTasks, 0.0);
      free(recv);
      
      return sum;
    }

    inline
    int mpi_min(int x, int numofTasks) {
      int *recv = (int*)malloc(numofTasks * sizeof(int));
      MPI_Allgather(&x, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);
      int min_x = *std::min_element(recv, recv+numofTasks);
      free(recv);
      
      return min_x;
    }

    inline
    int mpi_max(int x, int numofTasks) {
      int *recv = (int*)malloc(numofTasks * sizeof(int));
      MPI_Allgather(&x, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);
      int max_x = *std::max_element(recv, recv+numofTasks);
      free(recv);
      
      return max_x;
    }

    inline
    double mpi_sum(double x, int numofTasks) {
      double *recv = (double*)malloc(numofTasks * sizeof(double));
      MPI_Allgather(&x, 1, MPI_DOUBLE, recv, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      double sum = std::accumulate(recv, recv+numofTasks, 0.0);
      free(recv);
      
      return sum;
    }

    inline
    double mpi_min(double x, int numofTasks) {
      double *recv = (double*)malloc(numofTasks * sizeof(double));
      MPI_Allgather(&x, 1, MPI_DOUBLE, recv, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      double min_x = *std::min_element(recv, recv+numofTasks);
      free(recv);
      
      return min_x;
    }

    inline
    double mpi_max(double x, int numofTasks) {
      double *recv = (double*)malloc(numofTasks * sizeof(double));
      MPI_Allgather(&x, 1, MPI_DOUBLE, recv, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      double max_x = *std::max_element(recv, recv+numofTasks);
      free(recv);
      
      return max_x;
    }

    inline
    void mpi_extremes(double x, int numofTasks, double & min_x, double & max_x) {
      double *recv = (double*)malloc(numofTasks * sizeof(double));
      MPI_Allgather(&x, 1, MPI_DOUBLE, recv, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      min_x = *std::min_element(recv, recv+numofTasks);
      max_x = *std::max_element(recv, recv+numofTasks);
      free(recv);
      
      return;
    }

    inline
    void mpi_synchronizePotentialNodes(const map<int, Geometry::Cell*> & ids,
				      int numofTasks,
				      int rank,
				      set<Geometry::Cell*> & potentialNodes) {
      vector<int> contact_ids;
      map<int, Geometry::Cell*>::const_iterator pid = ids.begin(), idsEnd = ids.end();
      set<Geometry::Cell*>::iterator PNEnd = potentialNodes.end();
      for (; pid != idsEnd; pid++) {
	if ( potentialNodes.find(pid->second) != PNEnd )
	  contact_ids.push_back(pid->first);
      }
  
      // collecting the set of potential elements
      int recv_counter[numofTasks];
      int local_counter = contact_ids.size();
      MPI_Allgather(&local_counter, 1, MPI_INT, recv_counter, 1, MPI_INT, MPI_COMM_WORLD);
  
      int displs[numofTasks];
      int total_counter=0;
      for ( int i = 0; i < numofTasks; i++ ) {
	displs[i] = total_counter;
	total_counter += recv_counter[i];
      }

      int *recv_contact = (int*)malloc(total_counter*sizeof(int));
      MPI_Allgatherv(&contact_ids.front(), local_counter, MPI_INT, recv_contact, 
		     recv_counter, displs, MPI_INT, MPI_COMM_WORLD);
      contact_ids.clear();

      set<int> total_contact;
      for ( int i = 0; i < total_counter; i++ ) {
	total_contact.insert(recv_contact[i]);
      }

      // partitioning the set of potential elements
      potentialNodes.clear();
      for ( int i = 0; i < numofTasks - 1; i++ ) { 
	local_counter = recv_counter[i];
	for ( int j = 0; j < local_counter; j++ ) {
	  int cid = recv_contact[displs[i] + j];
	  if ( total_contact.find(cid) != total_contact.end() ) {
	    if ( i == rank ) potentialNodes.insert(ids.find(cid)->second);
	    total_contact.erase(cid);
	  }
	}

	if ( i == rank ) break;
      }
  
      if ( rank == numofTasks - 1 ) {    
	for ( set<int>::iterator ptc = total_contact.begin(); 
	      ptc != total_contact.end(); ptc++ ) {
	  if ( (pid=ids.find(*ptc)) == ids.end() ) assert(false);
	  potentialNodes.insert(pid->second);
	}
      }

      free(recv_contact);

      return;
    }

    inline
      void mpi_synchronizeTBPotentials(int numofTasks,
				       int rank,
				       std::vector<int> & contact_ids) {  
      // collecting the set of potential elements
      int recv_counter[numofTasks];
      int local_counter = contact_ids.size();
      MPI_Allgather(&local_counter, 1, MPI_INT, recv_counter, 1, MPI_INT, MPI_COMM_WORLD);
  
      int displs[numofTasks];
      int total_counter=0;
      for ( int i = 0; i < numofTasks; i++ ) {
	displs[i] = total_counter;
	total_counter += recv_counter[i];
      }

      int *recv_contact = (int*)malloc(total_counter*sizeof(int));
      MPI_Allgatherv(&contact_ids.front(), local_counter, MPI_INT, recv_contact, 
		     recv_counter, displs, MPI_INT, MPI_COMM_WORLD);

      std::set< std::pair<int, int> > total_contact;
      for ( int i = 0; i < total_counter; i+=2 ) {
	std::pair<int, int> p0(recv_contact[i],   recv_contact[i+1]);
	std::pair<int, int> p1(recv_contact[i+1], recv_contact[i]);
	if ( total_contact.find(p0) == total_contact.end() &&
	     total_contact.find(p1) == total_contact.end() ) {
	  total_contact.insert(p0);
	}
      }

      //
      // partitioning the set of potential elements
      //

      // number of elements per task
      int numofpairs = total_contact.size() / numofTasks;
      std::set< std::pair<int, int> >::iterator ptc, end;     

      ptc = total_contact.begin();
      std::advance(ptc, rank * numofpairs);

      if ( rank == numofTasks - 1 ) {    
	end = total_contact.end();
      }
      else {
	end = ptc;
	std::advance(end, numofpairs);
      }

      contact_ids.clear();
      while ( ptc != end ) {
	contact_ids.push_back(ptc->first);
	contact_ids.push_back(ptc->second);
	ptc++;
      }

      free(recv_contact);

      return;
    }

    // NOTICE: id_dim and val_dim must be different by an integer ratio
    inline
    void mpi_allGatherData(int numofTasks,
			   int id_dim,
			   int val_dim,
			   vector<int> & ids,			   
			   vector<int> & vals,
			   int & total_counter,
			   int *& recv_ids,
			   int *& recv_data) {
      int local_counter_id = ids.size();
      int *recv_counter_id = (int*)malloc(sizeof(int)*numofTasks);
      if ( recv_counter_id == NULL ) {
	cerr << "unable to allocate memory @MPI_Core.h" << endl;
	assert(false);
      }
      
      MPI_Allgather(&local_counter_id, 1, MPI_INT, recv_counter_id, 1, MPI_INT, MPI_COMM_WORLD);

      int local_counter_data = vals.size();
      int *recv_counter_data = (int*)malloc(sizeof(int)*numofTasks);

      // suppose multi-index data or multi-dimensional data
      int ratio = 1;
      bool isMultiDimensional = false; // 0: multi-index; 1: multi-dimension
      if ( val_dim < id_dim ) {
	ratio = id_dim / val_dim;
      }
      else {
	isMultiDimensional = true;
	ratio = val_dim / id_dim;
      }

      int *displs_id   = (int*)malloc(sizeof(int)*numofTasks);
      int *displs_data = (int*)malloc(sizeof(int)*numofTasks);

      total_counter = 0;
      int total_counter_data = 0;
      
      for ( int i = 0; i < numofTasks; i++ ) {
	displs_id[i]   = total_counter;
	displs_data[i] = total_counter_data;

	if (isMultiDimensional) {
	  recv_counter_data[i] = recv_counter_id[i] * ratio;
	}
	else {
	  recv_counter_data[i] = recv_counter_id[i] / ratio;
	}
	
	total_counter += recv_counter_id[i];
	total_counter_data += recv_counter_data[i];
      }

      int * ptr_recv_ids = NULL;
      ptr_recv_ids = (int*)realloc(recv_ids, sizeof(int)*total_counter);
      if ( ptr_recv_ids != NULL ) {
	recv_ids = ptr_recv_ids;
	MPI_Allgatherv(&ids.front(), local_counter_id, MPI_INT, recv_ids, 
		       recv_counter_id, displs_id, MPI_INT, MPI_COMM_WORLD);
      }
      
      int * ptr_recv_data = NULL;
      ptr_recv_data = (int*)realloc(recv_data, sizeof(int)*total_counter_data);
      if ( ptr_recv_data != NULL ) {
	recv_data = ptr_recv_data;
	MPI_Allgatherv(&vals.front(), local_counter_data, MPI_INT, recv_data, 
		       recv_counter_data, displs_data, MPI_INT, MPI_COMM_WORLD);
      }

      free(displs_data);
      free(displs_id);
      free(recv_counter_data);
      free(recv_counter_id);
      
      return;
    }

    // NOTICE: id_dim and val_dim must be different by an integer ratio
    inline
    void mpi_allGatherData(int numofTasks,
			   int id_dim,
			   int val_dim,
			   vector<int> & ids,			   
			   vector<double> & vals,
			   int & total_counter,
			   int *& recv_ids,
			   double *& recv_data) {
      int local_counter_id = ids.size();
      int *recv_counter_id = (int*)malloc(sizeof(int)*numofTasks);
      if ( recv_counter_id == NULL ) {
	cerr << "unable to allocate memory @MPI_Core.h" << endl;
	assert(false);
      }
      
      MPI_Allgather(&local_counter_id, 1, MPI_INT, recv_counter_id, 1, MPI_INT, MPI_COMM_WORLD);

      int local_counter_data = vals.size();
      int *recv_counter_data = (int*)malloc(sizeof(int)*numofTasks);

      // suppose multi-index data or multi-dimensional data
      int ratio = 1;
      bool isMultiDimensional = false; // 0: multi-index; 1: multi-dimension
      if ( val_dim < id_dim ) {
	ratio = id_dim / val_dim;
      }
      else {
	isMultiDimensional = true;
	ratio = val_dim / id_dim;
      }

      int *displs_id   = (int*)malloc(sizeof(int)*numofTasks);
      int *displs_data = (int*)malloc(sizeof(int)*numofTasks);

      total_counter = 0;
      int total_counter_data = 0;
      
      for ( int i = 0; i < numofTasks; i++ ) {
	displs_id[i]   = total_counter;
	displs_data[i] = total_counter_data;

	if (isMultiDimensional) {
	  recv_counter_data[i] = recv_counter_id[i] * ratio;
	}
	else {
	  recv_counter_data[i] = recv_counter_id[i] / ratio;
	}
	
	total_counter += recv_counter_id[i];
	total_counter_data += recv_counter_data[i];
      }

      int * ptr_recv_ids = NULL;
      ptr_recv_ids = (int*)realloc(recv_ids, sizeof(int)*total_counter);
      if ( ptr_recv_ids != NULL ) {
	recv_ids = ptr_recv_ids;
	MPI_Allgatherv(&ids.front(), local_counter_id, MPI_INT, recv_ids, 
		       recv_counter_id, displs_id, MPI_INT, MPI_COMM_WORLD);
      }
      
      double * ptr_recv_data = NULL;
      ptr_recv_data = (double*)realloc(recv_data, sizeof(double)*total_counter_data);
      if ( ptr_recv_data != NULL ) {
	recv_data = ptr_recv_data;
	MPI_Allgatherv(&vals.front(), local_counter_data, MPI_DOUBLE, recv_data, 
		       recv_counter_data, displs_data, MPI_DOUBLE, MPI_COMM_WORLD);
      }

      free(displs_data);
      free(displs_id);
      free(recv_counter_data);
      free(recv_counter_id);
      
      return;
    }

    // collect data from all the partitions
    inline
    void mpi_allGatherData(int numofTasks,
			   const vector<int> & vals,
			   int *& recv_data,
			   int & total_counter) {
      total_counter = 0;
      int local_counter = vals.size();
      int recv_counter[numofTasks];
      MPI_Allgather(&local_counter, 1, MPI_INT, recv_counter, 1, MPI_INT, MPI_COMM_WORLD);

      int displs[numofTasks];    
      for ( int i = 0; i < numofTasks; i++ ) {
	displs[i]   = total_counter;
	total_counter += recv_counter[i];
      }
      
      recv_data = (int*)malloc(total_counter*sizeof(int));
      MPI_Allgatherv(&vals.front(), local_counter, MPI_INT, recv_data, 
		     recv_counter, displs, MPI_INT, MPI_COMM_WORLD);
      
      return;
    }
	
    // collect data and save them locally (assuming the data can not be duplicated locally)
    template<typename T>
      void mpi_synchronize(int numofTasks,
			   set<T> & data) {
      vector<T> vals;
      for ( typename set<T>::iterator pD = data.begin(); pD != data.end(); pD++ ) {
	vals.push_back(*pD);
      }

      int total_counter = 0;
      T *recv_data = NULL;
      mpi_allGatherData(numofTasks, vals, recv_data, total_counter);

      if ( recv_data != NULL ) {
	for ( int i = 0; i < total_counter; ++i ) {
	  data.insert( recv_data[i] );
	}
	
	free(recv_data);
      }

      return;
    }

    // synchronize single-index and one-dimensional data at cells
    inline
    void mpi_synchronizeCellData(int numofTasks,
				 const map<int, Geometry::Cell*> & ids,
				 map<Geometry::Cell*, double> & celldata,
				 bool assembling = true,
				 map<int, double> * globalData = NULL ) {

      if ( (globalData==NULL) && !assembling ) return;
      
      vector<int> cell_ids;
      vector<double> cell_vals;
      map<int, Geometry::Cell*>::const_iterator pid;
      map<Geometry::Cell*, double>::iterator pCell;

      // collect the local data
      for (pid = ids.begin(); pid != ids.end(); pid++) {
	if ( (pCell=celldata.find(pid->second)) != celldata.end() ) {
	  cell_ids.push_back(pid->first);
	  cell_vals.push_back(pCell->second);
	}
      }
  
      // gather the cell data from each rank
      int total_counter = 0;
      int *recv_ids = NULL;
      double *recv_data = NULL;
      mpi_allGatherData(numofTasks, 1, 1, cell_ids, cell_vals, total_counter, recv_ids, recv_data);
      
      // clean the local data
      cell_ids.clear();
      cell_vals.clear();

      if ( globalData != NULL ) { // store the global data on the local processor
	map<int, double>::iterator pGD;
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pGD=globalData->find(recv_ids[i])) == globalData->end() ) {
	    globalData->insert( make_pair(recv_ids[i], recv_data[i]) );
	  }
	  else if ( assembling ) {
	    pGD->second += recv_data[i];	    
	  }
	}
      }
      else {
	// assemble the local cell data	
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pid=ids.find(recv_ids[i])) != ids.end() ) {
	    celldata.find(pid->second)->second += recv_data[i];
	  }	  
	}
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }

      if ( recv_data != NULL ) {
	free(recv_data);
      }

      return;
    }

    // synchronize single-index and one-dimensional data at given ids
    inline
    void mpi_synchronizeDOFData(int numofTasks,				
				map<int, double> & DOFdata,				
				bool assembling = true,
				map<int, double> * globalData = NULL) {

      if ( (globalData==NULL) && !assembling ) return;
      
      vector<int> dof_ids;
      vector<double> dof_vals;
      map<int, double>::iterator pData;

      // collect the local data
      for ( pData = DOFdata.begin(); pData != DOFdata.end(); pData++ ) {	
	  dof_ids.push_back(pData->first);
	  dof_vals.push_back(pData->second);	
      }

      // gather the data from each rank
      int total_counter = 0;
      int *recv_ids = NULL;
      double *recv_data = NULL;
      mpi_allGatherData(numofTasks, 1, 1, dof_ids, dof_vals, total_counter, recv_ids, recv_data);
      
      // clean the local data
      dof_ids.clear();
      dof_vals.clear();

      if ( globalData != NULL ) { // store the global data on the local processor
	map<int, double>::iterator pGD;
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pGD=globalData->find(recv_ids[i])) == globalData->end() ) {
	    globalData->insert( make_pair(recv_ids[i], recv_data[i]) );
	  }
	  else if ( assembling ) {
	    pGD->second += recv_data[i];	    
	  }
	}
      }
      else {
	// assemble the local dof data
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pData=DOFdata.find(recv_ids[i])) != DOFdata.end() ) {
	    pData->second += recv_data[i];
	  }
	}
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }

      if ( recv_data != NULL ) {
	free(recv_data);
      }

      return;
    }

    // synchronize single-index and one-dimensional data at Set::Manifold::Points
    template<typename T>
    void mpi_synchronizeDOFData(int numofTasks,
				const map<Set::Manifold::Point*, int> & ids,
				const map<int, Set::Manifold::Point*> & dofs,
				map<Set::Manifold::Point*, T> & DOFdata,			
				bool assembling = true,
				map<int, T> * globalData = NULL) {

      if ( (globalData==NULL) && !assembling ) return;
      
      vector<int> dof_ids;
      vector<T> dof_vals;
      map<Set::Manifold::Point*, int>::const_iterator pid;
      map<int, Set::Manifold::Point*>::const_iterator pdof;
      typedef typename map<Set::Manifold::Point*, T>::iterator DOFData_It;
      typedef typename map<int, T>::iterator GD_It;
      DOFData_It pData;

      // collect the local data
      for ( pData = DOFdata.begin(); pData != DOFdata.end(); pData++ ) {
	if ( (pid=ids.find(pData->first)) != ids.end() ) {
	  dof_ids.push_back(pid->second);
	  dof_vals.push_back(pData->second);
	}
      }

      // gather the cell data from each rank
      int total_counter = 0;
      int *recv_ids = NULL;
      T *recv_data = NULL;
      mpi_allGatherData(numofTasks, 1, 1, dof_ids, dof_vals, total_counter, recv_ids, recv_data);
      
      // clean the local data
      dof_ids.clear();
      dof_vals.clear();

      if ( globalData != NULL ) { // store the global data on the local processor
	GD_It pGD;
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pGD=globalData->find(recv_ids[i])) == globalData->end() ) {
	    globalData->insert( make_pair(recv_ids[i], recv_data[i]) );
	  }
	  else if ( assembling ) {
	    pGD->second += recv_data[i];	    
	  }
	}
      }
      else {
	// assemble the local dof data
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pdof=dofs.find(recv_ids[i])) != dofs.end() ) {
	    if ( (pData=DOFdata.find(pdof->second)) != DOFdata.end() ) {
	      pData->second += recv_data[i];
	    }
	    else {
	      DOFdata.insert( make_pair(pdof->second, recv_data[i]) );
	    }
	  }
	}
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }

      if ( recv_data != NULL ) {
	free(recv_data);
      }

      return;
    }


    // synchronize double-index and one-dimensional data at Set::Manifold::Points
    inline
    void mpi_synchronizeDOFData(int numofTasks,
    				const map<Set::Manifold::Point*, int> & ids,
    				const map<int, Set::Manifold::Point*> & dofs,
    				map<std::pair<Set::Manifold::Point*, Set::Manifold::Point*>, double> & DOFdata,
    				bool assembling = true,
    				map<std::pair<int, int>, double> * globalData = NULL) {

      if ( (globalData==NULL) && !assembling ) return;
      
      vector<int> dof_ids;
      vector<double> dof_vals;
      map<Set::Manifold::Point*, int>::const_iterator pid1, pid2;
      map<int, Set::Manifold::Point*>::const_iterator pdof1, pdof2;
      map<std::pair<Set::Manifold::Point*, Set::Manifold::Point*>, double>::iterator pData;

      // collect the local data
      for ( pData = DOFdata.begin(); pData != DOFdata.end(); pData++ ) {
    	const std::pair<Set::Manifold::Point*, Set::Manifold::Point*> & keypair = pData->first;
    	if ( (pid1=ids.find(keypair.first))  != ids.end() &&
    	     (pid2=ids.find(keypair.second)) != ids.end() ) {
    	  dof_ids.push_back(pid1->second);
    	  dof_ids.push_back(pid2->second);
    	  dof_vals.push_back(pData->second);
    	}
      }

      // gather the cell data from each rank
      int total_counter = 0;
      int *recv_ids = NULL;
      double *recv_data = NULL;
      mpi_allGatherData(numofTasks, 2, 1, dof_ids, dof_vals, total_counter, recv_ids, recv_data);
      
      // clean the local data
      dof_ids.clear();
      dof_vals.clear();

      int ratio = 2;
      if ( globalData != NULL ) { // store the global data on the local processor
    	map<std::pair<int,int>, double>::iterator pGD;
    	for ( int i = 0; i < total_counter; i=i+ratio ) {
    	  std::pair<int, int> keypair(recv_ids[i], recv_ids[i+1]);
    	  if ( (pGD=globalData->find(keypair)) == globalData->end() ) {
    	    globalData->insert( make_pair(keypair, recv_data[i/ratio]) );
    	  }
    	  else if ( assembling ) {
    	    pGD->second += recv_data[i/ratio];
    	  }
    	}
      }
      else {
    	// assemble the local dof data
    	for ( int i = 0; i < total_counter; i=i+ratio ) {
    	  if ( (pdof1=dofs.find(recv_ids[i]))   != dofs.end() &&
    	       (pdof2=dofs.find(recv_ids[i+1])) != dofs.end() ) {
	    if ( (pData=DOFdata.find(make_pair(pdof1->second, pdof2->second))) != DOFdata.end() ) {
	      pData->second += recv_data[i/ratio];
	    }
	    else {
	      DOFdata.insert( make_pair(make_pair(pdof1->second, pdof2->second), recv_data[i/ratio]) );
	    }
    	  }
    	}
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }

      if ( recv_data != NULL ) {
	free(recv_data);
      }

      return;
    }
    
    // synchronize single-index and multi-dimensional data at Set::Manifold::Points
    template<typename T>
    void mpi_synchronizeDOFData(int numofTasks,
				unsigned int DIM,
				const map<Set::Manifold::Point*, int> & ids,
				const map<int, Set::Manifold::Point*> & dofs,
				map<Set::Manifold::Point*, T> & DOFdata,	
				int flag = 0, //0: synchronize; 1: assembling (add); 2: dissembling (subtract)
				map<int, T> * globalData = NULL) {

      if ( (globalData==NULL) && (flag==0) ) return;
      
      vector<int> dof_ids;
      vector<double> dof_vals;
      vector<int> dof_dims;
      map<Set::Manifold::Point*, int>::const_iterator pid;
      map<int, Set::Manifold::Point*>::const_iterator pdof;
      typename map<Set::Manifold::Point*, T>::iterator pData;

      // collect the local data
      for ( pData = DOFdata.begin(); pData != DOFdata.end(); pData++ ) {
	if ( (pid=ids.find(pData->first)) != ids.end() ) {
	  int dimloc = pData->second.size();
	  dof_ids.push_back(pid->second);
	  dof_dims.push_back(dimloc);

	  double * phead = pData->second.begin();
	  for ( int k = 0; k < dimloc; ++k ) {
	    dof_vals.push_back(*(phead+k));
	  }

	  // if the data size is less than DIM, then fill it with zeros
	  for ( int k = dimloc; k < DIM; ++k ) {
	    dof_vals.push_back(0.0);
	  }
	}
      }

      // gather the cell data from each rank
      int total_counter = 0;
      int *recv_ids_tmp = NULL;
      int *recv_ids = NULL;
      int *recv_dims = NULL;
      double *recv_data = NULL;

      mpi_allGatherData(numofTasks, 1, 1, dof_ids, dof_dims, total_counter, recv_ids_tmp, recv_dims);
      mpi_allGatherData(numofTasks, 1, DIM, dof_ids, dof_vals, total_counter, recv_ids, recv_data);
      
      /* // clean the local data */
      /* dof_ids.clear(); */
      /* dof_dims.clear(); */
      /* dof_vals.clear(); */

      typename map<int, T>::iterator pGD;     
      if ( globalData != NULL ) { // store the global data on the local processor
	for ( int i = 0; i < total_counter; i++ ) {
	  int dimloc = recv_dims[i];
	  T vloc(dimloc);
	  for ( int k = 0; k < dimloc; ++k ) {
	    vloc[k] = recv_data[i*DIM+k];
	  }
	  
	  if ( (pGD=globalData->find(recv_ids[i])) == globalData->end() ) {
	    globalData->insert( make_pair(recv_ids[i], vloc) );
	  }
	  else if ( flag == 0 ) {
	    pGD->second = vloc;
	  }
	  else if ( flag == 1 ) {
	    for ( int k = 0; k < dimloc; ++k ) {
	      pGD->second[k] += vloc[k];
	    }	    
	  }
	  else if ( flag == 2 ) {
	    for ( int k = 0; k < dimloc; ++k ) {
	      pGD->second[k] -= vloc[k];
	    }
	  }
	}
      }
      else {
	// assemble or dissemble the local dof data
	for ( int i = 0; i < total_counter; i++ ) {
	  if ( (pdof=dofs.find(recv_ids[i])) != dofs.end() ) {
	    int dimloc = recv_dims[i];
	    T vloc(dimloc);
	    for ( int k = 0; k < dimloc; ++k ) {
	      vloc[k] = recv_data[i*DIM+k];
	    }

	    if ( (pData=DOFdata.find(pdof->second)) != DOFdata.end() ) {
	      if ( flag == 1 ) {
		for ( int k = 0; k < dimloc; ++k ) {
		  pData->second[k] += vloc[k];
		}
	      }
	      else {
		for ( int k = 0; k < dimloc; ++k ) {
		  pData->second[k] -= vloc[k]; 
		}
	      }
	    }
	    else {
	      DOFdata.insert( make_pair(pdof->second, vloc) );
	    }
	  }
	}
      }

      if (recv_ids_tmp != NULL) {
	free(recv_ids_tmp);
      }

      if ( recv_dims != NULL ) {
	free(recv_dims);
      }

      if ( recv_ids != NULL ) {
	free(recv_ids);
      }

      if ( recv_data != NULL ) {
	free(recv_data);
      }

      return;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Base class of the m4extreme2MPI interfaces
    //
    //////////////////////////////////////////////////////////////////////////////

    class MPI_Core {

    public:
        typedef Set::Manifold::Point dof_type;
        typedef Set::Euclidean::Orthonormal::Point point_type;
        typedef Set::VectorSpace::Vector vector_type;
        typedef std::map<dof_type*, point_type> pointset_type;
        typedef std::map<dof_type*, vector_type> vectorset_type;
	typedef std::map<Element::MaterialPoint::LocalState*, int> COST_TYPE;

        typedef struct {
	  int _id;
	  double _ke;
	  double _flag;
        } _NODE_ke_type;

	
    public:

        MPI_Core() : _numofCores(0), _pModel(NULL), _idmap(NULL), _print(false), _graph_comm(MPI_COMM_NULL), _dlb_type(1) {
        }

        virtual ~MPI_Core() {
            if (_ofs.is_open()) _ofs.close();

	    // clean up the new dof received from other cores.
	    // however if the dof is a boundary point, it will
	    // be taken cared by the ModelBuilder class.
	    for ( std::set<dof_type*>::iterator px = _xnew.begin();
	    	  px != _xnew.end(); ++px ) {
	      if ( *px != NULL ) delete *px;	    
	    }	    

	    MPI_Type_free(&_ke_type);

	    if ( _graph_comm != MPI_COMM_NULL ) MPI_Comm_free(&_graph_comm);
        }

        MPI_Core(int rank, int size, 
		 MEMPModelBuilder * pModel, 
		 map<dof_type*, int> * idmap, 
		 map<int, dof_type*> * dofmap, 
		 size_t type = 1) :
	  _idmap(idmap), _dofmap(dofmap), 
	  _pModel(pModel), _rank(rank), _numofCores(size), 
	  _print(false), _graph_comm(MPI_COMM_NULL), _dlb_type(type) {

	    assert( _pModel != NULL );	    

            /**
             *  Build a derived datatype consisting of 1 integers and 1 doubles
             */
            int block_lengths[2];
            MPI_Aint offsets[2];
            MPI_Datatype typelist[2];

            // First specify the types
            typelist[0] = MPI_INT;
            typelist[1] = MPI_DOUBLE;

            // Specify the number of elements of each type
            block_lengths[0] = 1;
            block_lengths[1] = 2;

            /**
             * Calculate the displacements of the members
             */
            offsets[0] = 0;
            MPI_Aint lb, extent;
            MPI_Type_get_extent(MPI_INT, &lb, &extent);
            
            offsets[1] = block_lengths[0] * extent;

            // Create the derived type
            MPI_Type_create_struct(2, block_lengths, offsets, typelist, &_ke_type);

            // Commit it so that it can be used
            MPI_Type_commit(&_ke_type);
        }

        virtual void writeRangebox(const char *) const = 0;

#if defined(_M4EXTREME_EIGEN_FRACTURE_)
	virtual void async_free_energy(int len, const double * epsilon) = 0;
	virtual void async_free_energy(int index, double epsilon) = 0;
#endif

	void setdlbtype(size_t type) {
	  _dlb_type = type;
	}

	void update() {
	  _constructCommunicationMap();
	  _synchronizeNodes();
	  return;
	}

	void resetPotentialNodes(const vector<int> & localPNIDs) {
	  int total_counter = 0;
	  int *recv_data = NULL;
	  mpi_allGatherData(_numofCores, localPNIDs, recv_data, total_counter);

	  if ( recv_data != NULL ) {
	    // find the number of partitions on which a node being considered as a potential node
	    map<int, int> nPartitions; //global potential node v.s. number of partitions
	    map<int, int>::iterator pNP;
	    for ( int i = 0; i < total_counter; ++i ) {
	      if ( (pNP=nPartitions.find(recv_data[i])) != nPartitions.end() ) {
		pNP->second++;
	      }
	      else {
		nPartitions.insert( make_pair(recv_data[i], 1) );
	      }
	    }

	    // free the global data
	    free(recv_data);
	    
	    // reset the local acceleration, velocity and mass if a potential node
	    // was synchronized and stored locally
	    double dt = _pModel->_pT->DTime();
	    dt *= 0.5;

	    map<int, dof_type*>::iterator pDOF;
	    map<dof_type*, double>::iterator pM;
	    for ( pNP = nPartitions.begin(); pNP != nPartitions.end(); pNP++ ) {
	      if ( (pDOF=_dofmap->find(pNP->first)) != _dofmap->end() ) {
		dof_type * xloc = pDOF->second;
		if ( (pM=_pModel->_m.find(xloc)) != _pModel->_m.end() ) {
		  int scaling_factor = pNP->second + 1;
		  
		  // the mass was assembled by multiple times
		  pM->second /= scaling_factor;
		  
		  vector_type & aloc = _pModel->_a->find(xloc)->second;
		  vector_type & vloc = _pModel->_v.find(xloc)->second;
		  vloc -= dt * aloc;
		  aloc *= scaling_factor;
		  vloc += dt * aloc;
		}
	      }
	    }
	  }

	  return;
	}

	void rebalance(double criterion, double local_cost) { // criterion: critical relative standard deviation

	    // note:GetComputationalCost function couldn't provide an accurate measurement of the 
	    //      total computational cost of the functions been called
	    //double local_cost = _pModel->GetComputationalCost(costs) + _pModel->_Computational_Cost;	   

	    // gather the cost of all ranks
	    double total_cost[_numofCores];
	    MPI_Allgather(&local_cost, 1, MPI_DOUBLE, total_cost, 1, MPI_DOUBLE, MPI_COMM_WORLD);
	    
	    double mean = accumulate(total_cost, total_cost+_numofCores, 0.0) / _numofCores;
	    double deviation = 0.0;
	    for ( int i = 0; i < _numofCores; ++i ) {
	      //if ( _rank == 0 ) cout << i << "\t" << total_cost[i] << endl;
	      deviation += total_cost[i] * total_cost[i];
	    }
	    deviation /= (double)_numofCores * mean * mean; 
	    deviation -= 1.0;

	    if ( _rank == 0 ) cout << "relative deviation of computational cost:" 
				   << sqrt(deviation) << endl;
	    
	    if ( sqrt(deviation) > criterion ) {

	      vector<COST_TYPE> costs;
	      _pModel->GetComputationalCost(costs);
	      
	      if ( _cm_recv.empty() ) {
		_constructCommunicationMap();
	      }

	      if ( _dlb_type == 1 )
		_constructMigrationPath_I(total_cost, costs);
	      else if ( _dlb_type == 2 )
		_constructMigrationPath_II(total_cost, costs);
	      else
		assert(false);

	      _migrateNodes();

	      for ( int i = 0; i < costs.size(); ++i ) _migrateMpts(i);
	      
	      // recontruct the lumped mass data structure
	      _pModel->ComputeLumpedMass();
	    }

	    return;
	}

	// Instrument tools

	// Get the total strain energy of all the bodies in the computational domain
	virtual double getStrainEnergy() const {
	  double wloc = _pModel->GetStrainEnergy();	  
	  double *recv = NULL;
	  if ( _rank == 0 ) {
	    recv = new double[_numofCores];
	  }

	  MPI_Gather(&wloc, 1, MPI_DOUBLE, recv, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	  if ( _rank == 0 ) {
	    double wtotal = std::accumulate(recv, recv+_numofCores, 0.);
	    delete[] recv;
	    return wtotal;
	  }
	  else {
	    return 0.;
	  }
	}

	// Get the total kinetic energy of all the nodes in the computational domain
        virtual double getKineticEnergy() const {

	  double ke = 0.0;

	  int sendcount = _pModel->_m.size();
	  int recvcount[_numofCores];
	  MPI_Allgather(&sendcount, 1, MPI_INT, recvcount, 1, MPI_INT, MPI_COMM_WORLD);

	  _NODE_ke_type * kn = new _NODE_ke_type[sendcount];
	  int count = 0;
	  for (std::map<dof_type*, double>::iterator pm = _pModel->_m.begin();
	       pm != _pModel->_m.end(); ++pm, ++count) {
	    const vector_type & vloc = _pModel->_v.find(pm->first)->second;
	    kn[count]._id = _idmap->find(pm->first)->second;
	    kn[count]._ke = 0.5 * pm->second * vloc(vloc);
	  }

	  int displs[_numofCores];
	  int totalrecv = 0;
	  for (int i = 0; i < _numofCores; ++i) {
	    displs[i]  = totalrecv;
	    totalrecv += recvcount[i];
	  }
	  
	  _NODE_ke_type * recv = new _NODE_ke_type[totalrecv];
	  MPI_Gatherv(kn, sendcount, _ke_type, recv, recvcount, displs, _ke_type, 0, MPI_COMM_WORLD);
	  
	  // assemble total kinetic energy
	  std::set<int> ids;
	  
	  if (_rank == 0) {
	    int startI, recvcountI;
	    for (int i = 0; i < _numofCores; i++) {
	      startI = displs[i];
	      recvcountI = recvcount[i];
	      for (int j = 0; j < recvcountI; j++) {
		const _NODE_ke_type & recvJ = recv[startI + j];
		if (ids.find(recvJ._id) == ids.end()) {
		  ids.insert(recvJ._id);
		  ke += recvJ._ke;
		} else {
		  continue;
		}
	      }
	    }
	  }
	  
	  // clean up
	  delete [] recv;
	  delete [] kn;
	  
	  return ke;
        }
	
	set<int> setInitialNodes(const std::set<Geometry::Cell*> & initial_cells) {
	  std::vector<int> initIds;
	  std::map<Geometry::Cell*, dof_type*>::iterator pBind;
	  for ( std::set<Geometry::Cell*>::const_iterator it = initial_cells.begin();
		it != initial_cells.end(); it++ ) {
	    pBind = _pModel->_Bind.find(*it);
	    if ( pBind != _pModel->_Bind.end() ) {
	      initIds.push_back( _idmap->find(pBind->second)->second );
	    }
	    else {
	      assert(false);
	    }	    
	  }

	  int sendcount = initIds.size();
	  int recvcount[_numofCores];
	  MPI_Allgather(&sendcount, 1, MPI_INT, recvcount, 1, MPI_INT, MPI_COMM_WORLD);
  
	  int displs[_numofCores];
	  int totalrecv = 0;
	  for (int i = 0; i < _numofCores; ++i) {
	    displs[i]  = totalrecv;
	    totalrecv += recvcount[i];
	  }
	  
	  int * recv = new int[totalrecv];
	  MPI_Allgatherv(&initIds.front(), sendcount, MPI_INT, recv, recvcount, displs, MPI_INT, MPI_COMM_WORLD);

	  std::set<int> totalIds;
	  for ( int i = 0; i < totalrecv; ++i ) {
	    _id_initial.insert(recv[i]);
	    totalIds.insert(recv[i]);
	  }

	  return totalIds;
	}

	const void getInitialNodes(std::set<dof_type*> & x_initial) const{
	  x_initial.clear();
	  for ( std::map<int, dof_type*>::const_iterator it = _dofmap->begin();
		it != _dofmap->end(); it++ ) {
	    if ( _id_initial.find(it->first) != _id_initial.end() ) {
	      x_initial.insert(it->second);
	    }	    
	  }

	  return;
	}
	
        void print() {
            if (_print && _ofs.is_open()) {
                _ofs.flush();
            }
        }

	void setprint(bool print) {
	  _print = print;
	  if (_print && !_ofs.is_open() ) {
	    char buf[16];
	    sprintf(buf, "./data/dump_%d.txt", _rank);
	    _ofs.open(buf);
	    if (_ofs.fail()) assert(false);
	  }
	}
 
	void GetGlobalMass(const vector<int> & localID,
			   map<int, double> & GlobalMass) {
	  int numofDetachedNodes = localID.size();
	  int *recv_detachedNodes_num = (int*)malloc(sizeof(int)*_numofCores);
	  MPI_Allgather(&numofDetachedNodes, 1, MPI_INT, recv_detachedNodes_num, 
			1, MPI_INT, MPI_COMM_WORLD);

	  int totalnumofDetachedNodes = 0;
	  vector<int> displs_detach_id;
	  for (int count = 0; count < _numofCores; ++count) {
	    displs_detach_id.push_back(totalnumofDetachedNodes);
	    totalnumofDetachedNodes += recv_detachedNodes_num[count];
	  }
	  
	  if ( totalnumofDetachedNodes != 0 ) {
	    int *totalDetachID = (int*)malloc(sizeof(int)*totalnumofDetachedNodes);
	    MPI_Allgatherv(&localID.front(), numofDetachedNodes, MPI_INT, 
			   totalDetachID, recv_detachedNodes_num, 
			   &displs_detach_id.front(), MPI_INT, MPI_COMM_WORLD);

	    map<int, dof_type *>::const_iterator pDOF;
	    map<dof_type *, double>::const_iterator pM;
	    
	    map<int, double> DetachedMass;
	    const map<dof_type *, double> & mass = _pModel->_m;
	    for (int count = 0; count < totalnumofDetachedNodes; ++count) {
	      pDOF = _dofmap->find(totalDetachID[count]);
	      if (pDOF != _dofmap->end() ) {
		pM = mass.find(pDOF->second);
		if ( pM != mass.end() ) {		  
		  if (DetachedMass.find(totalDetachID[count]) == DetachedMass.end()) {
		    DetachedMass.insert( make_pair(totalDetachID[count], pM->second));
		  }
		}
	      }
	    }
	    
	    mpi_synchronizeDOFData(_numofCores, DetachedMass, true, &GlobalMass);
	    free(totalDetachID);
	  }	  
	  
	  free(recv_detachedNodes_num);

	  return;
	}

	void setDetachedNodes(double critical_mass) {

	  set<Set::Manifold::Point *>::iterator px;
	  map<int, dof_type *>::const_iterator pDOF;
	  map<dof_type *, double>::iterator pM;

	  map<dof_type *, double> & mass = _pModel->_m;

	  vector<int> localDetachID;	  
	  set<Set::Manifold::Point *> & localDetachedNodes = _pModel->GetDetachedNodes();

	  // collect local detached nodes
	  for (px = _pModel->_x.begin(); px != _pModel->_x.end(); px++) {
	    pM = mass.find(*px);
	    if ( pM == mass.end() ) {
	      localDetachedNodes.insert(*px);
	      localDetachID.push_back(_idmap->find(*px)->second);
	    }
	    else if ( pM->second < critical_mass ) {
	      localDetachedNodes.insert(*px);
	      localDetachID.push_back(_idmap->find(*px)->second);
	    }
	  }

	  // synchronize the mass of detached nodes from all processors
	  map<int, double> GlobalMass;
	  GetGlobalMass(localDetachID, GlobalMass);
	  for( map<int, double>::iterator pGM = GlobalMass.begin(); 
	       pGM != GlobalMass.end(); pGM++) {
	    pDOF = _dofmap->find(pGM->first);
	    if (pDOF != _dofmap->end()) {		
	      if (pGM->second > critical_mass) {
		localDetachedNodes.erase(pDOF->second);
	      }
	      else {
		if ( (pM=mass.find(pDOF->second)) != mass.end() ) {
		  pM->second = 0.0;
		}
	      }
	    }
	  }
	  
	  return;
	}

    public:
	set<dof_type *> _xnew; // new dof received from other cores

    protected:
	virtual void _synchronizeNodes() = 0;
	virtual void _constructCommunicationMap() = 0;
	virtual void _constructMigrationPath_I(double *, vector<COST_TYPE> &){}
	virtual void _constructMigrationPath_II(double *, vector<COST_TYPE> &){}
	virtual void _migrateNodes(){}
	virtual void _migrateMpts(int){}

    protected:
	bool _print;
        int _rank, _numofCores, _numofNodes, _numofMpts, _numofOverlaps;
	MPI_Comm _graph_comm;

        map<dof_type*, int> * _idmap;
        map<int, dof_type*> * _dofmap;
	map<int, vector<dof_type*> > _shadowNodes;
	map<int, vector< vector<Element::MaterialPoint::LocalState*> > > _immigrants;
        map<int, int> _cm_recv;
        set<int> _sharedNodes;

        MEMPModelBuilder * _pModel;
	set<int> _id_initial;

        MPI_Datatype _message_type;
        MPI_Datatype _ke_type;
        vector<int> _overlaps;
	
	size_t _dlb_type;

        ofstream _ofs;
    }; // end_of_MPI_Core

} // end_of_m4extreme

#endif
