// LinearizedKinematics.h: interface for the LinearizedKinematics class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_H__INCLUDED_

#pragma once

#include <utility>
#include "../Symmetric.h"
#include "../../Material.h"

namespace Material
{
namespace Symmetric
{
namespace LinearizedKinematics
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState : public Material::LocalState
{
public: 

	LocalState();
	virtual ~LocalState();
	LocalState(Material::Symmetric::LocalState *);
	Material::LocalState *Clone() const;
	LocalState(const LocalState &);
	void operator ++ ();

        void Reset(double T_) { return LSSym->Reset(T_); }
        double GetTemperature() const { return LSSym->GetTemperature(); }

private:

	Material::Symmetric::LocalState *LSSym;

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

	typedef Material::Symmetric::LinearizedKinematics::Energy<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	Energy(Material::Symmetric::Energy<0> *);
	Material::Energy<0> *Clone() const;
	Energy(const Energy<0> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Symmetric::Energy<0> *WSym;

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

	typedef Material::Symmetric::LinearizedKinematics::Energy<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Vector range_type;

	Energy();
	virtual ~Energy();
	Energy(Material::Symmetric::Energy<1> *);
	Material::Energy<1> *Clone() const;
	Energy(const Energy<1> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Symmetric::Energy<1> *DWSym;

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

	typedef Material::Symmetric::LinearizedKinematics::Energy<3> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy();
	virtual ~Energy();
	Energy(Material::Symmetric::Energy<2> *);
	Material::Energy<2> *Clone() const;
	Energy(const Energy<2> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Symmetric::Energy<2> *DDWSym;
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
class Jet<0> : public Material::Jet<0>
{
public: 

	typedef Material::Symmetric::LinearizedKinematics::Jet<1> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<double,Set::VectorSpace::Vector> range_type;

	Jet();
	virtual ~Jet();
	Jet(Material::Symmetric::Jet<0> *);
	Material::Jet<0> *Clone() const;
	Jet(const Jet<0> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Symmetric::Jet<0> *KSym;

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

	typedef Material::Symmetric::LinearizedKinematics::Jet<2> tangent_type;
	typedef Set::VectorSpace::Vector domain_type;
	typedef pair<Set::VectorSpace::Vector,Set::VectorSpace::Hom> range_type;

	Jet();
	virtual ~Jet();
	Jet(Material::Symmetric::Jet<1> *);
	Material::Jet<1> *Clone() const;
	Jet(const Jet<1> &);
	range_type operator () (const domain_type &) const;

private:

	Material::Symmetric::Jet<1> *DKSym;
	unsigned int map[3][3];

private:

	Jet<1> & operator = (const Jet<1> &);
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_LINEARIZEDKINEMATICS_H__INCLUDED_
