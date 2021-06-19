// Factory.h: Factory for the Incompressible_NeoHookean class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_INCOMPRESSIBLE_NEOHOOKEAN_FACTORY_H__INCLUDED_)
#define MATERIAL_INCOMPRESSIBLE_NEOHOOKEAN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Incompressible_NeoHookean.h"
#include "../Factory.h"

namespace Material
{
namespace Incompressible_NeoHookean
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Incompressible_NeoHookean::Data data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
	}

	Factory(Material::Incompressible_NeoHookean::Data *Dat_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) {}

	Factory(const Factory &rhs) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(rhs.Dat) {}

	Material::Factory *Clone() const{return new Factory(*this);}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		if (W == 0) W = new Material::Incompressible_NeoHookean::Energy<0>(LS);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		if (DW == 0) DW = new Material::Incompressible_NeoHookean::Energy<1>(LS);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		if (DDW == 0) DDW = new Material::Incompressible_NeoHookean::Energy<2>(LS);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		if (J == 0) J = new Material::Incompressible_NeoHookean::Jet<0>(LS);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Incompressible_NeoHookean::LocalState(Dat);
		if (DJ == 0) DJ = new Material::Incompressible_NeoHookean::Jet<1>(LS);
		return DJ;
	}

private:

	Material::Incompressible_NeoHookean::Data *Dat;
	Material::Incompressible_NeoHookean::LocalState *LS;
	Material::Incompressible_NeoHookean::Energy<0> *W;
	Material::Incompressible_NeoHookean::Energy<1> *DW;
	Material::Incompressible_NeoHookean::Energy<2> *DDW;
	Material::Incompressible_NeoHookean::Jet<0> *J;
	Material::Incompressible_NeoHookean::Jet<1> *DJ;

private:

	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::Incompressible_NeoHookean::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Incompressible_NeoHookean::Data *Dat_) : Dat(Dat_) {}

	Material::Factory * Build() const
	{
		return new Material::Incompressible_NeoHookean::Factory(Dat);
	}

private:

	Material::Incompressible_NeoHookean::Data *Dat;
};

}

}

#endif // !defined(MATERIAL_INCOMPRESSIBLE_NEOHOOKEAN_FACTORY_H__INCLUDED_
