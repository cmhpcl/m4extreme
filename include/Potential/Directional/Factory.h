// Factory.h: interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////


#if !defined(POTENTIAL_DIRECTIONAL_FACTORY_H__INCLUDED_)
#define POTENTIAL_DIRECTIONAL_FACTORY_H__INCLUDED_

#pragma once

#include "Directional.h"

namespace Potential
{
namespace Directional
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory
{
public: 

	Factory() {}
	virtual ~Factory() {}
	virtual Potential::Directional::LocalState * GetLS()=0;
	virtual Potential::Directional::Energy<0> * GetE()=0;
	virtual Potential::Directional::Energy<1> * GetDE()=0;
	virtual Potential::Directional::Energy<2> * GetDDE()=0;
	virtual Potential::Directional::Jet<0> * GetJ()=0;
	virtual Potential::Directional::Jet<1> * GetDJ()=0;
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder 
{
public: 

	Builder() {}
	virtual ~Builder() {}
	virtual Potential::Directional::Factory * Build() const=0;
};

}

}

#endif // !defined(POTENTIAL_DIRECTIONAL_FACTORY_H__INCLUDED_
