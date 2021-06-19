// Factory.h: Factory for the NoTension class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_NOTENSION_FACTORY_H__INCLUDED_)
#define MATERIAL_NOTENSION_FACTORY_H__INCLUDED_

#pragma once

#include "Material/Factory.h"
#include "./NoTension.h"

namespace Material
{
namespace NoTension
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public:
        typedef Material::Builder mat_build_type;
        typedef Material::Factory mat_fact_type;

	Factory() : LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), MatFact(0) {}
	virtual ~Factory() 
	{
	        if (MatFact != 0) delete MatFact;
		if (LS != 0)  delete LS;
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;                
	}

	Factory(mat_build_type *MatB_) :
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), MatFact(MatB_->Build()) {}

	Factory(const Factory &rhs) : 
		LS(0), W(0), DW(0), DDW(0), J(0), DJ(0), MatFact(rhs.MatFact) {}

	Material::Factory *Clone() const{return new Factory(*this);}

	Material::LocalState * GetLS()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		if (W == 0) W = new Material::NoTension::Energy<0>(LS, MatFact->GetW());
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		if (DW == 0) DW = new Material::NoTension::Energy<1>(LS, MatFact->GetDW());
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		if (DDW == 0) DDW = new Material::NoTension::Energy<2>(LS, MatFact->GetDDW());
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		if (J == 0) J = new Material::NoTension::Jet<0>(LS, MatFact->GetJ());
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) LS = new Material::NoTension::LocalState(MatFact->GetLS());
		if (DJ == 0) DJ = new Material::NoTension::Jet<1>(LS, MatFact->GetDJ());
		return DJ;
	}

private:

	mat_fact_type *MatFact;
	Material::NoTension::LocalState *LS;
	Material::NoTension::Energy<0> *W;
	Material::NoTension::Energy<1> *DW;
	Material::NoTension::Energy<2> *DDW;
	Material::NoTension::Jet<0> *J;
	Material::NoTension::Jet<1> *DJ;

private:

	Factory & operator = (const Factory &);
};

//////////////////////////////////////////////////////////////////////
// Class Builder
//////////////////////////////////////////////////////////////////////

class Builder : public Material::Builder
{
public: 

	typedef Material::Builder mat_builder_type;

	Builder() {}
	virtual ~Builder() {}

	Builder(mat_builder_type *MatB_) : MatB(MatB_) {}

	Material::Factory * Build() const
	{
		return new Material::NoTension::Factory(MatB);
	}

private:
        mat_builder_type *MatB;
};

}

}

#endif // !defined(MATERIAL_NOTENSION_FACTORY_H__INCLUDED_
