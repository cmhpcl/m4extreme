///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
///////////////////////////////////////////////////////////////////////////////


#if !defined(MATERIAL_CAMCLAY_COMPRESSIBLE_FACTORY_H__INCLUDED_)
#define MATERIAL_CAMCLAY_COMPRESSIBLE_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "CamClayFiniteDeformation.h"
#include "Material/Factory.h"

namespace Material
{
namespace CamClayFiniteDeformation
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	typedef Material::CamClayFiniteDeformation::Data data_type;

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

	Factory( Material::CamClayFiniteDeformation::Data *Dat_, Clock *C_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_),C(C_) {}

	Factory(const Factory &rhs) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(rhs.Dat) {}

	Material::Factory *Clone() const{return new Factory(*this);}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		if (W == 0) W = new Material::CamClayFiniteDeformation::Energy<0>(LS);
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		if (DW == 0) DW = new Material::CamClayFiniteDeformation::Energy<1>(LS);
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		if (DDW == 0) DDW = new Material::CamClayFiniteDeformation::Energy<2>(LS);
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		if (J == 0) J = new Material::CamClayFiniteDeformation::Jet<0>(LS);
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::CamClayFiniteDeformation::LocalState(C,Dat);
		if (DJ == 0) DJ = new Material::CamClayFiniteDeformation::Jet<1>(LS);
		return DJ;
	}

private:

	Clock *C;
	Material::CamClayFiniteDeformation::Data *Dat;
	Material::CamClayFiniteDeformation::LocalState *LS;
	Material::CamClayFiniteDeformation::Energy<0> *W;
	Material::CamClayFiniteDeformation::Energy<1> *DW;
	Material::CamClayFiniteDeformation::Energy<2> *DDW;
	Material::CamClayFiniteDeformation::Jet<0> *J;
	Material::CamClayFiniteDeformation::Jet<1> *DJ;

private:

	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::CamClayFiniteDeformation::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::CamClayFiniteDeformation::Data *Dat_, Clock *C_) : Dat(Dat_), C(C_)  {}

	Material::Factory * Build() const
	{
		return new Material::CamClayFiniteDeformation::Factory(Dat,C);
	}

private:

	Clock *C;
	Material::CamClayFiniteDeformation::Data *Dat;
};

}

}

#endif // !defined(MATERIAL_CAMCLAY_COMPRESSIBLE_FACTORY_H__INCLUDED_
