// TGO.h: interface for the TGO class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_HOOKEAN_TGO_H__INCLUDED_)
#define MATERIAL_HOOKEAN_TGO_H__INCLUDED_

#pragma once

#include <utility>
#include "../../../Clock/Clock.h"
#include "../../Material.h"

namespace Material
{
  namespace Hookean
  {
    namespace TGO
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
	Data(const double * const);

	Data(const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &);

	Data(const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &, const double &, const double &,
	     const double &, const double &);

	Data(const Data &);
	const double & GetLambda() const;
	double & GetLambda();
	const double & GetMu() const;
	double & GetMu();
	const double & GetAlpha() const;
	double & GetAlpha();
	const double & GetT0() const;
	double & GetT0();
	Data & operator = (const Data &);
	void Randomize();

      private:
	double Lambda;
	double Mu;
	double Alpha;
	double T0;

	// growth
	double _t_cycle;
	double _h0, _he0;
	double _alpha_0, _Q, _R;
	double _beta;

	// plasticity
	double _sigc, _Et;
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

	typedef Material::Hookean::TGO::Data data_type;
	typedef Material::Hookean::TGO::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(Data *, Clock *);
	LocalState(const LocalState &);
	Set::VectorSpace::Hom operator () (const Set::VectorSpace::Vector &);
	void operator ++ ();
	void SetNormal(const Set::VectorSpace::Vector &);
	void SetTan(const Set::VectorSpace::Vector &);
	virtual void Reset(double);

      private:
	Set::VectorSpace::Hom _thermo_elasticity (const Set::VectorSpace::Vector &);
	Set::VectorSpace::Hom _thermo_plasticity (const Set::VectorSpace::Vector &);
	
      private:

	double T;
	Data *Properties;
	Clock *Timer;

	// growth
	Set::VectorSpace::Vector _e_normal, _e_tan; 

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

	typedef Material::Hookean::TGO::Energy<1> tangent_type;
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

	typedef Material::Hookean::TGO::Energy<2> tangent_type;
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

	typedef Material::Hookean::TGO::Energy<3> tangent_type;
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

	typedef Material::Hookean::TGO::Jet<1> tangent_type;
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

	typedef Material::Hookean::TGO::Jet<2> tangent_type;
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

#endif // !defined(MATERIAL_HOOKEAN_TGO_H__INCLUDED_
