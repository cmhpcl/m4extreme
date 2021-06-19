// ReactiveElement.h: interface for the ReactiveElement class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(ELEMENT_MATERIALPOINT_REACTIVE_H__INCLUDED_)
#define ELEMENT_MATERIALPOINT_REACTIVE_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Element/MaterialPoint/MaterialPoint.h"

using namespace std;

namespace Element
{
  namespace MaterialPoint
  {
    namespace Reactive // detonation and growth
    {
      class Data;
      class LocalState;
      template <unsigned int> class Energy;	
	
      //////////////////////////////////////////////////////////////////////
      // Class Data
      //////////////////////////////////////////////////////////////////////
      class Data {

	friend class LocalState;

      public:
        Data(double Jc_, const vector<double> & C_):C(C_), Jc(Jc_) {}
	~Data(){}
      private:
	// constants in the three terms reaction model
	// dF/dt = a1 * (1-F)^a2 * (rho/rho0 - 1 - a3)^a4 + a5*(1-F)^a6*F^a7*p^a8
	//       + a9*(1-F)^a10 * F^a11 * p^a12
	// vector<double>: F_ig, F_G1, F_G2, a1, ..., a12
	vector<double> C;
	double Jc;
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Element::MaterialPoint::LocalState
      {
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;

      public: 

	typedef map<Set::Manifold::Point *, double> domain_type;
	typedef vector<Set::VectorSpace::Vector> range_type;

	LocalState();
	virtual ~LocalState();
	Element::LocalState *Clone() const;
	LocalState(Data *, Clock *, double,
                   MaterialPoint::Data *,
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(Data *, Clock *, double,
                   MaterialPoint::Data *, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(const LocalState &);
	virtual void operator ++ ();

      private:
	void _update_reaction_degree(int);
	
      protected:
	bool isReactive;
	Data * Properties;
	Clock * Chronos;
	vector<double> lambda_old, lambda;
	vector<double> JOld, J;
	vector<double> TOld, T;
	vector<double> pressure_old, pressure;
	vector<Material::LocalState *> Reacted_MLS;
      };	

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<0>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<0> : public Element::MaterialPoint::Energy<0>
      {
      public: 

	typedef Element::MaterialPoint::Reactive::Energy<1> tangent_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<0> *Clone() const;
	Energy(LocalState *, const vector<Material::Energy<0> *> &);
	Energy(const Energy<0> &);
	virtual range_type operator () (const domain_type &) const;
        
      protected:
	LocalState *LS;
      };


      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<1> : public Element::MaterialPoint::Energy<1>
      {
      public: 

	typedef Element::MaterialPoint::Reactive::Energy<2> tangent_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;	
	
	Energy();
	virtual ~Energy();
	Element::Energy<1> *Clone() const;
	Energy(LocalState *, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> &);
	Energy(LocalState *, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> &,
	       const vector<Material::Energy<1> *> &);
	Energy(const Energy<1> &);

	Element::LocalState *GetLocalState() const;

	virtual vector_type operator() (const vector_type &) const {
	  assert(false);
	}

        virtual void operator () (const vector_type &, 
				  const vector<Set::VectorSpace::Vector> &, 
                                  vector_type &) const;
        
      protected:
	LocalState *LS;
	vector<Material::Energy<1> *> Reacted_DW;
      };
    }
  }
}

#endif
