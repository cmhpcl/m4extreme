// ReactiveTMElement.h: interface for the ReactiveTMElement class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(ELEMENT_MATERIALPOINT_REACTIVETMELEMENT_H__INCLUDED_)
#define ELEMENT_MATERIALPOINT_REACTIVETMELEMENT_H__INCLUDED_

#pragma once

#include "Clock/Clock.h"
#include "Material/Gas/EoS/JWL/Factory.h"
#include "Material/Source/Reaction/Factory.h"
#include "Element/ThermoMechanicalCoupling/TMElement.h"

using namespace std;

namespace Element
{
  namespace MaterialPoint
  {
    namespace ReactiveTMCoupling //ThermoMechanical Coupling
    {
	
      class LocalState;
      template <unsigned int> class Energy;	

      //////////////////////////////////////////////////////////////////////
      // Class Data
      //////////////////////////////////////////////////////////////////////
      class Data {

	friend class LocalState;

      public:
        Data(double Tc_, double A_, double E0_, double R_):
	Type(0), Arrh_Tc(Tc_), Arrh_A(A_), Arrh_ER(-1.0*E0_/R_) {}
	
	Data(double Pc_, double Fi_, double FG1_, double FG2_,
	     double I_,  double b_,  double a_,   double x_,
	     double G1_, double c_,  double d_,   double y_,
	     double G2_, double e_,  double g_,   double z_):
	Type(1), IG_Pc(Pc_), IG_Fi(Fi_), IG_FG1(FG1_), IG_FG2(FG2_),
	  IG_I(I_), IG_b(b_), IG_a(a_), IG_x(x_),
	  IG_G1(G1_), IG_c(c_), IG_d(d_), IG_y(y_),
	  IG_G2(G2_), IG_e(e_), IG_g(g_), IG_z(z_) {}

	virtual ~Data(){}

      private:
	int Type;

	//
	// constants in the Arrhenius equation:
	// reaction rate dlambda/dt=A*exp(-E0/(R*T))
	//
	double Arrh_Tc, Arrh_A, Arrh_ER;

	//
	// constants in the three terms ignition & growth model
	// dF/dt = I * (1-F)^b * (rho/rho0 - 1 - a)^x + G1*(1-F)^c*F^d*p^y
	//       + G2*(1-F)^e * F^g * p^z
	//
	double IG_I, IG_b, IG_a, IG_x, IG_G1, IG_c, IG_d, IG_y, IG_G2, IG_e, IG_g, IG_z;
	double IG_Fi, IG_FG1, IG_FG2;
	double IG_Pc;
      };
	
      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////
      class LocalState : public Element::MaterialPoint::TMCoupling::LocalState
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
	LocalState(Data  *Prop_,
		   Clock *Chronos_,
		   unsigned int TimeFactor_,			     
		   const vector<double> & T0_,
		   MaterialPoint::Data *D_,
		   const vector<Material::LocalState *> & MLS_,
		   const vector<Material::Gas::EoS::JWL::LocalState *> & Reacted_MLS_,
		   const vector<Material::LocalState *> & ChiLS_,
		   const vector<Material::LocalState *> & SourceLS_,
		   const vector<Material::Source::Reaction::LocalState*> &);
	LocalState(Data  *Prop_,
		   Clock *Chronos_,
		   unsigned int TimeFactor_,			     
		   const vector<double> & T0_,
		   MaterialPoint::Data *D_,
		   const vector<Material::LocalState *> & MLS_,
		   const vector<Material::Gas::EoS::JWL::LocalState *> & Reacted_MLS_,
		   const vector<Material::LocalState *> & ChiLS_,
		   const vector<Material::LocalState *> & SourceLS_,
		   const vector<Material::Source::Reaction::LocalState*> &,
		   const vector<Material::LocalState *> & BodyForceLS_);
	LocalState(const LocalState &);
	virtual void operator ++ ();
	virtual void Reset(const domain_type &);

	double GetReactionDegree() const;

      private:
	void _update_reaction_degree(int);
	
      protected:
	Set::VectorSpace::Hom Fold;
	Set::VectorSpace::Hom F;
	bool isReactive;
	Data * Properties;
	vector<double> lambda_old, lambda;
	vector<double> pressure_old, pressure;
	vector<Material::Gas::EoS::JWL::LocalState *> Reacted_MLS;
	vector<Material::Source::Reaction::LocalState*> Reaction_SourceLS;
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
	Energy(LocalState *, const vector<Material::Energy<0> *> &, 
	       const vector<Material::Energy<0> *> &);
	Energy(const Energy<0> &);

	virtual range_type operator () (const domain_type &) const;

      protected:
	LocalState *LS;
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<1> : public Element::MaterialPoint::TMCoupling::Energy<1>
      {
      public: 

	typedef Element::MaterialPoint::TMCoupling::Energy<2> tangent_type;
	typedef map<Set::Manifold::Point *, double> scalar_type;
	typedef map<Set::Manifold::Point *, Set::VectorSpace::Vector> vector_type;
	typedef map<Set::Manifold::Point *, double> range_type;
	
	Energy();
	virtual ~Energy();
	Element::Energy<1> *Clone() const;
	Energy(LocalState *, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Gas::EoS::JWL::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Energy<0> *> &,
	       const vector<Material::Source::Reaction::Energy<0> *> &);
	Energy(LocalState *LS_,
	       const vector<Material::Energy<1> *> &, 
	       const vector<Material::Gas::EoS::JWL::Energy<1> *> &, 
	       const vector<Material::Energy<1> *> & dChi_, 
	       const vector<Material::Energy<0> *> & Source_,
	       const vector<Material::Source::Reaction::Energy<0> *> &, 
	       const vector<Material::Energy<1> *> & BodyForceDW_);
	Energy(const Energy<1> &);

	Element::LocalState *GetLocalState() const;

      protected:
	virtual void _getStress(int q,
				const Set::VectorSpace::Hom & F,
				Set::VectorSpace::Hom & P) const;

	virtual double _getHeatSource(int) const;
        
      protected:
	LocalState *LS;
	vector<Material::Gas::EoS::JWL::Energy<1> *> Reacted_DW;
	vector<Material::Source::Reaction::Energy<0> *> Reaction_Source;
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<2> : public Element::MaterialPoint::TMCoupling::Energy<2>
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
	       const vector<Material::Energy<0> *> &,
	       const vector<Material::Source::Reaction::Energy<0> *> &);
	Energy(LocalState *, 
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<1> *> &,
	       const vector<Material::Energy<2> *> &,
	       const vector<Material::Energy<0> *> &,
	       const vector<Material::Source::Reaction::Energy<0> *> &,
	       const vector<Material::Energy<2> *> &);
	Energy(const Energy<2> &);
	
      protected:
	virtual double _getHeatSource(int) const;     
     
      protected:
	LocalState *LS;
	vector<Material::Source::Reaction::Energy<0> *> Reaction_Source;
      };

    }
  }
}

#endif
