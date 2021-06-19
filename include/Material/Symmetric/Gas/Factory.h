// Factory.h: Factory for the Symmetric::Gas class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_SYMMETRIC_GAS_FACTORY_H__INCLUDED_)
#define MATERIAL_SYMMETRIC_GAS_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "../../Gas/EoS/Factory.h"
#include "../../Factory.h"

namespace Material
{
namespace Symmetric
{
namespace Gas
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory 
{
public: 

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
		if (FEoS != 0)  delete FEoS;
	}

	Factory(Material::Gas::EoS::Builder *BEoS_) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) 
	{
		FEoS = BEoS_->Build();
	}

	Material::Symmetric::Gas::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		return LS;
	}

	Material::Symmetric::Gas::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		if (W == 0) W = new Material::Symmetric::Gas::Energy<0>(LS,FEoS->GetW());
		return W;
	}

	Material::Symmetric::Gas::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		if (DW == 0) DW = new Material::Symmetric::Gas::Energy<1>(LS,FEoS->GetDW());
		return DW;
	}

	Material::Symmetric::Gas::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		if (DDW == 0) DDW = new Material::Symmetric::Gas::Energy<2>(LS,FEoS->GetDJ());
		return DDW;
	}

	Material::Symmetric::Gas::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		if (J == 0) J = new Material::Symmetric::Gas::Jet<0>(LS,FEoS->GetJ());
		return J;
	}

	Material::Symmetric::Gas::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Symmetric::Gas::LocalState(FEoS->GetLS());
		if (DJ == 0) DJ = new Material::Symmetric::Gas::Jet<1>(LS,FEoS->GetDJ());
		return DJ;
	}

private:

	Material::Symmetric::Gas::LocalState *LS;
	Material::Symmetric::Gas::Energy<0> *W;
	Material::Symmetric::Gas::Energy<1> *DW;
	Material::Symmetric::Gas::Energy<2> *DDW;
	Material::Symmetric::Gas::Jet<0> *J;
	Material::Symmetric::Gas::Jet<1> *DJ;
	Material::Gas::EoS::Factory *FEoS;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder 
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Gas::EoS::Builder *BEoS_) : BEoS(BEoS_) {}

	Material::Symmetric::Gas::Factory * Build() const
	{
		return new Material::Symmetric::Gas::Factory(BEoS);
	}

private:

	Material::Gas::EoS::Builder *BEoS;
};

}

}

}

#endif // !defined(MATERIAL_SYMMETRIC_GAS_FACTORY_H__INCLUDED_
