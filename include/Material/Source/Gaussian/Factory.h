// Factory.h: Factory for the Source::Gaussian class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SOURCE_GAUSSIAN_FACTORY_H__INCLUDED_)
#define MATERIAL_SOURCE_GAUSSIAN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Gaussian.h"
#include "Material/Factory.h"

namespace Material
{
namespace Source
{
namespace Gaussian
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Source::Gaussian::Data data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
//		if (DW != 0)  delete DW;
//		if (DDW != 0) delete DDW;
//		if (J != 0)   delete J;
//		if (DJ != 0)  delete DJ;
	}

	Factory(Clock *Chronos_, Material::Source::Gaussian::Data *Dat_) : 
		Chronos(Chronos_), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) {}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Source::Gaussian::LocalState(Chronos, Dat);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
	  if (LS == 0) GetLS();
	  if (W == 0) W = new Material::Source::Gaussian::Energy<0>(LS);
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
	Clock *Chronos;
	Material::Source::Gaussian::Data *Dat;
	Material::Source::Gaussian::LocalState *LS;
	Material::Source::Gaussian::Energy<0> *W;
	Material::Source::Gaussian::Energy<1> *DW;
	Material::Source::Gaussian::Energy<2> *DDW;
	Material::Source::Gaussian::Jet<0> *J;
	Material::Source::Gaussian::Jet<1> *DJ;

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

	typedef Material::Source::Gaussian::Data data_type;
	typedef Material::Source::Gaussian::Factory fact_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Clock *Chronos_, data_type *Dat_) : 
	Chronos(Chronos_), Dat(Dat_) {}

	Material::Factory * Build() const
	{
		return new fact_type(Chronos, Dat);
	}

private:
	Clock *Chronos;
	data_type *Dat;
};

}

}

}

#endif // !defined(MATERIAL_SOURCE_GAUSSIAN_FACTORY_H__INCLUDED_
