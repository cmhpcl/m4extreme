// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_THERMOVISCOELASTICFLOW_H__INCLUDED_)
#define MATERIAL_THERMOVISCOELASTICFLOW_H__INCLUDED_

#pragma once

#include <utility>
#include "Clock/Clock.h"
#include "Material/Symmetric/Symmetric.h"
#include "Utils/Fields.h"

namespace Material
{
  namespace Symmetric
  {
    namespace ThermoViscoElasticFlow
    {
      class Data;
      class LocalState;
      template <unsigned int> class Energy;
      template <unsigned int> class Jet;

      //////////////////////////////////////////////////////////////////////
      // Class Data
      //////////////////////////////////////////////////////////////////////

      class Data
      {
	friend class LocalState;
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;
	friend class Jet<0>;
	friend class Jet<1>;

      public:
	Data();
	virtual ~Data();
	Data(bool, const double &, 
	     const double &, const double &,
	     m4extreme::Utils::ScalarField *,
	     m4extreme::Utils::ScalarField *,
	     m4extreme::Utils::ScalarField *,
	     m4extreme::Utils::ScalarField *,
	     m4extreme::Utils::ScalarField *,
	     m4extreme::Utils::ScalarField *);
	Data(const Data &);
	Data &operator=(const Data &);
	
      protected:
	m4extreme::Utils::ScalarField * Cf;
	m4extreme::Utils::ScalarField * Gamma;
	m4extreme::Utils::ScalarField * Bulk;
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
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<0> energy_type;
	  typedef Set::VectorSpace::Sym domain_type;

	  LocalState();
	  virtual ~LocalState();
	  LocalState(const LocalState &);

	  LocalState(Clock *, Data *);
	  LocalState(Clock *, Data *, bool);

	  Material::Symmetric::LocalState *Clone() const;
	  void operator++();
	  void Reset(double);
	  double GetTemperature() const;
	  const Set::VectorSpace::Sym &GetEpsOld() const;
	  Set::VectorSpace::Sym &GetEpsOld();
	  const double & GetShearModulus() const;
	  const double & GetBulkModulus() const;
	  const double & GetEta() const;
	  const double & GetCf() const;
	  const double & GetGamma() const;
	  const double & GetThermalExp() const;
	  bool IsMelted() const;
	  bool IsVaporized() const;
	  bool IsSaturated() const;

	private:
	  bool isMelted, isVaporized, isSaturated;
	  Clock *Chronos;
	  Data  *Properties;
	  double T;
	  double bulk, mu, eta, cf, gamma, thermal_exp;
	  Set::VectorSpace::Sym EpsNew;
	  Set::VectorSpace::Sym EpsOld;

	private:
	  LocalState &operator=(const LocalState &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<p>
      //////////////////////////////////////////////////////////////////////

      template <unsigned int p>
	class Energy;

      //////////////////////////////////////////////////////////////////////
      // Class Energy<0>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<0> : public Material::Symmetric::Energy<0>
	{
	public:
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<1> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef double range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<0> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<0> &);
	  virtual range_type operator()(const domain_type &) const;
	  virtual range_type operator()(const domain_type &, double) const;

	private:
	  LocalState *LS;

	private:
	  Energy<0> &operator=(const Energy<0> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<1> : public Material::Symmetric::Energy<1>
	{
	public:
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<2> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::SymDual range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<1> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<1> &);
	  virtual range_type operator()(const domain_type &) const;
	  virtual double operator()(const domain_type &, double) const;

	private:
	  LocalState *LS;

	private:
	  Energy<1> &operator=(const Energy<1> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<2> : public Material::Symmetric::Energy<2>
	{
	public:
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Energy<3> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef Set::VectorSpace::Hom range_type;

	  Energy();
	  virtual ~Energy();
	  Material::Symmetric::Energy<2> *Clone() const;
	  Energy(LocalState *);
	  Energy(const Energy<2> &);
	  virtual range_type operator()(const domain_type &) const;
	  virtual double operator()(const domain_type &, double) const;

	private:
	  LocalState *LS;

	private:
	  Energy<2> &operator=(const Energy<2> &);
	};

      //////////////////////////////////////////////////////////////////////
      // Class Jet<p>
      //////////////////////////////////////////////////////////////////////

      template <unsigned int p>
	class Jet;

      //////////////////////////////////////////////////////////////////////
      // Class Jet<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Jet<1> : public Material::Symmetric::Jet<1>
	{
	public:
	  typedef Material::Symmetric::ThermoViscoElasticFlow::Jet<2> tangent_type;
	  typedef Set::VectorSpace::Sym domain_type;
	  typedef pair<Set::VectorSpace::SymDual, Set::VectorSpace::Hom> range_type;

	  Jet();
	  virtual ~Jet();
	  Material::Symmetric::Jet<1> *Clone() const;
	  Jet(LocalState *);
	  Jet(const Jet<1> &);
	  range_type operator()(const domain_type &) const;

	private:
	  LocalState *LS;

	private:
	  Jet<1> &operator=(const Jet<1> &);
	};
    }
  }
}

#endif // !defined(MATERIAL_THERMOVISCOELASTICFLOW_H__INCLUDED_)
