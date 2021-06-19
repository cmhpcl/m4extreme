// TGOGrowth.h: interface for the TGOGrowth class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_HOOKEAN_TGO_GROWTH_H__INCLUDED_)
#define MATERIAL_HOOKEAN_TGO_GROWTH_H__INCLUDED_

#pragma once

#include <utility>
#include "../../../Clock/Clock.h"
#include "../../Material.h"

namespace Material
{
  namespace Hookean
  {
    namespace TGOGrowth
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

	Data(const double &Lambda0_, const double &Mu0_,    const double &Alpha0_,
	     const double &Lambda1_, const double &Mu1_,    const double &Alpha1_,
	     const double &T0_);
	Data(const double &Lambda0_, const double &Mu0_,    const double &Alpha0_,
	     const double &Lambda1_, const double &Mu1_,    const double &Alpha1_,
	     const double &sigc0_,   const double &Et0_,
	     const double &sigc1_,   const double &Et1_,
	     const double &T0_);
	Data(const Data &);
	Data & operator = (const Data &);

      private:
	// elastic
	double Lambda0, Lambda1;
	double Mu0, Mu1;

	// thermal
	double Alpha0, Alpha1;
	double T0;

	// plastic
	double _sigc0, _Et0, _sigc1, _Et1;
      };

      //////////////////////////////////////////////////////////////////////
      // Class LocalState
      //////////////////////////////////////////////////////////////////////

      class LocalState : public Material::LocalState
      {
	friend class Energy<0>;
	friend class Energy<1>;
	friend class Energy<2>;
	friend class Jet<0>;
	friend class Jet<1>;

      public: 

	typedef Material::Hookean::TGOGrowth::Data data_type;
	typedef Material::Hookean::TGOGrowth::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(Data *, Clock *);
	LocalState(const LocalState &);
	Set::VectorSpace::Hom operator () (const Set::VectorSpace::Vector &);
	void operator ++ ();

	void SetNormal(const Set::VectorSpace::Vector &);
	const Set::VectorSpace::Vector & GetNormal() const;

	void SetTan(const Set::VectorSpace::Vector &);
	void SetStatus(bool);
	bool GetStatus() const;

	// set the growth strain in thickness
	void SetGrowthStrain(double);

	// set the growth strain in tangent
	void SetGrowthStrainT1(double);
	void SetGrowthStrainT2(double);

	const double & GetLambda() const;
	const double & GetMu() const;

	virtual void Reset(double);

      private:
	Set::VectorSpace::Hom _thermo_elasticity (const Set::VectorSpace::Vector &);
	Set::VectorSpace::Hom _thermo_plasticity (const Set::VectorSpace::Vector &);
	void _compute_growth_strain(const Set::VectorSpace::Hom &, Set::VectorSpace::Hom &);
	void _compute_thermal_strain(Set::VectorSpace::Hom &);
	
      private:
	double T;
	Data *Properties;
	Clock *Timer;

	// status
	bool _isTGO;
	double _lambda;
	double _mu;
	double _alpha;
	double _sigc;
	double _Et;

	// growth in thickness
	double _epst;

	// growth along the tangent direction
	double _epss1;
	double _epss2;

	// thickness direction and tangent direction
	Set::VectorSpace::Vector _e_normal, _e_tan1, _e_tan2; 
	Set::VectorSpace::Vector _e_norm_New;

	// plasticity
	double EpspEff, EpspEffOld;
	Set::VectorSpace::Hom Epsp;
	Set::VectorSpace::Hom EpspOld;

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
	class Energy<0> : public Material::Energy<0>
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:

	Energy<0> & operator = (const Energy<0> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<1> : public Material::Energy<1>
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Material::Energy<1> *Clone() const;
	Energy(LocalState *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:

	Energy<1> & operator = (const Energy<1> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Energy<2>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Energy<2> : public Material::Energy<2>
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Material::Energy<2> *Clone() const;
	Energy(LocalState *);
	Energy(const Data *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

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
	class Jet<0> : public Material::Jet<0>
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Material::Jet<0> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:

	Jet<0> & operator = (const Jet<0> &);
      };

      //////////////////////////////////////////////////////////////////////
      // Class Jet<1>
      //////////////////////////////////////////////////////////////////////

      template <>
	class Jet<1> : public Material::Jet<1>
      {
      public: 

	typedef Material::Hookean::TGOGrowth::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Material::Jet<1> *Clone() const;
	Jet(LocalState *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

      private:

	LocalState *LS;

      private:

	Jet<1> & operator = (const Jet<1> &);
      };

    }

  }

}

#endif // !defined(MATERIAL_HOOKEAN_TGO_GROWTH_H__INCLUDED_
