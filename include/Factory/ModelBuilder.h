// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(M4EXTREME_MODELBUILDER_H__INCLUDED_)
#define M4EXTREME_MODELBUILDER_H__INCLUDED_

#include <limits>
#include <numeric>
#include <iostream>
#include <vector>
#include "ElementBuilder.h"
#include "Potential/PotLib.h"
#include "Utils/Utils.h"
#include "Clock/Clock.h"
#include "Model/ModLib.h"

////////////////////////////////////////////////////////////////////////////
//  Global interfaces
////////////////////////////////////////////////////////////////////////////
#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
int
gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
}
#endif

std::ostream & operator <<(std::ostream & os, const std::vector<double> & v);

namespace m4extreme {

  class ModelBuilder;

  // global variables

  // global functions
  void ComputeNodalArea(int d, // dimension of the problem
			Geometry::Cell * node,   // current node
			Geometry::Cell * face,   // cobounday of the node
			Geometry::Cell * volume, // co-co-boundary of the node
			const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> & P, // location of all the nodes
			Set::VectorSpace::Vector & area); // return the nodal area
  void FindNodalArea(const set<Geometry::Cell*> & SD,
		     const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> & P,
		     map<Geometry::Cell *, double> & areas);
  void FindNodalNormal(const set<Geometry::Cell*> & SD,
		       const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> & P,
		       map<Geometry::Cell *, Set::VectorSpace::Vector> & normals);
  void FindSurfaces( int d,
		     const set<Geometry::Cell*> & vertices,
		     set<Geometry::Cell*> & surfaces,
		     set<Geometry::Cell*> * volume=NULL);

  void FindSurfaces( const set<Geometry::Cell*> &,
		     const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> &,
		     map< Geometry::Cell*, vector<Geometry::Cell*> > &,
		     map< Geometry::Cell*, vector<Set::VectorSpace::Vector> > &);

  void FindSurfaces( const vector<Geometry::Cell*> &, 
		     const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> &,
		     vector<Set::VectorSpace::Vector> &);
 
  void FindInterfaces(int d,
		     const set<Geometry::Cell*> & current_domain,
		     const set<Geometry::Cell*> & vertices,
		     set<Geometry::Cell*> & surfaces);  
  void FindInterfaces(int d,
		     const set<Geometry::Cell*> & current_domain,
		     const set<Geometry::Cell*> & vertices,
		     const map<Geometry::Cell *, Set::Euclidean::Orthonormal::Point> & P,
		      map<Geometry::Cell*, Set::VectorSpace::Vector> & surfaces);  
  void FindNeighborElements(int FLAG,
			    const set<Geometry::Cell*> & SD,
			    vector< set<Geometry::Cell*> >& SDvec);
  Set::VectorSpace::Vector CrossProduct3D(const Set::VectorSpace::Vector & u,
					  const Set::VectorSpace::Vector & v);
  double CrossProduct2D(const Set::VectorSpace::Vector & u,
			const Set::VectorSpace::Vector & v);
  bool IsNAN(double*, size_t);

  template <typename T>
    void cleanContainer(T & t) {
    typename T::iterator it;
    for (it = t.begin(); it != t.end(); it++) {
      if ( *it != NULL) {
	delete *it;
      }
    }
    
    t.clear();
    
    return;
  }

  //
  // functors
  //
  struct ScalarFunc {
    virtual double operator() (Element::MaterialPoint::LocalState *) = 0;
  };
}


////////////////////////////////////////////////////////////////////////////
//  Model Builder interfaces
////////////////////////////////////////////////////////////////////////////

namespace m4extreme {

    ////////////////////////////////////////////////////////////////////////////
    // Base class interfaces
    ////////////////////////////////////////////////////////////////////////////

    class ModelBuilder {
    public:

        typedef Set::Manifold::Point dof_type;
        typedef Set::Euclidean::Orthonormal::Point point_type;
        typedef Set::VectorSpace::Vector vector_type;
        typedef std::set<dof_type*> dofset_type;


        ModelBuilder(Clock *, unsigned int);
        virtual ~ModelBuilder();

	void InitializeA(std::map<dof_type *, vector_type> * a_);

        virtual void InitializeDof(std::map<Geometry::Cell*, point_type> &,
                std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
                set<Geometry::Cell*> * = NULL);	

	virtual void InitializeNewDof(const vector<Geometry::Cell*> &,
				      std::map<Geometry::Cell*, point_type> &,
				      std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
				      set<Geometry::Cell*> * = NULL);

