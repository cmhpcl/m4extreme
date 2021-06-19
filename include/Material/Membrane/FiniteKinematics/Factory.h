// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_MEMBRANE_FINITEKINEMATICS_FACTORY_H__INCLUDED_)
#define MATERIAL_MEMBRANE_FINITEKINEMATICS_FACTORY_H__INCLUDED_

#pragma once

#include <vector>
#include "./FiniteKinematics.h"
#include "../../Material.h"
#include "../../Factory.h"

namespace Material
{
namespace Membrane
{
namespace FiniteKinematics
{
//////////////////////////////////////////////////////////////////////
// Class Factory
//////////////////////////////////////////////////////////////////////

class Factory : public Material::Factory
{
public: 

	Factory() : MF(0), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) {}

	virtual ~Factory() 
	{
		if (DJ     != 0) delete DJ;
		if (J      != 0) delete J;
		if (DDW    != 0) delete DDW;
		if (DW     != 0) delete DW;
		if (W      != 0) delete W;
		if (LS     != 0) delete LS;
		if (MF     != 0) delete MF;
	}

	Factory(const unsigned int &n, Material::Builder *MB) : 
		MF(MB->Build()), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) 
	{
		LS = new Material::Membrane::FiniteKinematics::LocalState(n,MF->GetLS(),MF->GetDW(),MF->GetDDW());
	}

	Factory(const Set::VectorSpace::Vector &N, Material::Builder *MB) : 
		MF(MB->Build()), LS(0), W(0), DW(0), DDW(0), J(0), DJ(0) 
	{
		LS = new Material::Membrane::FiniteKinematics::LocalState(N,MF->GetLS(),MF->GetDW(),MF->GetDDW());
	}

	Material::LocalState * GetLS()
	{
		return LS;
	}

	Material::Energy<0> * GetW()
	{
		if (W == 0) W = new Material::Membrane::Energy<0>(LS,MF->GetW());
		return W;
	}

	Material::Energy<1> * GetDW()
	{
		if (DW == 0) DW = new Material::Membrane::Energy<1>(LS,MF->GetDW());
		return DW;
	}

	Material::Energy<2> * GetDDW()
	{
		if (DDW == 0) DDW = new Material::Membrane::Energy<2>(LS,MF->GetDDW());
		return DDW;
	}

	Material::Jet<0> * GetJ()
	{
		if (J == 0) J = new Material::Membrane::Jet<0>(LS,MF->GetJ());
		return J;
	}

	Material::Jet<1> * GetDJ()
	{
		if (DJ == 0) DJ = new Material::Membrane::Jet<1>(LS,MF->GetDJ());
		return DJ;
	}

	Material::Factory * GetMF()
	{
		return MF;
	}

private:

	Material::Factory *MF;
	Material::Membrane::FiniteKinematics::LocalState *LS;
	Material::Membrane::Energy<0> *W;
	Material::Membrane::Energy<1> *DW;
	Material::Membrane::Energy<2> *DDW;
	Material::Membrane::Jet<0> *J;
	Material::Membrane::Jet<1> *DJ;

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

	Builder(const unsigned int &n_, Material::Builder *B_) : n(n_),B(B_) {}

	Material::Factory * Build() const
	{
		return new Material::Membrane::FiniteKinematics::Factory(n,B);
	}

private:

	unsigned int n;
	Material::Builder *B;
};

}

}

}

#endif // !defined(MATERIAL_MEMBRANE_FINITEKINEMATICS_FACTORY_H__INCLUDED_
