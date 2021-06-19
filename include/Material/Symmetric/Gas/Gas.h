// Gas.h: interface for the Gas class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_GAS_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_GAS_H__INCLUDED_

#pragma once

#include <utility>
#include "../Symmetric.h"
#include "../../Gas/EoS/EoS.h"

namespace Material
{
namespace Symmetric
{
namespace Gas
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

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

	typedef Material::Symmetric::Gas::Energy<0> energy_type;
	typedef Set::VectorSpace::Sym domain_type;

	LocalState();
	LocalState(Material::Gas::EoS::LocalState *);
	virtual ~LocalState();
	Material::Symmetric::LocalState *Clone() const;
	LocalState(const LocalState &);
	void operator ++ ();

        void Reset(double T_) { return LS->Reset(T_); }
        double GetTemperature() const { return LS->GetTemperature(); }
private:

	Material::Gas::EoS::LocalState *LS;

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

	typedef Material::Symmetric::Gas::Energy<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef double range_type;

	Energy();
	Energy(LocalState *, Material::Gas::EoS::Energy<0> *);
	virtual ~Energy();
	Material::Symmetric::Energy<0> *Clone() const;
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Energy<0> *f;

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

	typedef Material::Symmetric::Gas::Energy<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::SymDual range_type;

	Energy();
	virtual ~Energy();
	Material::Symmetric::Energy<1> *Clone() const;
	Energy(LocalState *, Material::Gas::EoS::Energy<1> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

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

	typedef Material::Symmetric::Gas::Energy<3> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy();
	Energy(LocalState *, Material::Gas::EoS::Jet<1> *);
	virtual ~Energy();
	Material::Symmetric::Energy<2> *Clone() const;
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Jet<1> *Dg;

private:

	unsigned int map[3][3];

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

	typedef Material::Symmetric::Gas::Jet<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<double,Set::VectorSpace::SymDual> range_type;

	Jet();
	Jet(LocalState *, Material::Gas::EoS::Jet<0> *);
	virtual ~Jet();
	Material::Symmetric::Jet<0> *Clone() const;
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Jet<0> *g;

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

	typedef Material::Symmetric::Gas::Jet<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<Set::VectorSpace::SymDual, 
		Set::VectorSpace::Hom> range_type;

	Jet();
	Jet(LocalState *, Material::Gas::EoS::Jet<1> *);
	virtual ~Jet();
	Material::Symmetric::Jet<1> *Clone() const;
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Gas::EoS::Jet<1> *Dg;

private:

	unsigned int map[3][3];

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_GAS_H__INCLUDED_
