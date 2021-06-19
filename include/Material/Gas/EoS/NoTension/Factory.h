// Factory.h: Factory for the NoTension class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_NOTENSION_FACTORY_H__INCLUDED_)
#define MATERIAL_GAS_EOS_NOTENSION_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./NoTension.h"
#include "../Factory.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace NoTension
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Gas::EoS::Factory
{
public: 

	typedef Material::Gas::EoS::NoTension::Factory data_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
	        if (Fact != 0) delete Fact;
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
	}

	Factory(Material::Gas::EoS::Builder *Bloc_) : 
	  LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Fact(Bloc_->Build()) {}

	void NewLS()
	{
		LS = new Material::Gas::EoS::NoTension::LocalState(Fact->GetLS());
	}

	Material::Gas::EoS::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Gas::EoS::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W == 0) W = new Material::Gas::EoS::NoTension::Energy<0>(Fact->GetW());
		return W;
	}

	Material::Gas::EoS::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS();
		if (DW == 0) DW = new Material::Gas::EoS::NoTension::Energy<1>(Fact->GetDW());
		return DW;
	}

	Material::Gas::EoS::Energy<2> * GetDDW()
	{
		if (LS == 0) NewLS();
		if (DDW == 0) DDW = new Material::Gas::EoS::NoTension::Energy<2>(Fact->GetDDW());
		return DDW;
	}

	Material::Gas::EoS::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS();
		if (J == 0) J = new Material::Gas::EoS::NoTension::Jet<0>(Fact->GetJ());
		return J;
	}

	Material::Gas::EoS::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ == 0) DJ = new Material::Gas::EoS::NoTension::Jet<1>(Fact->GetDJ());
		return DJ;
	}

private:

	Material::Gas::EoS::Factory *Fact;
	Material::Gas::EoS::NoTension::LocalState *LS;
	Material::Gas::EoS::NoTension::Energy<0> *W;
	Material::Gas::EoS::NoTension::Energy<1> *DW;
	Material::Gas::EoS::NoTension::Energy<2> *DDW;
	Material::Gas::EoS::NoTension::Jet<0> *J;
	Material::Gas::EoS::NoTension::Jet<1> *DJ;

private:

	Factory(const Factory &);
	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Gas::EoS::Builder
{
public: 

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Gas::EoS::Builder *Bloc_) : Bloc(Bloc_) {}

	Material::Gas::EoS::Factory * Build() const
	{
		return new Material::Gas::EoS::NoTension::Factory(Bloc);
	}

private:

	Material::Gas::EoS::Builder *Bloc;
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_NOTENSION_FACTORY_H__INCLUDED_
