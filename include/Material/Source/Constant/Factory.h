// Factory.h: Factory for the NeoHookean class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SOURCE_CONSTANT_FACTORY_H__INCLUDED_)
#define MATERIAL_SOURCE_CONSTANT_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Constant.h"
#include "../../Factory.h"
#include "../../../Potential/Field/Field.h"

namespace Material
{
namespace Source
{
namespace Constant
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Source::Constant::Data data_type;

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

	Factory(Material::Source::Constant::Data *Dat_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) {}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		if (W == 0) W = new Material::Source::Constant::Energy<0>(LS);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		if (DW == 0) DW = new Material::Source::Constant::Energy<1>(LS);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		if (DDW == 0) DDW = new Material::Source::Constant::Energy<2>(LS);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		if (J == 0) J = new Material::Source::Constant::Jet<0>(LS);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Source::Constant::LocalState(Dat);
		if (DJ == 0) DJ = new Material::Source::Constant::Jet<1>(LS);
		return DJ;
	}

private:

	Material::Source::Constant::Data *Dat;
	Material::Source::Constant::LocalState *LS;
	Material::Source::Constant::Energy<0> *W;
	Material::Source::Constant::Energy<1> *DW;
	Material::Source::Constant::Energy<2> *DDW;
	Material::Source::Constant::Jet<0> *J;
	Material::Source::Constant::Jet<1> *DJ;

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

	typedef Material::Source::Constant::Data data_type;
	typedef Material::Source::Constant::Factory fact_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(data_type *Dat_) : Dat(Dat_) {}

	Material::Factory * Build() const
	{
		return new fact_type(Dat);
	}

private:

	data_type *Dat;
};

//////////////////////////////////////////////////////////////////////
// Class LocalBuilder
//////////////////////////////////////////////////////////////////////

class LocalBuilder : public Material::LocalBuilder
{
public: 

	typedef Material::Source::Constant::Data data_type;
	typedef Material::Source::Constant::Factory fact_type;
	typedef Potential::Field::Energy<1> dens_type;

	LocalBuilder() : D(0), f(0) {}
	virtual ~LocalBuilder() 
	{
		if (D != 0) delete D;
	}

	LocalBuilder(dens_type *f_) : D(0), f(f_) {}

	Material::Factory * Build(const Set::VectorSpace::Vector &x)
	{
		D = new data_type((*f)(x));
		return new fact_type(D);
	}

private:

	dens_type *f;
	data_type *D;
};

}

}

}

#endif // !defined(MATERIAL_SOURCE_CONSTANT_FACTORY_H__INCLUDED_
