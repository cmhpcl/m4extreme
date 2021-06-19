// MoistPorousTMElement.h: interface for the MoistPorousTMElement class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(ELEMENT_MATERIALPOINT_MOISTPOROUSTMELEMENT_H__INCLUDED_)
#define ELEMENT_MATERIALPOINT_MOISTPOROUSTMELEMENT_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Element/MaterialPoint/MaterialPoint.h"

using namespace std;

namespace Element
{
  namespace MaterialPoint
  {
    namespace MoistPorousTMCoupling //ThermoMechanical Coupling
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
	  LocalState(Clock *, unsigned int, unsigned int,
		     const vector<double> &, Data *,
		     double, double, double, double, double,
		     map<Set::Manifold::Point*, Set::VectorSpace::Vector> *,
		     const vector<Material::LocalState *> &, 
		     const vector<Material::LocalState *> &, 
		     const vector<Material::LocalState *> &);	
	  LocalState(const LocalState &);
	  virtual void operator ++ ();
	  virtual range_type operator () (const domain_type &);	
	  virtual void Reset(const domain_type &);

	  virtual void operator() (const domain_type &, range_type &);

	  virtual void UpdatePorousMedia();
	  void UpdateVoidFraction();
	  void UpdateFluidDensity();

	  const vector<Material::LocalState*> & GetSourceLS() const;
	  double GetDJ() const;
	  double GetPressure() const;
	  double GetPressureOld() const;
	  double GetMoist() const;
	  void SetPressure(double);	
	  void SetVoidFraction(double);
	  double GetVoidFraction() const;
	  void SetFluidDensity(double);
	  double GetFluidDensity() const;
	  void SetVolumeStrain(double);
	  double GetTemperature() const;

	private:
	  const vector<double> & _computeTemperature(const domain_type &);
	  void _ComputePressure_ForwardEuler();
	  void _UpdatePressure();
	
	protected:
	  Clock * Chronos;
	  unsigned int TimeFactor;
	  vector<double> Told, T;
	  vector<Material::LocalState *> ChiLS;
	  vector<Material::LocalState *> SourceLS;
	  double Ks;
	  double Moist, Tb;
	  double density_scaling, Rhof;
	  double Rc, Ma, Mf;
	  double Phif, Phif0;
	  double P0, POld, P;
	  double DPOld, DP;
	  double VolumeStrain, VolumeStrainOld;
	  vector<double> JOld, DJ, J;
	  vector<double> phistory, dqhistory, dvdxhistory;
	  map<Set::Manifold::Point*, Set::VectorSpace::Vector> * Flux;
	  Set::VectorSpace::Hom FOld;
	  Set::VectorSpace::Hom PKOld;
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
	  Energy();
	  virtual ~Energy();
	  Element::Energy<0> *Clone() const;
	  Energy(LocalState *, const vector<Material::Energy<0> *> &);
	  Energy(const Energy<0> &);
	};


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
	  Energy(const Energy<1> &);

	  Element::LocalState *GetLocalState() const;
	  virtual range_type operator () (const vector_type &,
					  const scalar_type &) const {
	    assert(false);
	  }

	  virtual vector_type operator () (const vector_type &) const {
	    assert(false);
	  }	

	  virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;
	  virtual void operator () (const vector_type &, vector_type &) const;
        
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
	  Energy(const Energy<2> &);
	
	  virtual range_type operator () (const vector_type &,
					  const scalar_type &) const {
	    assert(false);
	  }
	
	  virtual void operator () (const vector_type &, const scalar_type &, range_type &) const;
      
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
