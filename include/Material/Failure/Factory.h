// Factory.h: interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_FAILURE_FACTORY_H__INCLUDED_)
#define MATERIAL_FAILURE_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "Failure.h"
#include "../Factory.h"

namespace Material
{
namespace Failure
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	Factory() : Fact1(0), Fact2(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
	        if(Fact1 != 0) delete Fact1; 
		if(Fact2 != 0) delete Fact2; 
		if (DJ     != 0) delete DJ;
		if (J      != 0) delete J;
		if (DDW    != 0) delete DDW;
		if (DW     != 0) delete DW;
		if (W      != 0) delete W;
		if (LS     != 0) delete LS;
	}

	Factory(Material::Builder *B1_, Material::Builder *B2_) : 
	  Fact1(B1_->Build()), Fact2(B2_->Build()), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	void NewLS()
	{
		LS = new Material::Failure::LocalState(Fact1->GetLS(),Fact2->GetLS());
	}

	Material::LocalState * GetLS()
	{
		if (LS == 0) NewLS();
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) NewLS();
		if (W  == 0) W = new Material::Failure::Energy<0>(LS,Fact1->GetW(),Fact2->GetW());
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS();
		if (DW  == 0) DW = new Material::Failure::Energy<1>(LS,Fact1->GetDW(),Fact2->GetDW());
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS  == 0) NewLS();
		if (DDW  == 0) DDW = new Material::Failure::Energy<2>(LS,Fact1->GetDDW(),Fact2->GetDDW());
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS();
		if (J  == 0) J = new Material::Failure::Jet<0>(LS,Fact1->GetJ(),Fact2->GetJ());
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS();
		if (DJ  == 0) DJ = new Material::Failure::Jet<1>(LS,Fact1->GetDJ(),Fact2->GetDJ());
		return DJ;
	}

private:

	Material::Factory *Fact1; 
	Material::Factory *Fact2;
	Material::Failure::LocalState *LS;
	Material::Failure::Energy<0> *W;
	Material::Failure::Energy<1> *DW;
	Material::Failure::Energy<2> *DDW;
	Material::Failure::Jet<0> *J;
	Material::Failure::Jet<1> *DJ;

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

	Builder() {}
	virtual ~Builder() {}

	Builder(Material::Builder *B1_, Material::Builder *B2_) : 
		B1(B1_), B2(B2_) {}

	Material::Factory * Build() const
	{
		return new Material::Failure::Factory(B1,B2);
	}

private:

	Material::Builder *B1;
	Material::Builder *B2;
};

}

}

#endif // !defined(MATERIAL_FAILURE_FACTORY_H__INCLUDED_
