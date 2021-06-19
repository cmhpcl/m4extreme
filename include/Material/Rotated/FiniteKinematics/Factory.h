// Factory.h: Interface for the Factory class.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_ROTATED_FINITEKINEMATICS_FACTORY_H__INCLUDED_)
#define MATERIAL_ROTATED_FINITEKINEMATICS_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./FiniteKinematics.h"
#include "../../Factory.h"

namespace Material
{
namespace Rotated
{
namespace FiniteKinematics
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

        Factory() : R(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}
	virtual ~Factory() 
	{
		if (W != 0)   delete W;
		if (DW != 0)  delete DW;
		if (DDW != 0) delete DDW;
		if (J != 0)   delete J;
		if (DJ != 0)  delete DJ;
		if (LS != 0)  delete LS;
	}

	Factory(Set::VectorSpace::Hom *R_, Material::Builder *B0_) : 
	  R(R_), Fact0(B0_->Build()), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {	  
	}

	Material::Factory *Clone() const{return new Factory(*this);}

	void NewLS()
	{
 	        LS = new Material::Rotated::FiniteKinematics::LocalState(Fact0->GetLS(),R, 0);
	}

	Material::LocalState * GetLS()
	{
		if (LS == 0) NewLS(); 
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (LS == 0) NewLS(); 
		if (W == 0) W = new Material::Rotated::FiniteKinematics::Energy<0>(LS,Fact0->GetW());
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (LS == 0) NewLS(); 
		if (DW == 0) DW = new Material::Rotated::FiniteKinematics::Energy<1>(LS,Fact0->GetDW());
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (LS == 0) NewLS(); 
		if (DDW == 0) DDW = new Material::Rotated::FiniteKinematics::Energy<2>(LS,Fact0->GetDDW());
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (LS == 0) NewLS(); 
		if (J == 0) J = new Material::Rotated::FiniteKinematics::Jet<0>(LS,Fact0->GetJ());
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (LS == 0) NewLS(); 
		if (DJ == 0) DJ = new Material::Rotated::FiniteKinematics::Jet<1>(LS,Fact0->GetDJ());
		return DJ;
	}

	Set::VectorSpace::Hom * GetR() { return R; }

private:
	Set::VectorSpace::Hom *R;

	Material::Factory *Fact0;
	Material::Rotated::FiniteKinematics::LocalState *LS;
	Material::Rotated::FiniteKinematics::Energy<0> *W;
	Material::Rotated::FiniteKinematics::Energy<1> *DW;
	Material::Rotated::FiniteKinematics::Energy<2> *DDW;
	Material::Rotated::FiniteKinematics::Jet<0> *J;
	Material::Rotated::FiniteKinematics::Jet<1> *DJ;

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

	Builder(Set::VectorSpace::Hom *R_, Material::Builder *B0_) : R(R_), B0(B0_) {}

	Material::Factory * Build() const
	{
		return new Material::Rotated::FiniteKinematics::Factory(R,B0);
	}

private:

	Set::VectorSpace::Hom *R;
	Material::Builder *B0;

private:

	Builder(const Builder &);
	Builder & operator = (const Builder &);
};

}

}

}

#endif // !defined(MATERIAL_ROTATED_FINITEKINEMATICS_FACTORY_H__INCLUDED_
