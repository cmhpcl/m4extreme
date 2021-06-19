// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_THERMALLK2FK_FACTORY_H__INCLUDED_)
#define MATERIAL_GAS_EOS_THERMALLK2FK_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./ThermalLK2FK.h"
#include "Material/Gas/EoS/Factory.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace ThermalLK2FK
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Gas::EoS::Factory
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
	}

	Factory(Material::Gas::EoS::ThermalLK2FK::Data * Prop_,
		Material::Gas::EoS::Builder *Bloc_) : 
	  LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), Fact(Bloc_->Build()), Prop(Prop_) {}

	Material::Gas::EoS::LocalState * GetLS()
	{
	        if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		return LS;
	}

	Material::Gas::EoS::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		if (W == 0) W = new Material::Gas::EoS::ThermalLK2FK::Energy<0>(LS, Fact->GetW());
		return W;
	}

	Material::Gas::EoS::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		if (DW == 0) DW = new Material::Gas::EoS::ThermalLK2FK::Energy<1>(LS, Fact->GetDW());
		return DW;
	}

	Material::Gas::EoS::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		if (DDW == 0) DDW = new Material::Gas::EoS::ThermalLK2FK::Energy<2>(LS, Fact->GetDJ());
		return DDW;
	}

	Material::Gas::EoS::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		if (J == 0) J = new Material::Gas::EoS::ThermalLK2FK::Jet<0>(LS, Fact->GetJ());
		return J;
	}

	Material::Gas::EoS::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::Gas::EoS::ThermalLK2FK::LocalState(Prop, Fact->GetLS());
		if (DJ == 0) DJ = new Material::Gas::EoS::ThermalLK2FK::Jet<1>(LS, Fact->GetDJ());
		return DJ;
	}

private:
	Material::Gas::EoS::Factory *Fact;
	Material::Gas::EoS::ThermalLK2FK::Data * Prop;
	Material::Gas::EoS::ThermalLK2FK::LocalState *LS;
	Material::Gas::EoS::ThermalLK2FK::Energy<0> *W;
	Material::Gas::EoS::ThermalLK2FK::Energy<1> *DW;
	Material::Gas::EoS::ThermalLK2FK::Energy<2> *DDW;
	Material::Gas::EoS::ThermalLK2FK::Jet<0> *J;
	Material::Gas::EoS::ThermalLK2FK::Jet<1> *DJ;

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

	Builder(Material::Gas::EoS::ThermalLK2FK::Data * Prop_,
		Material::Gas::EoS::Builder *Bloc_) : Prop(Prop_), Bloc(Bloc_) {}

	Material::Gas::EoS::Factory * Build() const
	{
	  return new Material::Gas::EoS::ThermalLK2FK::Factory(Prop, Bloc);
	}

private:
	Material::Gas::EoS::ThermalLK2FK::Data * Prop;
	Material::Gas::EoS::Builder *Bloc;
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_THERMALLK2FK_FACTORY_H__INCLUDED_
