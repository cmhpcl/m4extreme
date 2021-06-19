// ThermalFlow.cpp: implementation of the ThermalFlow class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Symmetric/Symmetric.h"
#include "Material/Uniaxial/Uniaxial.h"
#include "Material/Gas/EoS/EoS.h"
#include "Clock/Clock.h"
#include "Utils/Fields.h"

namespace Material
{
  namespace Symmetric
  {
    namespace ThermoViscoPlasticFlow
    {
      class Data;
      class LocalState;
      template <unsigned int> class Energy;
      template <unsigned int> class Jet;

      //////////////////////////////////////////////////////////////////////
      // Class Data
      //////////////////////////////////////////////////////////////////////
      class Data {
	friend class LocalState;
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;
	friend class Jet<0>;
	friend class Jet<1>;

      public:
	Data ( bool enableVaporization_,
	       double beta_, double T0_, double Tm_, double Tb_,
	       double a_,    double aT_, double gamma0_,
	       m4extreme::Utils::ScalarField *Cf_, 
	       m4extreme::Utils::ScalarField *Gamma_,
	       m4extreme::Utils::ScalarField *Mu_,
	       m4extreme::Utils::ScalarField *Eta_,
	       m4extreme::Utils::ScalarField *Thermal_Exp_ ) 
	  : enableVaporization(enableVaporization_), 
	  beta(beta_), T0(T0_), Tm(Tm_), Tb(Tb_), a(a_), 
	  aT(aT_), gamma0(2.0*(1.0/3.0 + aT_ - gamma0_)),
	  Mu(Mu_), Eta(Eta_), Cf(Cf_), Gamma(Gamma_), 
	  Thermal_Exp(Thermal_Exp_){
	  }
	
        Data( bool enableVaporization_,
	      double beta_, double T0_, double Tm_, 
	      double Tb_,   double a_,
	      m4extreme::Utils::ScalarField *Cf_, 
	      m4extreme::Utils::ScalarField *Gamma_,
	      m4extreme::Utils::ScalarField *Mu_,
	      m4extreme::Utils::ScalarField *Eta_,
	      m4extreme::Utils::ScalarField *Thermal_Exp_)
	  : enableVaporization(enableVaporization_),
	  beta(beta_), T0(T0_), Tm(Tm_), Tb(Tb_), a(a_),
	  aT(0.0), gamma0(0.0), 
	  Mu(Mu_), Eta(Eta_), Cf(Cf_), Gamma(Gamma_), 
	  Thermal_Exp(Thermal_Exp_) {
	}      

	~Data(){}

