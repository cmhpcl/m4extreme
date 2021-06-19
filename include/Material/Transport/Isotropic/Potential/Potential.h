// Potential.h: interface for the Potential class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL__INCLUDED_)
#define MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL__INCLUDED_

#pragma once

#include <utility>

using namespace std;

namespace Material
{
namespace Transport
{
namespace Isotropic
{
namespace Potential
{
class LocalState;
template<unsigned int> class Energy;
template<unsigned int> class Jet;

//////////////////////////////////////////////////////////////////////
// Class LocalState
//////////////////////////////////////////////////////////////////////

class LocalState
{
public: 

	LocalState();
	virtual ~LocalState();
	virtual LocalState *Clone() const=0;
	virtual void operator ++ ()=0;
};

//////////////////////////////////////////////////////////////////////
// Class Energy<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Energy
{
public: 

	Energy(){}
	virtual ~Energy(){}
	virtual Energy<p> *Clone() const=0;
	virtual double operator () (const double &) const=0;
};

//////////////////////////////////////////////////////////////////////
// Class Jet<p>
//////////////////////////////////////////////////////////////////////

template<unsigned int p> class Jet
{
public: 

	Jet(){}
	virtual ~Jet(){}
	virtual Jet<p> *Clone() const=0;
	virtual pair<double,double> operator () (const double &) const=0;
};

}

}

}

}

#endif // !defined(MATERIAL_TRANSPORT_ISOTROPIC_POTENTIAL__INCLUDED_)
