// Factory.h: Factory for the Symmetric::Ogden class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_OGDEN_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_OGDEN_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./Ogden.h"
#include "../Gas/Factory.h"
#include "../Factory.h"
#include "../../Gas/EoS/Factory.h"

namespace Material
{
namespace Symmetric
{
namespace Ogden
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Symmetric::Factory
{
public: 

	typedef Material::Symmetric::Ogden::Data data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)   delete LS;
		if (W != 0)    delete W;
		if (DW != 0)   delete DW;
		if (DDW != 0)  delete DDW;
		if (J != 0)    delete J;
		if (DJ != 0)   delete DJ;
		if (FGas != 0) delete FGas;      
	}

	Factory(Material::Symmetric::Ogden::Data *Dat_, 
		Material::Gas::EoS::Builder *BEoS_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Dat(Dat_) 
	{
		Material::Symmetric::Gas::Builder BGas(BEoS_);
		FGas = BGas.Build();
	}

	Material::Symmetric::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		return LS;
	}

	Material::Symmetric::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		if (W == 0) W = new Material::Symmetric::Ogden::Energy<0>(LS,FGas->GetW());
		return W;
	}

	Material::Symmetric::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		if (DW == 0) DW = new Material::Symmetric::Ogden::Energy<1>(LS,FGas->GetDW());
		return DW;
	}

	Material::Symmetric::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		if (DDW == 0) DDW = new Material::Symmetric::Ogden::Energy<2>(LS,FGas->GetDDW());
		return DDW;
	}

	Material::Symmetric::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		if (J == 0) J = new Material::Symmetric::Ogden::Jet<0>(LS,FGas->GetJ());
		return J;
	}

	Material::Symmetric::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Symmetric::Ogden::LocalState(Dat,FGas->GetLS());
		if (DJ == 0) DJ = new Material::Symmetric::Ogden::Jet<1>(LS,FGas->GetDJ());
		return DJ;
	}

private:

	Material::Symmetric::Gas::Factory *FGas;
	Material::Symmetric::Ogden::Data *Dat;
	Material::Symmetric::Ogden::LocalState *LS;
	Material::Symmetric::Ogden::Energy<0> *W;
	Material::Symmetric::Ogden::Energy<1> *DW;
	Material::Symmetric::Ogden::Energy<2> *DDW;
	Material::Symmetric::Ogden::Jet<0> *J;
	Material::Symmetric::Ogden::Jet<1> *DJ;

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

	typedef Material::Symmetric::Ogden::Data data_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Symmetric::Ogden::Data *Dat_, 
		Material::Gas::EoS::Builder *BEoS_) : Dat(Dat_), BEoS(BEoS_){}

	Material::Symmetric::Factory * Build() const
	{
		return new Material::Symmetric::Ogden::Factory(Dat,BEoS);
	}

private:

	Material::Symmetric::Ogden::Data *Dat;
	Material::Gas::EoS::Builder *BEoS;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_OGDEN_FACTORY_H__INCLUDED_
