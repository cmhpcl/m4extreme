// Factory.h: interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(FLUX_FIELD_FACTORY_H__INCLUDED_)
#define FLUX_FIELD_FACTORY_H__INCLUDED_

#pragma once

#include "Field.h"

namespace Flux
{
namespace Field
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory
{
public: 

	Factory() {}
	virtual ~Factory() {}
	virtual Flux::Field::LocalState * GetLS()=0;
	virtual Flux::Field::Energy<1> * GetDW()=0;
	virtual Flux::Field::Energy<2> * GetDDW()=0;
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder 
{
public: 

	Builder() {}
	virtual ~Builder() {}
	virtual Flux::Field::Factory * Build() const=0;
};

}

}

#endif // !defined(FLUX_FIELD_FACTORY_H__INCLUDED_
