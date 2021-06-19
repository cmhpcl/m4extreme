// Factory.h: Interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_HOOKEAN_CUBIC_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_HOOKEAN_CUBIC_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Cubic.h"
#include "../../Factory.h"

namespace Material
{
namespace Symmetric
{
namespace Hookean
{
namespace Cubic
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (W       != 0) delete W;
		if (DW      != 0) delete DW;
		if (DDW     != 0) delete DDW;
		if (J       != 0) delete J;
		if (DJ      != 0) delete DJ;
		if (LS      != 0) delete LS;
	}

	Factory(Material::Symmetric::Hookean::Cubic::Data *Dat_) : Dat(Dat_),
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		if (LS == 0) LS  = new Material::Symmetric::Hookean::Cubic::LocalState(Dat);
	}

	Material::Symmetric::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W    == 0) W     = new Material::Symmetric::Hookean::Cubic::Energy<0>(LS);
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (LS    == 0) NewLS();
		if (DW    == 0) DW     = new Material::Symmetric::Hookean::Cubic::Energy<1>(LS);
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (LS    == 0) NewLS();
		if (DDW   == 0) DDW    = new Material::Symmetric::Hookean::Cubic::Energy<2>(LS);
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (LS   == 0) NewLS();
		if (J    == 0) J     = new Material::Symmetric::Hookean::Cubic::Jet<0>(LS);
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ    == 0) DJ     = new Material::Symmetric::Hookean::Cubic::Jet<1>(LS);
		return DJ;
	}

private:

	Material::Symmetric::Hookean::Cubic::Data *Dat;
	Material::Symmetric::Hookean::Cubic::LocalState *LS;
	Material::Symmetric::Hookean::Cubic::Energy<0> *W;
	Material::Symmetric::Hookean::Cubic::Energy<1> *DW;
	Material::Symmetric::Hookean::Cubic::Energy<2> *DDW;
	Material::Symmetric::Hookean::Cubic::Jet<0> *J;
	Material::Symmetric::Hookean::Cubic::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Symmetric::Builder
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Symmetric::Hookean::Cubic::Data *D_) : D(D_) {}

	Material::Symmetric::Factory * Build() const
	{
		return new Material::Symmetric::Hookean::Cubic::Factory(D);
	}

private:

	Material::Symmetric::Hookean::Cubic::Data *D;
};

}

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_HOOKEAN_CUBIC_FACTORY_H__INCLUDED_
