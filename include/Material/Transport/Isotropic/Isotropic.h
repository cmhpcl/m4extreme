// Isotropic.h: interface for the Isotropic class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_TRANSPORT_ISOTROPIC_H__INCLUDED_)
#define MATERIAL_TRANSPORT_ISOTROPIC_H__INCLUDED_

#pragma once

#include <utility>
#include "./Potential/Potential.h"
#include "../../Material.h"

namespace Material
{
namespace Transport
{
namespace Isotropic
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

	typedef Material::Transport::Isotropic::Energy<0> energy_type;
	typedef Set::VectorSpace::Vector domain_type;

	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(const unsigned int &);
	LocalState(const LocalState &);
	void operator () (const domain_type &);
	void operator ++ ();

private:

	unsigned int n;
	double Du2;

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

	typedef Material::Transport::Isotropic::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(LocalState *,
		Material::Transport::Isotropic::Potential::Energy<0> *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Transport::Isotropic::Potential::Energy<0> *f;

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

	typedef Material::Transport::Isotropic::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Material::Energy<1> *Clone() const;
	Energy(LocalState *,
		Material::Transport::Isotropic::Potential::Energy<1> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Transport::Isotropic::Potential::Energy<1> *Df;

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

	typedef Material::Transport::Isotropic::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Material::Energy<2> *Clone() const;
	Energy(LocalState *,
		Material::Transport::Isotropic::Potential::Energy<1> *,
		Material::Transport::Isotropic::Potential::Energy<2> *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Transport::Isotropic::Potential::Energy<1> *Df;
	Material::Transport::Isotropic::Potential::Energy<2> *DDf;

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

	typedef Material::Transport::Isotropic::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Material::Jet<0> *Clone() const;
	Jet(LocalState *, 
		Material::Transport::Isotropic::Potential::Jet<0> *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Transport::Isotropic::Potential::Jet<0> *J;

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

	typedef Material::Transport::Isotropic::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Material::Jet<1> *Clone() const;
	Jet(LocalState *, 
		Material::Transport::Isotropic::Potential::Jet<1> *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Transport::Isotropic::Potential::Jet<1> *DJ;

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_TRANSPORT_ISOTROPIC_H__INCLUDED_
