// TMModel.h: interface for the TMModel class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details

#if !defined(MODEL_TMMODEL_H__INCLUED_)
#define MODEL_TMMODEL_H__INCLUED_

#pragma once
#include "Model/MaterialPoint/MaterialPoint.h"

namespace Model
{
  namespace MaterialPoint
  {
    namespace TMCoupling //ThermoMechanical Coupling
    {
      class LocalState;
      template <unsigned int> class Energy;
      template <unsigned int> class Jet;

      // Class LocalState

      class LocalState : public MaterialPoint::LocalState
	{
	  friend class Energy<0>;
	  friend class Energy<1>;
	  friend class Energy<2>;

	public:
		
	  typedef set<Set::Manifold::Point *> domain_type;
		
	  LocalState();
	  virtual ~LocalState();
	  Model::LocalState *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
	  LocalState(unsigned int, Clock *, vector<Element::LocalState *> *,
		     map<Set::Manifold::Point *, Set::Manifold::Map *> *,
		     map<Set::Manifold::Point *, Set::Manifold::TMap *> *,
		     bool isMultiBodies = false);
#else
	  LocalState(unsigned int, Clock *, set<Element::LocalState *> *,
		     map<Set::Manifold::Point *, Set::Manifold::Map *> *,
		     map<Set::Manifold::Point *, Set::Manifold::TMap *> *,
		     bool isMultiBodies = false);
#endif

	  LocalState(const LocalState &rhs);

	  // Mechanical Forces
	  virtual void Embed(const domain_type &,
			     map <Set::Manifold::Point *, Set::Euclidean::Orthonormal::Point> &);
	  virtual void Embed(const domain_type &,
			     map <Set::Manifold::Point *, Set::VectorSpace::Vector> &);
	  virtual void Embed(const map <Set::Manifold::Point *, Set::VectorSpace::Vector> &,
			     map <Set::Manifold::Point *, Set::VectorSpace::Vector> &);
	  
	  // Mechanical Forces		
	  virtual void Submerge(const map<Set::Manifold::Point *, Set::VectorSpace::Vector> &,
				map<Set::Manifold::Point *, Set::VectorSpace::Vector> &);
				
	  // Thermal forces
	  virtual void Embed(const domain_type &,
			     map <Set::Manifold::Point *, double> &);

	  // Thermal forces
	  virtual void Submerge(const map<Set::Manifold::Point *, double> &,
				map<Set::Manifold::Point *, double> &);
				
	  virtual void Submerge( const map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> &dfemb,
				 map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> &df);

	  virtual void UpdateInternalVariables(const set<Set::Manifold::Point *> &);

	  //
	  // template functions to avoid specific type for the container and data type 
	  //
	  template<typename InputIterator, typename point_type>
	    void Embed(InputIterator first, InputIterator last,
		       map <Set::Manifold::Point *, point_type> &yemb) {
	    typedef map <Set::Manifold::Point *, point_type> yemb_type;
	    
	    yemb.clear();
	    map<Set::Manifold::Point *, Set::Manifold::Map *>::iterator pEmb;
	    
	    while ( first != last ) {
	      Set::Manifold::Point * yloc = *first;
	      pEmb = Emb->find(yloc);
	      
	      if (pEmb != Emb->end()) {   
		if (pEmb->second == 0) {
		  const point_type * ploc = dynamic_cast<point_type*> (yloc);
		  yemb.insert( make_pair(yloc, point_type(SPATIAL_DIM, ploc->begin())) );
		} else {
		  const point_type & ploc = 
		    dynamic_cast<const point_type &> ((*pEmb->second)(*yloc));
		  yemb.insert( make_pair(yloc, point_type(SPATIAL_DIM, ploc.begin())) );
		}
	      } else {
		assert(false);
	      }
	      
	      ++first;
	    }
	    
	    return;
	  }

	  template<typename InputIterator>
	    void Embed(InputIterator first, InputIterator last,
		       map <Set::Manifold::Point *, double> &yemb) {
	    typedef map <Set::Manifold::Point *, double> yemb_type;
	    
	    yemb.clear();
	    map<Set::Manifold::Point *, Set::Manifold::Map *>::iterator pEmb;
	    
	    while ( first != last ) {
	      Set::Manifold::Point * yloc = *first;
	      pEmb = Emb->find(yloc);
	      
	      if (pEmb != Emb->end()) {   
		if (pEmb->second == 0) {
		  const Set::Euclidean::Orthonormal::Point * ploc = 
		    dynamic_cast<Set::Euclidean::Orthonormal::Point*> (yloc);
		  yemb.insert( yemb_type::value_type(yloc,  *(ploc->end()-1)) );
		} else {
		  Set::Euclidean::Orthonormal::Point & ploc =
		    (Set::Euclidean::Orthonormal::Point &)(*pEmb->second)((*yloc));
		  yemb.insert( yemb_type::value_type(yloc,  *(ploc.end()-1)) );
		}
	      } else {
		assert(false);
	      }
	      
	      ++first;
	    }
	    
	    return;
	  }

