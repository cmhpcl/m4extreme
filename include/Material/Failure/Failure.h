// Failure.h: interface for the Failure class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_FAILURE_H__INCLUDED_)
#define MATERIAL_FAILURE_H__INCLUDED_

#pragma once

#include <vector>
#include <utility>
#include "../Material.h"

namespace Material
{
namespace Failure
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

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

	typedef Material::Failure::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	LocalState(Material::LocalState *, Material::LocalState *);
	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(const LocalState &);
	void operator ++ ();
	bool & GetFailed();
	const bool & GetFailed() const;
        double GetTemperature() const;

        Material::LocalState * GetLS1() { return LS1; }
        Material::LocalState * GetLS2() { return LS2; }

private:

	bool Failed; 
	Material::LocalState *LS1;
	Material::LocalState *LS2;

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

	typedef Material::Failure::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	Energy(Material::Failure::LocalState *,
		   Material::Energy<0> *, Material::Energy<0> *);
	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;
	double EnergyReleaseDensity(const Set::VectorSpace::Vector &); 

private:

	Material::Failure::LocalState *LS;
	Material::Energy<0> *f1; Material::Energy<0> *f2;

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

	typedef Material::Failure::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Material::Energy<1> *Clone() const;
	Energy(Material::Failure::LocalState *,
		   Material::Energy<1> *, Material::Energy<1> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Failure::LocalState *LS;
	Material::Energy<1> *Df1; Material::Energy<1> *Df2;

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

	typedef Material::Failure::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy(Material::Failure::LocalState *,
		   Material::Energy<2> *, Material::Energy<2> *);
	virtual ~Energy();
	Material::Energy<2> *Clone() const;
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Failure::LocalState *LS;
	Material::Energy<2> *DDf1; Material::Energy<2> *DDf2;

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

	typedef Material::Failure::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	Jet(Material::Failure::LocalState *,
		Material::Jet<0> *, Material::Jet<0> *);
	virtual ~Jet();
	Material::Jet<0> *Clone() const;
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Failure::LocalState *LS;
	Material::Jet<0> *g1; Material::Jet<0> *g2;

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

	typedef Material::Failure::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	Jet(Material::Failure::LocalState *,
		Material::Jet<1> *, Material::Jet<1> *);
	virtual ~Jet();
	Material::Jet<1> *Clone() const;
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Failure::LocalState *LS;
	Material::Jet<1> *Dg1; Material::Jet<1> *Dg2;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

#endif // !defined(MATERIAL_FAILURE_H__INCLUDED_
