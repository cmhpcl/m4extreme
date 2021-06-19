// SoilPlasticity_thermal.h: interface for the SoilPlasticity_thermal class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_SOILPLASTICITY_THERMAL_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_SOILPLASTICITY_THERMAL_H__INCLUDED_

#pragma once

#include <utility>
#include "Material/Symmetric/Symmetric.h"
#include "Material/Shear/Shear.h"
#include "Material/Uniaxial/Uniaxial.h"
#include "Material/Gas/EoS/EoS.h"
#include "Clock/Clock.h"

namespace Material
{
  namespace Symmetric
  {
    namespace SoilPlasticity_thermal
    {
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
	Data( const double & rhs_T0,
	      const double & rhs_Tm,		   
	      const double & rhs_Tc, 
	      const double & rhs_a,
	      const double & rhs_a1,
	      const double & rhs_b1, 
	      const double & rhs_a2, 
	      const double & rhs_b2, 
	      const double & rhs_m, 
	      const double & rhs_n, 
	      const double & rhs_R,
	      const double & rhs_friction,
	      const double & rhs_dilatancy,
	      const double & rhs_Bc,
	      const double & rhs_Muc)
	  : Tm(rhs_Tm), T0(rhs_T0), Tc(rhs_Tc), a(rhs_a), 
	  a1(rhs_a1), b1(rhs_b1), a2(rhs_a2), b2(rhs_b2),
	  m(rhs_m), n(rhs_n), R(rhs_R), Bc(rhs_Bc), Muc(rhs_Muc) {
	  assert(Tc > T0 && Tm > T0);
	  assert(rhs_friction <= 45.0 && rhs_friction >= 0.0
		 && rhs_dilatancy <= 45.0 && rhs_dilatancy >= 0.0);
	  double sinx = sin(rhs_friction * 3.1415926 / 180.0);
	  double cloc = 6.0 * sqrt(2.0 / 3.0);
	  friction = cloc * sinx / (3.0 - sinx);
	  
	  double siny = sin(rhs_dilatancy * 3.1415926 / 180.0);
	  dilatancy = cloc * siny / (3.0 - siny);
	}
	~Data(){}

      private:
	double a;
	double friction, dilatancy;
	double T0, Tm, Tc;
	double a1, b1, a2, b2, m, n, R;
	double Bc, Muc;
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

	  typedef Material::Symmetric::SoilPlasticity_thermal::Energy<0> energy_type;
	  typedef Set::VectorSpace::Sym domain_type;

	  LocalState();
	  virtual ~LocalState();