        virtual void InsertBody(Geometry::CellComplex &,
				std::map<Geometry::Cell*, point_type> &,
				ElementBuilder *,
				int numofQP,
				Material::Builder *,
				std::map<Geometry::Cell*, Set::VectorSpace::Hom*> * = NULL,
				set<Geometry::Cell*> * = NULL,
				const double * = NULL,
				bool = true);
        virtual void InsertTractionBoundary(const set<Geometry::Cell*> &,
                                            const map<Geometry::Cell *, Set::Euclidean::Orthon\
ormal::Point> &,
                                            Potential::Field::TimeDependent::TimeFunction *);
        virtual void InsertTractionBoundary(const map<Geometry::Cell*, double> &,
                                            const map<Geometry::Cell *, Set::Euclidean::Orthon\
ormal::Point> &,
                                            Potential::Field::TimeDependent::TimeFunction *);                
        virtual void InsertPotentialField(const set<Geometry::Cell*> &,
					  Potential::Field::Builder *);
	virtual void InsertPotentialTwoBody(const set<Geometry::Cell*> &,
					    const set<Geometry::Cell*> &,
					    Potential::Directional::Builder *,
					    bool=true);
        virtual void InsertPotentialTwoBody(dof_type *,
					    dof_type *,
					    Potential::Directional::Builder *,
					    bool=true); 
        virtual void InsertContactField(Geometry::SubComplex &,
					set<Geometry::Cell*> &,
					set<Geometry::Cell*> &,
					Potential::Contact::Data *);
	
        void SetPrint(bool);
	void SetDetachMode(bool);

        virtual void CreateModel();
        virtual void ComputeLumpedMass();

        virtual void Reset();
        virtual double GetStrainEnergy() const = 0;
        virtual double GetKineticEnergy() const = 0;
        virtual double GetStrainEnergy(int) const = 0;
        virtual double GetKineticEnergy(int) const = 0;
        virtual void GetMomentum(std::map<Geometry::Cell*, point_type> &,
				 Set::VectorSpace::Vector &) const;
        virtual void GetMomentum(int,
				 Set::VectorSpace::Vector & ) const;
        virtual void GetAngularMomentum(std::map<Geometry::Cell*, point_type> &,
                const point_type &,
                Set::VectorSpace::Vector &) const;
        virtual double GetAngularMomentum(std::map<Geometry::Cell*, point_type> &,
                const point_type &) const;
        virtual double GetTotalMass(std::map<Geometry::Cell*, point_type> &) const;

        virtual map<Geometry::Cell *, Potential::OneBody::LocalState *> & GetPOBELS(int=0);
	virtual map<dof_type*, map<dof_type*, Potential::TwoBody::LocalState*> > & GetPTBELS();

    protected:
        void _createMaterial(Material::Builder *, int numofQP);

        virtual void _initializeMaterial() = 0;
        virtual void _initializeSingleElement(Geometry::Cell *,
                std::map<Geometry::Cell*, point_type> &,
                ElementBuilder *,
                const double * = NULL) = 0;

    private:
        ModelBuilder(const ModelBuilder &);
        ModelBuilder & operator =(const ModelBuilder &);

    public:
        static unsigned int _DIM;
	int _Factorial;
	double _Computational_Cost;

        dofset_type _x;
        std::map<dof_type *, vector_type> _v;
        std::map<dof_type *, vector_type> *_a;
        std::map<dof_type *, double> _m;

        std::map<Geometry::Cell*, dof_type *> _Bind;

        Clock * _pT;
        Model::LumpedMass * _pLM;
        Model::LocalState * _pLS;
        Model::Energy < 0 > * _pE;
        Model::Energy < 1 > * _pDE;
        Model::Energy < 2 > * _pDDE;
        std::map<dof_type *, Set::Manifold::Map *> _Emb;
        std::map<dof_type *, Set::Manifold::TMap *> _DEmb;

    protected:
        bool _print;
	bool _ballistic;

	vector<Material::LocalState *>   _MLSvec;
        vector<Material::Energy < 0 > *> _MWvec;
        vector<Material::Energy < 1 > *> _MDWvec;
        vector<Material::Energy < 2 > *> _MDDWvec;

        Material::LocalState * _pMLS;
        Material::Energy < 0 > * _pW;
        Material::Energy < 1 > * _pDW;
        Material::Energy < 2 > * _pDDW;

        std::vector<Material::Factory *> _MatFac;
	//std::vector<Potential::Field::Factory *> _PotFac;

        std::set<Material::LocalState *> _MLS;
        std::set<Material::Energy < 0 > *> _ME;
        std::set<Material::Energy < 1 > *> _MDE;
        std::set<Material::Energy < 2 > *> _MDDE;
        std::set<Material::Jet < 0 > * > _MJ;
        std::set<Material::Jet < 1 > * > _MDJ;

        std::set<Element::LocalState *> _ELS;
        std::set<Element::Energy < 0 > *> _EE;
        std::set<Element::Energy < 1 > *> _EDE;
        std::set<Element::Energy < 2 > *> _EDDE;
        std::set<Element::LumpedMass *> _ELM;
        map<Geometry::Cell*, Potential::OneBody::LocalState*> _POBELS;
        map<Geometry::Cell*, Potential::OneBody::LocalState*> _POBELS_Traction;
	map<dof_type*, map<dof_type*, Potential::TwoBody::LocalState*> > _PTBELS;
	std::vector<dof_type*> _boundary_points;
    };    
}

#endif //M4EXTREME_MODELBUILDER_H__INCLUDED_
