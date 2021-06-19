// Symmetric.h: interface for the Symmetric class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_H__INCLUDED_

#pragma once

#include <utility>
#include "../../Set/Algebraic/VectorSpace/Category/Symmetric/Symmetric.h"

namespace Material
{
namespace Symmetric
{
class LocalState;
template <unsigned int> class Energy;
template <unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState
{
public: 

	LocalState();
	virtual ~LocalState();
	virtual Material::Symmetric::LocalState *Clone() const=0;
	virtual void operator ++ ()=0;
        virtual void Reset(double) { return; }
	virtual void Relax() { return; }
        virtual double GetTemperature() const { return 0.0; }
        virtual void write(ostream & os) const { return; }
	virtual void read(istream & is) { return; }
};

//////////////////////////////////////////////////////////////////////
// Class Energy<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Energy;

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<>
class Energy<0>
{
public: 

	typedef Material::Symmetric::Energy<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef double range_type;

	Energy();
	virtual ~Energy();
	virtual Material::Symmetric::Energy<0> *Clone() const=0;

	// W(F)
	virtual range_type operator () (const domain_type &) const=0;

	// W(F, T)
	virtual range_type operator () (const domain_type &, double) const { return 0.0; }
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<1>
{
public: 

	typedef Material::Symmetric::Energy<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::SymDual range_type;

	Energy();
	virtual ~Energy();
	virtual Material::Symmetric::Energy<1> *Clone() const=0;

	// P = dW/dF
	virtual range_type operator () (const domain_type &) const=0;

	// N = dW/dT
	virtual double operator() (const domain_type &, double) const { return 0.0; }
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template <>
class Energy<2>
{
public: 

	typedef Material::Symmetric::Energy<3> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef Set::VectorSpace::Hom range_type;

	Energy();
	virtual ~Energy();
	virtual Material::Symmetric::Energy<2> *Clone() const=0;

	// ddW/dFdF
	virtual range_type operator () (const domain_type &) const=0;

	// ddw/dTdT
	virtual double operator() (const domain_type &, double) const { return 0.0; }
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<>
class Jet<0>
{
public: 

	typedef Material::Symmetric::Jet<1> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<double,Set::VectorSpace::SymDual> range_type;

	Jet();
	virtual ~Jet();
	virtual Material::Symmetric::Jet<0> *Clone() const=0;
	virtual range_type operator () (const domain_type &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <>
class Jet<1>
{
public: 

	typedef Material::Symmetric::Jet<2> tangent_type;
	typedef Set::VectorSpace::Sym domain_type;
	typedef pair<Set::VectorSpace::SymDual,Set::VectorSpace::Hom> range_type;

	Jet();
	virtual ~Jet();
	virtual Material::Symmetric::Jet<1> *Clone() const=0;
	virtual range_type operator () (const domain_type &) const=0;
};

}

}

#endif // !defined(MATERIAL_SYMMETRIC_H__INCLUDED_
