// LinearizedKinematics.h: interface for the LinearizedKinematics class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_ROTATED_LINEARIZEDKINEMATICS_H__INCLUDED_)
#define MATERIAL_ROTATED_LINEARIZEDKINEMATICS_H__INCLUDED_

#pragma once

#include <utility>
#include "../../Material.h"

using namespace std;

namespace Material
{
namespace Rotated
{
namespace LinearizedKinematics
{
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

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

	typedef Material::Rotated::LinearizedKinematics::Energy<0> energy_type;

	LocalState();
	virtual ~LocalState();
	Material::LocalState *Clone() const;
	LocalState(Material::LocalState *, Set::VectorSpace::Hom *);
	LocalState(const LocalState &);
	void operator ++ ();

private:

	Material::LocalState *LS;
	Set::VectorSpace::Hom *R;

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

template<> class Energy<0> : public Material::Energy<0>
{
public: 

	typedef Material::Rotated::LinearizedKinematics::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	virtual ~Energy();
	Material::Energy<0> *Clone() const;
	Energy(LocalState *, Material::Energy<0> *);
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<0> *W;

private:

	Energy & operator = (const Energy<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <> class Energy<1> : public Material::Energy<1>
{
public: 

	typedef Material::Rotated::LinearizedKinematics::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	virtual ~Energy();
	Material::Energy<1> *Clone() const;
	Energy(LocalState *, Material::Energy<1> *);
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<1> *DW;

private:

	Energy & operator = (const Energy<1> &);
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <> class Energy<2> : public Material::Energy<2>
{
public: 

	typedef Material::Rotated::LinearizedKinematics::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	virtual ~Energy();
	Material::Energy<2> *Clone() const;
	Energy(LocalState *, Material::Energy<2> *);
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Energy<2> *DDW;

private:

	Energy & operator = (const Energy<2> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<> class Jet<0> : public Material::Jet<0>
{
public: 

	typedef Material::Rotated::LinearizedKinematics::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	virtual ~Jet();
	Material::Jet<0> *Clone() const;
	Jet(LocalState *, Material::Jet<0> *);
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Jet<0> *J;

private:

	Jet & operator = (const Jet<0> &);
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <> class Jet<1> : public Material::Jet<1>
{
public: 

	typedef Material::Rotated::LinearizedKinematics::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	virtual ~Jet();
	Material::Jet<1> *Clone() const;
	Jet(LocalState *, Material::Jet<1> *);
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	LocalState *LS;
	Material::Jet<1> *DJ;

private:

	Jet & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_ROTATED_LINEARIZEDKINEMATICS_H__INCLUDED_)
