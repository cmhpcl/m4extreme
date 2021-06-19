// TMSemiImplicitDynamics.h: interface for the TMSemiImplicitDynamics class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(SOLVER_TMSEMIIMPLICITDYNAMICS__INCLUDED_)
#define SOLVER_TMSEMIIMPLICITDYNAMICS__INCLUDED_

#pragma once

#include <map>
#include "Solver/Solver.h"
#include "Clock/Clock.h"
#include "Model/Model.h"
#include "Set/Manifold/Manifold.h"
#include "Model/LumpedMass/LumpedMass.h"

#ifdef _M4EXTREME_MPI_
#include "mpi/MPI_Core.h"
#include "Solver/Linear/SuperLU/SuperLUMPI.h"
#else
#include "Solver/Linear/SuperLU/SuperLU.h"
#endif

using namespace std;

namespace Solver
{
  class TMSemiImplicitDynamics : public Propagator  
  {
  public:
    typedef map<Set::Manifold::Point *, double> scalar_type;
    typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
    
    TMSemiImplicitDynamics();
    virtual ~TMSemiImplicitDynamics();

#ifdef _M4EXTREME_MPI_
    TMSemiImplicitDynamics(unsigned int,
			   Clock *,
			   Model::LocalState *, 
			   Model::Energy<1> *, 
			   Model::Energy<2> *, 
			   scalar_type *, 
			   set<Set::Manifold::Point *> *,
			   vector_type *,
			   int,
			   int,
			   int,
			   int,
			   const map<int,Set::Manifold::Point*> *, 
			   const map<Set::Manifold::Point*, int> *,			   
			   const double=0.5);
#else
    TMSemiImplicitDynamics(unsigned int,
			   Clock *,
			   Model::LocalState *, 
			   Model::Energy<1> *, 
			   Model::Energy<2> *, 
			   scalar_type *, 
			   set<Set::Manifold::Point *> *,
			   vector_type *,
			   const double=0.5,
			   bool SolvingTemperature_=true);

    TMSemiImplicitDynamics(unsigned int,
			   Clock *,
			   Model::LocalState *, 
			   Model::Energy<1> *, 
			   Model::Energy<2> *, 
			   scalar_type *, 
			   set<Set::Manifold::Point *> *,
			   vector_type *,
			   const vector_type &,
			   const double=0.5,
			   bool SolvingTemperature_=true);
#endif

    bool & SetPrint();
    void SetTolerance(double);
    void SetDynamicRelaxation(double);
    void SetMaxIncrement(double);
    void SetMaxIterations(unsigned int);
    void SetDetachedNodes(const set<Set::Manifold::Point *> *);
    void UpdateMass(map<Set::Manifold::Point*, double> *);
    vector_type & getA() { return a;}
    const vector_type & getA() const { return a;}
    const vector_type & getF() const { return f;}
    const vector<double> & GetNodalDT() const;
    const double & GetMaxDT() const;

    void operator ++ ();
    void DynamicRelaxation();
    double SolvingTemperature ();

  private:
    unsigned int _DIM;
    double GamOld;
    double GamNew;
    bool Print; 
    Clock *T;
    Model::LocalState *LS;
    Model::Energy<1> *DE;
    Model::Energy<2> *DDE;
    scalar_type *m;
    set<Set::Manifold::Point *> *x;
    vector_type *v;
    vector_type a;
    vector_type f;

    double _TOLERANCE;
    double _DRFactor; // dynamic relaxation factor
    double _DTC;
    unsigned int _MAXIT;
    const set<Set::Manifold::Point *> *_detached_nodes;
    vector<double> _x_DTk;
    double _maxDT;

#ifdef _M4EXTREME_MPI_
    int nprow;
    int npcol;
    const map<int, Set::Manifold::Point*> * PointID;
    const map<Set::Manifold::Point*, int> * DOF;
    int rank;
    int numofTasks;
    set<int> Temperature_BC;

    typedef struct {
      int _row;
      int _column;
      double _value;
      double _flag;
    }_DQDT_DATA_TYPE;

    MPI_Datatype _message_type;
#else
    Solver::Linear::System<Set::Manifold::Point*> * _ImplicitSolver;
    vector<Set::Manifold::Point *> _Keys;
#endif

  private:

#ifndef _M4EXTREME_MPI_
    void _initializeImplicitSolver();
#endif

    TMSemiImplicitDynamics(TMSemiImplicitDynamics &);
    void operator=(TMSemiImplicitDynamics &);       
  };

}

#endif // !defined(SOLVER_TMSEMIIMPLICITDYNAMICS__INCLUDED_)
