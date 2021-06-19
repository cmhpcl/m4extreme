// Factory.h: Factory for the Yeoh class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_YEOH_FACTORY_H__INCLUDED_)
#define MATERIAL_NEOHOOKEAN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Yeoh.h"
#include "../Factory.h"

namespace Material
{
namespace Yeoh
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::Yeoh::Data data_type;

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

	Factory(Material::Yeoh::Data *Dat_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) {}

	Factory(const Factory &rhs) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(rhs.Dat) {}

	Material::Factory *Clone() const{return new Factory(*this);}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		if (W == 0) W = new Material::Yeoh::Energy<0>(LS);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		if (DW == 0) DW = new Material::Yeoh::Energy<1>(LS);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		if (DDW == 0) DDW = new Material::Yeoh::Energy<2>(LS);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		if (J == 0) J = new Material::Yeoh::Jet<0>(LS);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Yeoh::LocalState(Dat);
		if (DJ == 0) DJ = new Material::Yeoh::Jet<1>(LS);
		return DJ;
	}

private:

	Material::Yeoh::Data *Dat;
	Material::Yeoh::LocalState *LS;
	Material::Yeoh::Energy<0> *W;
	Material::Yeoh::Energy<1> *DW;
	Material::Yeoh::Energy<2> *DDW;
	Material::Yeoh::Jet<0> *J;
	Material::Yeoh::Jet<1> *DJ;

private:

	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::Yeoh::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Yeoh::Data *Dat_) : Dat(Dat_) {}

	Material::Factory * Build() const
	{
		return new Material::Yeoh::Factory(Dat);
	}

private:

	Material::Yeoh::Data *Dat;
};

}

}

#endif // !defined(MATERIAL_YEOH_FACTORY_H__INCLUDED_
