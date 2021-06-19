// Uniaxial.h: interface for the Uniaxial class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_UNIAXIAL__INCLUDED_)
#define MATERIAL_UNIAXIAL__INCLUDED_

#pragma once

#include <utility>

using namespace std;

namespace Material
{
namespace Uniaxial
{
//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState
{
public: 

	LocalState(){}
	virtual ~LocalState(){}
	virtual LocalState *Clone() const=0;
	virtual void operator ++ ()=0;
	virtual void Reset(double) {}
};

//////////////////////////////////////////////////////////////////////
// Class Energy<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int> class Energy;

//////////////////////////////////////////////////////////////////////
// Class Energy<0>
//////////////////////////////////////////////////////////////////////

template<> class Energy<0>
{
public: 

	typedef Material::Uniaxial::Energy<1> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	Energy(){}
	virtual ~Energy(){}
	virtual Energy<0> *Clone() const=0;
	virtual range_type operator () (const double &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Energy<1>
//////////////////////////////////////////////////////////////////////

template<> class Energy<1>
{
public: 

	typedef Material::Uniaxial::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	Energy(){}
	virtual ~Energy(){}
	virtual Energy<1> *Clone() const=0;
	virtual range_type operator () (const double &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Energy<2>
//////////////////////////////////////////////////////////////////////

template<> class Energy<2>
{
public: 

	typedef Material::Uniaxial::Energy<2> tangent_type;
	typedef double domain_type;
	typedef double range_type;

	Energy(){}
	virtual ~Energy(){}
	virtual Energy<2> *Clone() const=0;
	virtual range_type operator () (const double &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet;

//////////////////////////////////////////////////////////////////////
// Class Jet<0>
//////////////////////////////////////////////////////////////////////

template<> class Jet<0>
{
public: 

	typedef Material::Uniaxial::Jet<1> tangent_type;
	typedef double domain_type;
	typedef pair<double, double> range_type;

	Jet(){}
	virtual ~Jet(){}
	virtual Jet<0> *Clone() const=0;
	virtual range_type operator () (const domain_type &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Jet<1>
//////////////////////////////////////////////////////////////////////

template <> class Jet<1>
{
public: 

	typedef Material::Uniaxial::Jet<2> tangent_type;
	typedef double domain_type;
	typedef pair<double,double> range_type;

	Jet(){}
	virtual ~Jet(){}
	virtual Jet<1> *Clone() const=0;
	virtual range_type operator () (const domain_type &) const=0;
};

}

}

#endif // !defined(MATERIAL_UNIAXIAL__INCLUDED_)