	  LocalState(Clock *,  Data *,
		     Material::Gas::EoS::LocalState *,
		     Material::Gas::EoS::Energy<1> *,
		     Material::Shear::Modulus<0> *,
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
	  void ComputeCureDegree();
	  
	  const double & GetEpspEff() const;
	  double & GetEpspEff();
	  const double & GetEpspEffOld() const;
	  double & GetEpspEffOld();
	  const Set::VectorSpace::Sym & GetEpsp() const;
	  Set::VectorSpace::Sym & GetEpsp();
	  const Set::VectorSpace::Sym & GetEpspOld() const;
	  Set::VectorSpace::Sym & GetEpspOld();
	  const double & GetCureDegreeOld() const;
	  double & GetCureDegreeOld();
	  const double & GetCureDegree() const;	  
	  double & GetCureDegree();
	  const double & GetCureRate() const;
	  double & GetCureRate();

	  double GetSigEff() const;
	  double GetTemperature() const;
	  double GetPressure() const;
	  void   Reset(double T_);
        
	private:
	  bool IsMelted;
	  Data * D;
	  Clock *Chronos;
	  Material::Gas::EoS::LocalState * _pEoSLS;
	  Material::Gas::EoS::Energy<1> *Df;
	  Material::Shear::Modulus<0> *Mu;
	  Material::Uniaxial::Energy<1> *DWp;
	  Material::Uniaxial::Energy<2> *DDWp;
	  Material::Uniaxial::Jet<1> *DJp;
	  Material::Uniaxial::Energy<1> *DWv;
	  Material::Uniaxial::Energy<2> *DDWv;
	  Material::Uniaxial::Jet<1> *DJv;
	  double EpspEff, EpspEffOld, SigEff;
	  double J, JOld, Timer;
	  double T, TOld, Tm_T0;        
	  double CureDegree, CureDegreeOld, CureRate;
	  Set::VectorSpace::Sym Epsp;
	  Set::VectorSpace::Sym EpspOld;

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

	  typedef Material::Symmetric::SoilPlasticity_thermal::Energy<1> tangent_type;
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

	  typedef Material::Symmetric::SoilPlasticity_thermal::Energy<2> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::SymDual range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<1> *Clone() const;
	  Energy(LocalState *,
		 Material::Gas::EoS::Energy<1> *,
		 Material::Shear::Jet<0> *);
	  Energy(const Energy<1> &);
	  range_type operator () (const domain_type &) const;
	  virtual double operator () (const domain_type &, double) const;
	private:

	  LocalState *LS;
	  Material::Gas::EoS::Energy<1> *Df;
	  Material::Shear::Jet<0> *JMu;

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

	  typedef Material::Symmetric::SoilPlasticity_thermal::Energy<3> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::Hom range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<2> *Clone() const;
	  Energy(LocalState *, 
		 Material::Gas::EoS::Energy<2> *,
		 Material::Uniaxial::Jet<1> *,
		 Material::Uniaxial::Jet<1> *,
		 Material::Shear::JetJet<0> *);
	  Energy(const Energy<2> &);
	  range_type operator () (const domain_type &) const;
	  virtual double operator () (const domain_type &, double) const;
	private:

	  LocalState *LS;
	  Material::Gas::EoS::Energy<2> *DDf;
	  Material::Uniaxial::Jet<1> *DJp;
	  Material::Uniaxial::Jet<1> *DJv;
	  Material::Shear::JetJet<0> *JJMu;

	private:

	  Energy<2> & operator = (const Energy<2> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<p>
      //////////////////////////////////////////////////////////////////////

      template<unsigned int p> class Jet;

      //////////////////////////////////////////////////////////////////////
      // Class Jet<0>
      //////////////////////////////////////////////////////////////////////

      template<>
	class Jet<0> : public Material::Symmetric::Jet<0>
	{
	public: 

	  typedef Material::Symmetric::SoilPlasticity_thermal::Jet<1> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef pair<double, Set::VectorSpace::SymDual> range_type;

	  Jet();
	  virtual ~Jet();
	  Material::Symmetric::Jet<0> *Clone() const;
	  Jet(LocalState *, 
	      Material::Gas::EoS::Jet<0> *, 
	      Material::Shear::Jet<0> *,
	      Material::Uniaxial::Energy<0> *,
	      Material::Uniaxial::Energy<0> *);
	  Jet(const Jet<0> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	  Material::Gas::EoS::Jet<0> *g;
	  Material::Shear::Jet<0> *JMu;
	  Material::Uniaxial::Energy<0> *Wp;
	  Material::Uniaxial::Energy<0> *Wv;

	private:

	  Jet<0> & operator = (const Jet<0> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Jet<1> : public Material::Symmetric::Jet<1>
	{
	public: 

	  typedef Material::Symmetric::SoilPlasticity_thermal::Jet<2> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef pair<Set::VectorSpace::SymDual, 
	    Set::VectorSpace::Hom> range_type;

	  Jet();
	  virtual ~Jet();
	  Material::Symmetric::Jet<1> *Clone() const;
	  Jet(LocalState *,
	      Material::Gas::EoS::Jet<1> *,
	      Material::Shear::JetJet<0> *,
	      Material::Uniaxial::Jet<1> *,
	      Material::Uniaxial::Jet<1> *);
	  Jet(const Jet<1> &);
	  range_type operator () (const domain_type &) const;

	private:

	  LocalState *LS;
	  Material::Gas::EoS::Jet<1> *Dg;
	  Material::Shear::JetJet<0> *JJMu;
	  Material::Uniaxial::Jet<1> *DJp;
	  Material::Uniaxial::Jet<1> *DJv;

	private:

	  Jet<1> & operator = (const Jet<1> &);
	};

    }

  }

}

#endif // !defined(MATERIAL_SYMMETRIC_J2ISOTROPIC_THERMAL_H__INCLUDED_
