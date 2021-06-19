// TMElement.h: interface for the TMElement class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(ELEMENT_MATERIALPOINT_TMELEMENT_H__INCLUDED_)
#define ELEMENT_MATERIALPOINT_TMELEMENT_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Element/MaterialPoint/MaterialPoint.h"

using namespace std;

namespace Element
{
  namespace MaterialPoint
  {
    namespace TMCoupling //ThermoMechanical Coupling
    {
	
      class LocalState;
      template <unsigned int> class Energy;	
	
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
	LocalState(Clock *, unsigned int, 
		   const vector<double> & T0, Data *, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(Clock *, unsigned int, 
		   const vector<double> & T0, Data *, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(Clock *, unsigned int, double, double,
		   const vector<double> & T0, Data *, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(Clock *, unsigned int, double, double,
		   const vector<double> & T0, Data *, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &, 
		   const vector<Material::LocalState *> &);
	LocalState(const LocalState &);
	virtual void operator ++ ();
	virtual range_type operator () (const domain_type &);	
	virtual void Reset(const domain_type &);

	Material::LocalState * GetSourceLS(int index);
	Material::LocalState * GetChiLS(int index);

	virtual void operator() (const domain_type &, range_type &);
      private:
	void _initialization();
	const vector<double> & _computeTemperature(const domain_type &);
	
      protected:
	Clock * Chronos;
	unsigned int TimeFactor;
	double Tm;
	double Tb;
	bool activatedSolidification;
	bool activatedMelting;
	bool activatedVaporization;
	bool activatedSaturation;
	vector<double> Told, T;
	vector<Material::LocalState *> ChiLS;
	vector<Material::LocalState *> SourceLS;
      };	

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Energy;


      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<1> : public Element::MaterialPoint::Energy<1>
      {
      public: 

	typedef Element::MaterialPoint::Energy<2> tangent_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<Set::Manifold::Point *, double> range_type;
	
	Energy();
	virtual ~Energy();
	Element::Energy<1> *Clone() const;
	Energy(LocalState *, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<0> *> &);
	Energy(LocalState *, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<0> *> &,
	       const vector<Material::Energy<1> *> &);
	Energy(const Energy<1> &);

	Element::LocalState *GetLocalState() const;
	virtual range_type operator () (const vector_type &,
					const scalar_type &) const {
	  assert(false);
	}

        virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;

      protected:
	virtual double _getHeatSource(int) const;
        
      protected:
	LocalState *LS;
	vector<Material::Energy<1> *> dChi; // H=dChi/dG
	vector<Material::Energy<0> *> Source;
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<2> : public Element::MaterialPoint::Energy<2>
      {
      public: 

	typedef Element::MaterialPoint::Energy<3> tangent_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<pair<Set::Manifold::Point *, Set::Manifold::Point *>, double> range_type;

	Energy();
	virtual ~Energy();
	Element::Energy<2> *Clone() const;
	Element::LocalState *GetLocalState() const;
	Energy(LocalState *, 
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<1> *> &,
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<0> *> &);
	Energy(LocalState *, 
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<1> *> &,
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<0> *> &,
	       const vector<Material::Energy<2> *> &);
	Energy(const Energy<2> &);
	
	virtual range_type operator () (const vector_type &,
					const scalar_type &) const {
	  assert(false);
	}
	
        virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;
 
      protected:
	virtual double _getHeatSource(int) const;     
     
      protected:
	LocalState *LS;
	vector<Material::Energy<1> *> dChi;  //H=dChi/dG
	vector<Material::Energy<2> *> ddChi; //dHdG=ddChi/dGdG	
	vector<Material::Energy<0> *> Source;
      };

    }
  }
}

#endif