	private:
	  unsigned int SPATIAL_DIM;

#if defined(_M4EXTREME_THREAD_POOL)
        private:
	  static void * _reset(void *);
	  static void * _embed_mechanical(void *);
	  static void * _embed_thermal(void *);
	  static void * _submerge_mechanical(void *);
	  static void * _submerge_thermal(void *);
	  static void * _dsubmerge_thermal(void *);

	  typedef struct {
	    std::vector<Element::LocalState*> * _pELS;
	    const map<Set::Manifold::Point *, double> * _temb;
	  }_eureka_reset_arg;

	  typedef struct {
	    unsigned int * _dim;
	    const set<Set::Manifold::Point *> * _y;
	    map<Set::Manifold::Point *, Set::VectorSpace::Vector> * _yemb;
	    map<Set::Manifold::Point *, Set::Manifold::Map *> * _Emb;
	  } _eureka_emb_mechanical_arg;

	  typedef struct {
	    unsigned int * _dim;
	    const map<Set::Manifold::Point *, Set::VectorSpace::Vector> * _femb;
	    map<Set::Manifold::Point *, Set::VectorSpace::Vector> * _f;
	    map<Set::Manifold::Point *, Set::Manifold::TMap *> * _DEmb;
	  } _eureka_sub_mechanical_arg;

	  typedef struct {
	    const set<Set::Manifold::Point *> * _y;
	    map<Set::Manifold::Point *, double> * _yemb;
	    map<Set::Manifold::Point *, Set::Manifold::Map *> * _Emb;
	  } _eureka_emb_thermal_arg;

	  typedef struct {
	    const  map<Set::Manifold::Point *, double> * _femb;
	    map<Set::Manifold::Point *, double> * _f;
	    map<Set::Manifold::Point *, Set::Manifold::TMap *> * _DEmb;
	  } _eureka_sub_thermal_arg;

	  typedef struct {
	    const map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> * _dfemb;
	    map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> * _df;
	    map<Set::Manifold::Point *, Set::Manifold::TMap *> * _DEmb;
	  } _eureka_dsub_thermal_arg;

	  static ost::Mutex _lock;
#endif
	};

      // Class Energy<p>

      template<unsigned int p> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<0>
      //////////////////////////////////////////////////////////////////////

      template<> class Energy <0> : public Model::MaterialPoint::Energy <0> 
	{

	public:

	  Energy();
	  virtual ~Energy();
	  Model::Energy < 0 > *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
	    Energy(LocalState *, vector<Element::Energy < 0 > *> *);
#else
            Energy(LocalState *, set<Element::Energy < 0 > *> *);
#endif

	  Energy(const Energy < 0 > &);
	  Energy < 0 > & operator =(const Energy < 0 > &);
	  virtual range_type operator () (const domain_type &) const { assert(false); }
	private:
	  LocalState *LS;
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template<> class Energy<1> : public Model::MaterialPoint::Energy<1>
	{

	public: 

	  typedef Model::MaterialPoint::TMCoupling::Energy<2> tangent_type;
	  typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	  typedef map<Set::Manifold::Point *, double> force_type;
	  typedef set<Set::Manifold::Point *> domain_type;
	  typedef map<Set::Manifold::Point *, double> range_type;

	  Energy();
	  virtual ~Energy();
	  Model::Energy<1> *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
	  Energy(LocalState *, vector<Element::Energy < 1 > *> *);
#else
	  Energy(LocalState *, set<Element::Energy < 1 > *> *);
#endif

	  Energy(const Energy<1> &);
		
	  void operator () (const domain_type &, range_type &) const;

	protected:
	  LocalState *LS;  

	private:
#if defined(_M4EXTREME_THREAD_POOL)
	  static void * _computeForce(void *);

	  typedef struct {	      
	    const std::vector<Element::Energy < 1 > *> * _pDE;
	    const vector_type * _nodeset;
	    const force_type * _tempset;
	    std::vector<force_type> * _forces;
	  } _eureka_thread_arg;
#endif    
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template<> class Energy<2> : public Model::MaterialPoint::Energy<2>
	{
	public: 

	  typedef Model::MaterialPoint::Energy<3> tangent_type;
	  typedef map<Set::Manifold::Point *, double> scalar_type;
	  typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	  typedef set<Set::Manifold::Point *> domain_type;
	  typedef map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> range_type;

	  Energy();
	  virtual ~Energy();
	  Model::Energy<2> *Clone() const;

#if defined(_M4EXTREME_THREAD_POOL)
	  Energy(LocalState *, vector<Element::Energy < 2 > *> *);
#else
	  Energy(LocalState *, set<Element::Energy < 2 > *> *);
#endif

	  Energy(const Energy<2> &);
        
	  void operator () (const domain_type &y, range_type & DF) const;

	private:
	  LocalState *LS;

#if defined(_M4EXTREME_THREAD_POOL)
	  static void * _computeStiffness(void *);
	  typedef struct {	      
	    const std::vector<Element::Energy < 2 > *> * _pDDE;
	    const vector_type * _nodeset;
	    const scalar_type * _tempset;
	    std::vector<range_type> * _stiffness;
	  } _eureka_thread_arg;
#endif    
	};

    }
  }
}

#endif
