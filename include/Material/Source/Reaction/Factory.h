// Factory.h: Factory for the Source::Reaction class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SOURCE_REACTION_FACTORY_H__INCLUDED_)
#define MATERIAL_SOURCE_REACTION_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Reaction.h"
#include "Material/Factory.h"

namespace Material
{
namespace Source
{
namespace Reaction
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Source::Reaction::Data data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		//if (DW != 0)  delete DW;
		//if (DDW != 0) delete DDW;
		//if (J != 0)   delete J;
		//if (DJ != 0)  delete DJ;
	}

        Factory(double T0_, Clock *Chronos_, Material::Source::Reaction::Data *Dat_) : 
	T0(T0_), Chronos(Chronos_), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) {}

	Material::LocalState * GetLS()
	{
	  if (LS == 0) LS = new Material::Source::Reaction::LocalState(T0, Chronos, Dat);
	  return LS;
	}

	Material::Energy<0> * GetW()
	{
	  if (LS == 0) LS = new Material::Source::Reaction::LocalState(T0, Chronos, Dat);
	  if (W == 0) W = new Material::Source::Reaction::Energy<0>(LS);
	  return W;
	}

	Material::Energy<1> * GetDW()
	{
		return NULL;
	}

	Material::Energy<2> * GetDDW()
	{
		return NULL;
	}

	Material::Jet<0> * GetJ()
	{
		return NULL;
	}

	Material::Jet<1> * GetDJ()
	{
		return NULL;
	}

private:
	double T0;
	Clock *Chronos;
	Material::Source::Reaction::Data *Dat;
	Material::Source::Reaction::LocalState *LS;
	Material::Source::Reaction::Energy<0> *W;
	Material::Source::Reaction::Energy<1> *DW;
	Material::Source::Reaction::Energy<2> *DDW;
	Material::Source::Reaction::Jet<0> *J;
	Material::Source::Reaction::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::Source::Reaction::Data data_type;
	typedef Material::Source::Reaction::Factory fact_type;

	Builder() {}
	virtual ~Builder() {}

        Builder(double T0_, Clock *Chronos_, data_type *Dat_) : 
	Dat(Dat_), Chronos(Chronos_), T0(T0_) {}

	Material::Factory * Build() const
	{
	  return new fact_type(T0, Chronos, Dat);
	}

private:
	double T0;
	Clock *Chronos;
	data_type *Dat;
};

}

}

}

#endif // !defined(MATERIAL_SOURCE_REACTION_FACTORY_H__INCLUDED_