      private:
	double beta, a, aT, gamma0;
	m4extreme::Utils::ScalarField * Cf;
	m4extreme::Utils::ScalarField * Gamma;
	m4extreme::Utils::ScalarField * Mu;
	m4extreme::Utils::ScalarField * Eta;
	m4extreme::Utils::ScalarField * Thermal_Exp;
	double T0, Tm, Tb;
	bool enableVaporization;
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Material::Symmetric::LocalState
	{
	  friend class Energy<0>;
	  friend class Energy<1>;
	  friend class Energy<2>;
	  friend class Jet<0>;
	  friend class Jet<1>;

	public: 

	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<0> energy_type;
	  typedef Set::VectorSpace::Sym domain_type;

	  LocalState();
	  virtual ~LocalState();

	  LocalState(Clock *,  Data *,		     
		     Material::Gas::EoS::LocalState *,		     
		     Material::Uniaxial::Energy<1> *,
		     Material::Uniaxial::Energy<2> *,
		     Material::Uniaxial::Jet<1> *,
		     Material::Uniaxial::Energy<1> *,
		     Material::Uniaxial::Energy<2> *,
		     Material::Uniaxial::Jet<1> *);

	  Material::Symmetric::LocalState *Clone() const;
	  LocalState(const LocalState &);
	  bool operator () (const domain_type &);
	  void operator ++ ();
	  const double & GetEpspEff() const;
	  double & GetEpspEff();
	  const double & GetEpspEffOld() const;
	  double & GetEpspEffOld();

	  const Set::VectorSpace::Sym & GetEpsp() const;
	  Set::VectorSpace::Sym & GetEpsp();
	  const Set::VectorSpace::Sym & GetEpspOld() const;
	  Set::VectorSpace::Sym & GetEpspOld();
	  double & GetDotEpspEff();
	  const double & GetDotEpspEff() const;

	  const Set::VectorSpace::Sym & GetEps() const;
	  Set::VectorSpace::Sym & GetEps();
	  const Set::VectorSpace::Sym & GetEpsOld() const;
	  Set::VectorSpace::Sym & GetEpsOld();
	  double & GetDotEpsEff();
	  const double & GetDotEpsEff() const;
        
	  double GetSigEff() const;
	  double GetTemperature() const;
	  double GetPressure() const;
	  void   Reset(double T_);
	  void   Relax();

	  const double & GetShearModulus() const;	  
	  const double & GetEta() const;
	  const double & GetCf() const;
	  const double & GetGamma() const;
	  const double & GetThermalExp() const;
	  bool IsMelted() const;
	  bool IsVaporized() const;
	  bool IsSaturated() const;

	private:
	  Data * D;
	  Clock *Chronos;
	  Material::Gas::EoS::LocalState * _pEoSLS;
	  Material::Uniaxial::Energy<1> *DWp;
	  Material::Uniaxial::Energy<2> *DDWp;
	  Material::Uniaxial::Jet<1> *DJp;
	  Material::Uniaxial::Energy<1> *DWv;
	  Material::Uniaxial::Energy<2> *DDWv;
	  Material::Uniaxial::Jet<1> *DJv;
	  double EpspEff, EpspEffOld, SigEff;
	  double T, TOld, Tm_T0; 
	  Set::VectorSpace::Sym Epsp;
	  Set::VectorSpace::Sym EpspOld;
	  Set::VectorSpace::Sym Eps;
	  Set::VectorSpace::Sym EpsOld;
	  double Dint, DDint;
	  double DintOld,DDintOld;

	  // current material properties
	  bool isMelted, isVaporized;
	  double mu, eta, cf, gamma, thermal_exp;

	private:
	  LocalState & operator = (const LocalState &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<0>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Energy<0> : public Material::Symmetric::Energy<0>
	{
	public: 

	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<1> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef double range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<0> *Clone() const;
	  Energy(LocalState *,
		 Material::Gas::EoS::Energy<0> *,
		 Material::Uniaxial::Energy<0> *,
		 Material::Uniaxial::Energy<0> *);
	  Energy(const Energy<0> &);
	  range_type operator () (const domain_type &) const;
	  double operator () (const domain_type &, double) const;
	private:

	  LocalState *LS;
	  Material::Gas::EoS::Energy<0> *f;
	  Material::Uniaxial::Energy<0> *Wp;
	  Material::Uniaxial::Energy<0> *Wv;

	private:

	  Energy<0> & operator = (const Energy<0> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<1> : public Material::Symmetric::Energy<1>
	{
	public: 

	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<2> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::SymDual range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<1> *Clone() const;
	  Energy(LocalState *,		 
		 Material::Gas::EoS::Energy<1> *);
	  Energy(const Energy<1> &);
	  range_type operator () (const domain_type &) const;
	  virtual double operator () (const domain_type &, double) const;

	private:
	  LocalState *LS;	  
	  Material::Gas::EoS::Energy<1> *Df;	  

	private:
	  Energy<1> & operator = (const Energy<1> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<2> : public Material::Symmetric::Energy<2>
	{
	public: 

	  typedef Material::Symmetric::ThermoViscoPlasticFlow::Energy<3> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::Hom range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<2> *Clone() const;
	  Energy(LocalState *,		 
		 Material::Gas::EoS::Energy<2> *,
		 Material::Uniaxial::Jet<1> *,
		 Material::Uniaxial::Jet<1> *);
	  Energy(const Energy<2> &);
	  range_type operator () (const domain_type &) const;
	  virtual double operator () (const domain_type &, double) const;

	private:
	  LocalState *LS;
	  Material::Gas::EoS::Energy<2> *DDf;
	  Material::Uniaxial::Jet<1> *DJp;
	  Material::Uniaxial::Jet<1> *DJv;

	private:

	  Energy<2> & operator = (const Energy<2> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Jet;
    }

  }

}

#endif // !defined(MATERIAL_SYMMETRIC_THERMOVISCOPLASTICFLOW_H__INCLUDED_
