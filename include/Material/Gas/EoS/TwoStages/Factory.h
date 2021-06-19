// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOS_TWOSTAGES_FACTORY_H_INCLUDED_)
#define MATERIAL_GAS_EOS_TWOSTAGES_FACTORY_H_INCLUDED_

#pragma once

#include <vector>
#include "./TwoStages.h"
#include "Material/Gas/EoS/Factory.h"

namespace Material
{
namespace Gas
{
namespace EoS
{
namespace TwoStages
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Gas::EoS::Factory
{
public: 

        Factory() : Fcomp(0), Ften(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
	        if (Fcomp != 0 ) delete Fcomp;
		if (Ften != 0 ) delete Ften;
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
	}

	Factory(Material::Gas::EoS::TwoStages::Data *Prop_,
		Material::Gas::EoS::Builder * Bcomp_, 
		Material::Gas::EoS::Builder *Bten_) : 
	  Fcomp(Bcomp_->Build()), Ften(Bten_->Build()), Prop(Prop_),
	  LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	Material::Gas::EoS::LocalState * GetLS()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    return LS;
	}

	Material::Gas::EoS::Energy<0> * GetW()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    if (W == 0) W = new Material::Gas::EoS::TwoStages::Energy<0>(LS, Fcomp->GetW(), Ften->GetW());
	    return W;
	}

	Material::Gas::EoS::Energy<1> * GetDW()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    if (DW == 0) DW = new Material::Gas::EoS::TwoStages::Energy<1>(LS, Fcomp->GetDW(),Ften->GetDW());		
	    return DW;
	}

	Material::Gas::EoS::Energy<2> * GetDDW()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    if (DDW == 0) DDW = new Material::Gas::EoS::TwoStages::Energy<2>(LS, Fcomp->GetDDW(), Ften->GetDDW());
	    return DDW;
	}

	Material::Gas::EoS::Jet<0> * GetJ()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    if (J == 0) J = new Material::Gas::EoS::TwoStages::Jet<0>(LS, Fcomp->GetJ(), Ften->GetJ());
	    return J;
	}

	Material::Gas::EoS::Jet<1> * GetDJ()
	{
	    if (LS == 0) LS = new Material::Gas::EoS::TwoStages::LocalState(Prop, Fcomp->GetLS(), Ften->GetLS());
	    if (DJ == 0) DJ = new Material::Gas::EoS::TwoStages::Jet<1>(LS, Fcomp->GetDJ(),Ften->GetDJ());
	    return DJ;
	}

private:
	Material::Gas::EoS::TwoStages::Data *Prop;
	Material::Gas::EoS::Factory *Fcomp;
	Material::Gas::EoS::Factory *Ften;
	Material::Gas::EoS::TwoStages::LocalState *LS;
	Material::Gas::EoS::TwoStages::Energy<0> *W;
	Material::Gas::EoS::TwoStages::Energy<1> *DW;
	Material::Gas::EoS::TwoStages::Energy<2> *DDW;
	Material::Gas::EoS::TwoStages::Jet<0> *J;
	Material::Gas::EoS::TwoStages::Jet<1> *DJ;

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

	Builder(Material::Gas::EoS::TwoStages::Data *Prop_,
		Material::Gas::EoS::Builder *Bcomp_, 
		Material::Gas::EoS::Builder *Bten_) : Prop(Prop_), Bcomp(Bcomp_), Bten(Bten_) {}

	Material::Gas::EoS::Factory * Build() const
	{
	  return new Material::Gas::EoS::TwoStages::Factory(Prop, Bcomp, Bten);
	}

private:
	Material::Gas::EoS::TwoStages::Data *Prop;
	Material::Gas::EoS::Builder *Bcomp; 
	Material::Gas::EoS::Builder *Bten;
};

}

}

}

}

#endif // !defined(MATERIAL_GAS_EOS_TWOSTAGES_FACTORY_H_INCLUDED_)
